#pragma once

#include "camera.h"
#include "vector2.h"
#include "player_id.h"
#include "platform.h"
#include "animation.h"	
#include <graphics.h>

extern std::vector<Platform> platform_list;

class Player
{
public:
	Player()
	{
		current_animation = &animation_idle_right;
	}
	~Player() = default;

	virtual void on_update(int delta)
	{
		//通过二者作差的方式来判断玩家的移动方向
		int direction = is_right_key_down - is_left_key_down;

		if (direction != 0)
		{
			is_fating_right = direction > 0;
			//根据玩家的移动方向来切换动画
			current_animation = is_fating_right ? &animation_run_right : &animation_run_left;
			//在这一帧中玩家在水平方向上移动的距离存储在distance中
			float distance = direction * run_velocity * delta;
			on_run(distance);
		}
		else
		{
			//如果玩家没有移动，那么就播放默认动画
			current_animation = is_fating_right ? &animation_idle_right : &animation_idle_left;
		}
		//更新动画
		current_animation->on_update(delta);

		move_and_collide(delta);
	}

	virtual void on_draw(const Camera& camera)
	{
		//对当前正在播放的动画进行绘制
		current_animation->on_draw(camera, (int)position.x, (int)position.y);
	}

	virtual void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
					// 'A'
				case 0x41:
					is_left_key_down = true;
					break;
					// 'D'
				case 0x44:
					is_right_key_down = true;
					break;
					// 'W'
				case 0x57:
					on_jump();
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
					// '←'
				case VK_LEFT:
					is_left_key_down = true;
					break;
					// '→'
				case VK_RIGHT:
					is_right_key_down = true;
					break;
					// '↑'
				case VK_UP:
					on_jump();
					break;
				}

			default:
				break;
			}
			break;
		case WM_KEYUP:
			switch (id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
					// 'A'
				case 0x41:
					is_left_key_down = false;
					break;
					// 'D'
				case 0x44:
					is_right_key_down = false;
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
					// '←'
				case VK_LEFT:
					is_left_key_down = false;
					break;
					// '→'
				case VK_RIGHT:
					is_right_key_down = false;
					break;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	void set_id(PlayerID id)
	{
		this->id = id;
	}

	//设置玩家的位置
	void set_position(float x,float y)
	{
		position.x = x, position.y = y;
	}

	virtual void on_run(float distance)
	{
		position.x += distance;
	}

	virtual void on_jump()
	{
		//如果玩家正在下落，那么就不允许再次跳跃
		if (velocity.y != 0)
		{
			return;
		}
		//设置玩家的垂直速度为跳跃速度
		velocity.y += jump_velocity;
	}


protected:
	//物理相关的所有代码
	void move_and_collide(int delta)
	{
		//根据重力加速度的值来更新玩家的速度
		velocity.y += gravity * delta;
		//根据速度来更新玩家的位置
		position += velocity * (float)delta;

		//玩家的速度向下时检测碰撞
		if (velocity.y > 0)
		{
			//遍历所有的平台对象
			for (const Platform& platform : platform_list)
			{
				//获取平台的碰撞形状
				const Platform::CollisionShape& shape = platform.shape;
				//检测玩家是否与平台发生碰撞
				//  1.	position.x + size.x：玩家的右边界。
				//  2.	shape.right：平台的右边界。
				//	3.	max(position.x + size.x, shape.right)：玩家和平台右边界中的最大值。
				//	4.	position.x：玩家的左边界。
				/*	5.	shape.left：平台的左边界。
					6.	min(position.x, shape.left)：玩家和平台左边界中的最小值。
					7.	max(position.x + size.x, shape.right) - min(position.x, shape.left)：玩家和平台的总宽度。
					8.	size.x：玩家的宽度。
					9.	shape.right - shape.left：平台的宽度。
					10.	size.x + (shape.right - shape.left)：玩家和平台的宽度之和。
					这段代码的逻辑是：如果玩家和平台的总宽度小于等于玩家和平台的宽度之和，
					则说明玩家和平台在水平方向上发生了重叠，即发生了碰撞。*/
				bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left)
					<= size.x + (shape.right - shape.left));
				/*这段代码用于检测玩家在垂直方向上是否与平台发生碰撞，并处理碰撞后的逻辑。
				具体来说，它通过比较玩家和平台的边界来确定是否发生了重叠，
				并在发生碰撞时调整玩家的位置和速度。*/
				bool is_collide_y = (shape.y >= position.y && shape.y <= position.y + size.y);

				/*	这行代码用于检测玩家在垂直方向上是否与平台发生碰撞。
					具体来说，它检查平台的y坐标是否在玩家的顶部和底部之间。
					1.	if (is_collide_x && is_collide_y)：如果玩家在水平方向和垂直方向上都与平台发生了碰撞。
					2.	float delta_pos_y = velocity.y * delta; ：计算玩家在这一帧中在垂直方向上移动的距离。
					3.	float last_tick_foot_pos_y = position.y + size.y - delta_pos_y; ：计算玩家在上一帧的脚部位置。
					4.	if (last_tick_foot_pos_y <= shape.y)：如果玩家在上一帧的脚部位置在平台的y坐标之上。
					5.	position.y = shape.y - size.y; ：将玩家的位置调整到平台的顶部。
					6.	velocity.y = 0; ：将玩家的垂直速度设置为0，以停止玩家的下落。
					7.	break; ：跳出循环，因为玩家已经与平台发生了碰撞。*/
				if (is_collide_x && is_collide_y)
				{
					float delta_pos_y = velocity.y * delta;
					float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;
					if (last_tick_foot_pos_y <= shape.y)
					{
						position.y = shape.y - size.y;
						velocity.y = 0;
						break;
					}
				}
			}
		}
	}


protected:
	const float gravity = 1.6e-3f;		//玩家的重力
	const float run_velocity = 0.55f;	//玩家的移动速度
	const float jump_velocity = -0.85f;	//玩家的跳跃速度


protected:
	Vector2 size;		//玩家的尺寸
	Vector2 position;	//记录玩家的位置
	Vector2 velocity;	//记录玩家的速度

	Animation animation_idle_left;			//玩家朝向左的默认动画
	Animation animation_idle_right;			//玩家朝向右的默认动画
	Animation animation_run_left;	  		//玩家朝向左的跑步动画
	Animation animation_run_right;			//玩家朝向右的跑步动画

	Animation* current_animation = nullptr; //当前正在播放的动画

	PlayerID id = PlayerID::P1;				//玩家ID

	bool is_left_key_down = false;			//左键是否按下
	bool is_right_key_down = false;			//右键是否按下

	bool is_fating_right = true;			//玩家是否朝向右边
};

