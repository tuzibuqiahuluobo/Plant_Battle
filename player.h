#pragma once

#include "camera.h"
#include "vector2.h"
#include "player_id.h"
#include "animation.h"	
#include <graphics.h>


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
		}
		else
		{
			//如果玩家没有移动，那么就播放默认动画
			current_animation = is_fating_right ? &animation_idle_right : &animation_idle_left;
		}
		//更新动画
		current_animation->on_update(delta);
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
				}
				break;
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


protected:
	Vector2 position; //记录玩家的位置

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

