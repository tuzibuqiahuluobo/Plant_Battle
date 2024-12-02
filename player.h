#pragma once

#include "camera.h"
#include "vector2.h"
#include "particle.h"
#include "player_id.h"
#include "platform.h"
#include "animation.h"	
#include <graphics.h>

extern bool is_debug; // 是否为调试模式

extern IMAGE img_1P_cursor; // 1P 指示光标图片
extern IMAGE img_2P_cursor; // 2P 指示光标图片

extern Atlas atlas_run_effect; // 奔跑特效图集
extern Atlas atlas_jump_effect; // 跳跃特效图集
extern Atlas atlas_land_effect; // 落地特效图集

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;

class Player
{
public:
	Player(bool facing_right = true) : is_facing_right(facing_right)
	{
		current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;

		//初始化普通攻击冷却定时器
		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]() 
			{ 
				can_attack = true;
			});

		//初始化无敌时间定时器
		timer_invulnerable.set_wait_time(750);
		timer_invulnerable.set_one_shot(true);
		timer_invulnerable.set_callback([&]() 
			{ 
				is_invulnerable = false;
			});

		//初始化无敌动画闪烁定时器
		timer_invulnerable_blink.set_wait_time(75);
		timer_invulnerable_blink.set_callback([&]()
			{
				is_showing_sketch_frame = !is_showing_sketch_frame;
			});

		//初始化奔跑特效定时器
		timer_run_effect_generation.set_wait_time(75);
		timer_run_effect_generation.set_callback([&]()//[&] 表示捕获外部作用域中的所有变量。
			{
				Vector2 particle_position;//声明了一个 Vector2 类型的变量 particle_position，用于存储粒子的位置
				IMAGE* frame = atlas_run_effect.get_image(0);//从 atlas_run_effect 中获取第一个图像帧，并将其指针存储在 frame 变量中
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				//将粒子的位置设置在玩家位置的中心。position.x 是玩家的 x 轴位置，
				// size.x 是玩家的宽度，frame->getwidth() 是图像帧的宽度
				particle_list.emplace_back(particle_position, &atlas_run_effect, 45);
				//这行代码在 particle_list 中添加一个新的粒子对象。
				// 粒子的位置是 particle_position，
				// 使用的图像集是 atlas_run_effect，
				// 粒子的生命周期是 45 毫秒。
			});

		//初始化死亡特效定时器
		timer_die_effect_generation.set_wait_time(35);
		timer_die_effect_generation.set_callback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 150);
			});

		//初始化跳跃特效定时器
		animation_jump_effect.set_atlas(&atlas_jump_effect);
		animation_jump_effect.set_interval(25);
		animation_jump_effect.set_loop(false);
		animation_jump_effect.set_callback([&]()
			{
				is_jump_effect_visible = false;
			});

		//初始化落地特效定时器
		animation_land_effect.set_atlas(&atlas_land_effect);
		animation_land_effect.set_interval(50);
		animation_land_effect.set_loop(false);
		animation_land_effect.set_callback([&]()
			{
				is_land_effect_visible = false;
			});

		//初始化指示器
		timer_cursor_visibility.set_wait_time(2500);
		timer_cursor_visibility.set_one_shot(true);
		timer_cursor_visibility.set_callback([&]()
			{
				is_cursor_visible = false;
			});

	}
	~Player() = default;

	virtual void on_update(int delta)
	{
		//通过二者作差的方式来判断玩家的移动方向
		int direction = is_right_key_down - is_left_key_down;

		if (direction != 0)
		{
			if (!is_attacking_ex)
			{
				is_facing_right = direction > 0;
			}
			//根据玩家的移动方向来切换动画
			current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
			//在这一帧中玩家在水平方向上移动的距离存储在distance中
			float distance = direction * run_velocity * delta;
			on_run(distance);
		}
		else
		{
			//如果玩家没有移动，那么就播放默认动画
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
			timer_run_effect_generation.pause();
		}

		if (is_attacking_ex)
		{
			current_animation = is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;

		}

		if (hp <= 0)
		{
			current_animation = last_hurt_direcrion.x < 0 ? &animation_die_left : &animation_die_right;
		}

		//更新动画
		current_animation->on_update(delta);

		timer_attack_cd.on_update(delta);

		//更新无敌时间定时器
		timer_invulnerable.on_update(delta);
		//更新无敌动画闪烁定时器
		timer_invulnerable_blink.on_update(delta);

		//更新奔跑特效动画
		timer_run_effect_generation.on_update(delta);

		//更新跳跃特效动画
		animation_jump_effect.on_update(delta);
		//更新落地特效动画
		animation_land_effect.on_update(delta);

		//更新指示器
		timer_cursor_visibility.on_update(delta);

		if (hp <= 0)
		{
			timer_die_effect_generation.on_update(delta);
		}

		particle_list.erase(std::remove_if(
			particle_list.begin(),
			particle_list.end(), 
			[](const Particle& particle)
			{
				return !particle.check_valid();
			}),
			particle_list.end());

		for (Particle& particle : particle_list)
		{
			particle.on_update(delta);
		}


		if (is_showing_sketch_frame)
		{
			sketch_image(current_animation->get_frame(), &img_sketch);
		}

		move_and_collide(delta);

		// 检查大招是否结束，如果结束，重置 is_attacking_ex 状态
		if (current_animation->check_finished() && is_attacking_ex)
		{
			is_attacking_ex = false;
			// 重置动画为默认动画
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
		}
	}

	virtual void on_draw(const Camera& camera)
	{
		if (is_jump_effect_visible)
		{
			animation_jump_effect.on_draw(camera, (int)position_jump_effect.x, (int)position_jump_effect.y);
		}

		if (is_land_effect_visible)
		{
			animation_land_effect.on_draw(camera, (int)position_land_effect.x, (int)position_land_effect.y);
		}

		for (const Particle& particle : particle_list)
		{
			particle.on_draw(camera);
		}

		if (hp > 0 && is_invulnerable && is_showing_sketch_frame)
		{
			putimage_alpha(camera, (int)position.x, (int)position.y, &img_sketch);
		}
		else
		{
			//对当前正在播放的动画进行绘制
			current_animation->on_draw(camera, (int)position.x, (int)position.y);
		}

		if (is_cursor_visible)
		{
			switch (id)
			{
			case PlayerID::P1:
				putimage_alpha(camera, (int)position.x + (size.x - img_1P_cursor.getwidth()) / 2,
					(int)(position.y - img_1P_cursor.getheight()), &img_1P_cursor);
				break;
			case PlayerID::P2:
				putimage_alpha(camera, (int)position.x + (size.x - img_2P_cursor.getwidth()) / 2,
					(int)(position.y - img_2P_cursor.getheight()), &img_2P_cursor);
				break;
			}
		}

		if (is_debug)
		{
			setlinecolor(RGB(0, 125, 255));
			rectangle((int)position.x, (int)position.y, (int)(position.x + size.x), (int)(position.y + size.y));
		}
		
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
					// 'K'
				case 0x4B:
					on_jump();
					break;
					// 'J'
				case 0x4A:
					if (can_attack)
					{
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}
					break;
					// 'L'
				case 0x4C:
					if (mp >= 100)
					{
						on_attack_ex();
						mp = 0;
					}
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
					// '2'
				case VK_NUMPAD2:
					on_jump();
					break;
					// '1'
				case VK_NUMPAD1:
					if (can_attack)
					{
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}
					break;
					// '3'
				case VK_NUMPAD3:
					if (mp >= 100)
					{
						on_attack_ex();
						mp = 0;
					}
					break;
				}
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
		if (is_attacking_ex)
		{
			return;
		}

		position.x += distance;
		timer_run_effect_generation.resume();
	}

	virtual void on_jump()
	{
		//如果玩家正在下落或者正在进行特殊攻击，那么就不允许再次跳跃
		if (velocity.y != 0 || is_attacking_ex)
		{
			return;
		}
		//设置玩家的垂直速度为跳跃速度
		velocity.y += jump_velocity;
		//播放跳跃特效动画
		is_jump_effect_visible = true;
		animation_jump_effect.reset();

		IMAGE* effect_frame = animation_jump_effect.get_frame();
		position_jump_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
		position_jump_effect.y = position.y + size.y - effect_frame->getheight();
	}

	virtual void on_land()
	{
		is_land_effect_visible = true;
		animation_land_effect.reset();

		IMAGE* effect_frame = animation_land_effect.get_frame();
		position_land_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
		position_land_effect.y = position.y + size.y - effect_frame->getheight();
	}
	
	//攻击逻辑
	virtual void on_attack()
	{
		
	}

	//特殊攻击逻辑
	virtual void on_attack_ex()
	{
	}

	const Vector2& get_position() const
	{
		return position;
	}

	const Vector2& get_size() const
	{
		return size;
	}

	void make_invulnerable()
	{
		is_invulnerable = true;
		timer_invulnerable.restart();
	}

	void set_hp(int val)
	{
		hp = val;
	}

	int get_hp() const
	{
		return hp;
	}

	int get_mp() const
	{
		return mp;
	}



protected:
	//物理相关的所有代码
	void move_and_collide(int delta)
	{
		float last_velocity_y = velocity.y;

		//根据重力加速度的值来更新玩家的速度
		velocity.y += gravity * delta;
		//根据速度来更新玩家的位置
		position += velocity * (float)delta;

		if (hp <= 0)
		{
			return;
		}

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

						//只有在前一帧y轴速度不等于0，并且当前帧y轴速度等于0时才触发落地事件
						if (last_velocity_y != 0)
						{
							on_land();
						}
						break;
					}
				}
			}
		}

		if (!is_invulnerable)
		{
			for (Bullet* bullet : bullet_list)
			{
				if (!bullet->get_valid() || bullet->get_collide_target() != id)
				{
					continue;
				}

				if (bullet->check_collision(position, size))
				{
					make_invulnerable();
					bullet->on_collide();
					bullet->set_valid(false);
					hp -= bullet->get_damage();
					last_hurt_direcrion = bullet->get_position() - position;
					if (hp <= 0)
					{
						velocity.x = last_hurt_direcrion.x < 0 ? 0.35f : -0.35f;
						velocity.y = -1.0f;
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
	int mp = 0;		    //玩家能量值
	int hp = 100;		//玩家的生命值

	Vector2 size;		//玩家的尺寸
	Vector2 position;	//记录玩家的位置
	Vector2 velocity;	//记录玩家的速度

	Animation animation_idle_left;			//玩家朝向左的默认动画
	Animation animation_idle_right;			//玩家朝向右的默认动画
	Animation animation_run_left;	  		//玩家朝向左的跑步动画
	Animation animation_run_right;			//玩家朝向右的跑步动画
	Animation animation_attack_ex_left;		//玩家朝向左的特殊攻击动画
	Animation animation_attack_ex_right;	//玩家朝向右的特殊攻击动画

	Animation animation_die_left;			//玩家朝向左的死亡动画
	Animation animation_die_right;			//玩家朝向右的死亡动画

	Animation animation_jump_effect;		//跳跃特效动画
	Animation animation_land_effect;		//落地特效动画

	bool is_jump_effect_visible = false;	//跳跃特效动画是否可见
	bool is_land_effect_visible = false;	//落地特效动画是否可见

	Vector2 position_jump_effect;			//跳跃特效动画的位置
	Vector2 position_land_effect;			//落地特效动画的位置

	Animation* current_animation = nullptr; //当前正在播放的动画

	PlayerID id = PlayerID::P1;				//玩家ID

	bool is_left_key_down = false;			//左键是否按下
	bool is_right_key_down = false;			//右键是否按下

	bool is_facing_right = true;			//玩家是否朝向右边

	int attack_cd = 500;					//普通攻击冷却时间
	bool can_attack = true;					//玩家是否可以释放普通攻击
	Timer timer_attack_cd;					//普通攻击冷却定时器

	bool is_attacking_ex = false;			//玩家是否正在释放特殊攻击

	bool is_invulnerable = false;			//玩家是否无敌
	bool is_showing_sketch_frame = false;	//当前帧是否应该显示剪影

	Timer timer_invulnerable;				//无敌状态定时器
	Timer timer_invulnerable_blink;			//无敌状态闪烁定时器

	IMAGE img_sketch;						//剪影图像

	Timer timer_run_effect_generation;		//奔跑特效粒子发射定时器
	Timer timer_die_effect_generation;		//死亡特效粒子发射定时器

	std::vector<Particle> particle_list;	//粒子对象数组

	bool is_cursor_visible = true;			//指示光标是否可见
	Timer timer_cursor_visibility;			//指示光标可见性定时器

	Vector2 last_hurt_direcrion;			//最后一次受击的方向
};

