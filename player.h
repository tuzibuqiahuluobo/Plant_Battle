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
		//ͨ����������ķ�ʽ���ж���ҵ��ƶ�����
		int direction = is_right_key_down - is_left_key_down;

		if (direction != 0)
		{
			is_fating_right = direction > 0;
			//������ҵ��ƶ��������л�����
			current_animation = is_fating_right ? &animation_run_right : &animation_run_left;
		}
		else
		{
			//������û���ƶ�����ô�Ͳ���Ĭ�϶���
			current_animation = is_fating_right ? &animation_idle_right : &animation_idle_left;
		}
		//���¶���
		current_animation->on_update(delta);
	}

	virtual void on_draw(const Camera& camera)
	{
		//�Ե�ǰ���ڲ��ŵĶ������л���
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
					// '��'
				case VK_LEFT:
					is_left_key_down = true;
					break;
					// '��'
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
					// '��'
				case VK_LEFT:
					is_left_key_down = false;
					break;
					// '��'
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

	//������ҵ�λ��
	void set_position(float x,float y)
	{
		position.x = x, position.y = y;
	}


protected:
	Vector2 position; //��¼��ҵ�λ��

	Animation animation_idle_left;			//��ҳ������Ĭ�϶���
	Animation animation_idle_right;			//��ҳ����ҵ�Ĭ�϶���
	Animation animation_run_left;	  		//��ҳ�������ܲ�����
	Animation animation_run_right;			//��ҳ����ҵ��ܲ�����

	Animation* current_animation = nullptr; //��ǰ���ڲ��ŵĶ���

	PlayerID id = PlayerID::P1;				//���ID

	bool is_left_key_down = false;			//����Ƿ���
	bool is_right_key_down = false;			//�Ҽ��Ƿ���

	bool is_fating_right = true;			//����Ƿ����ұ�
};

