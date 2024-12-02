#pragma once

#include "camera.h"
#include "vector2.h"
#include "player_id.h"
#include "platform.h"
#include "animation.h"	
#include <graphics.h>

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;

class Player
{
public:
	Player()
	{
		current_animation = &animation_idle_right;

		//��ʼ����ͨ������ȴ��ʱ��
		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]() { can_attack = true; });
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
			//����һ֡�������ˮƽ�������ƶ��ľ���洢��distance��
			float distance = direction * run_velocity * delta;
			on_run(distance);
		}
		else
		{
			//������û���ƶ�����ô�Ͳ���Ĭ�϶���
			current_animation = is_fating_right ? &animation_idle_right : &animation_idle_left;
		}
		//���¶���
		current_animation->on_update(delta);

		timer_attack_cd.on_update(delta);

		move_and_collide(delta);
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
					// '��'
				case VK_LEFT:
					is_left_key_down = true;
					break;
					// '��'
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

	virtual void on_run(float distance)
	{
		if (is_attacking_ex)
		{
			return;
		}

		position.x += distance;
	}

	virtual void on_jump()
	{
		//��������������������ڽ������⹥������ô�Ͳ������ٴ���Ծ
		if (velocity.y != 0 || is_attacking_ex)
		{
			return;
		}
		//������ҵĴ�ֱ�ٶ�Ϊ��Ծ�ٶ�
		velocity.y += jump_velocity;
	}
	
	//�����߼�
	virtual void on_attack()
	{
		
	}

	//���⹥���߼�
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



protected:
	//������ص����д���
	void move_and_collide(int delta)
	{
		//�����������ٶȵ�ֵ��������ҵ��ٶ�
		velocity.y += gravity * delta;
		//�����ٶ���������ҵ�λ��
		position += velocity * (float)delta;

		//��ҵ��ٶ�����ʱ�����ײ
		if (velocity.y > 0)
		{
			//�������е�ƽ̨����
			for (const Platform& platform : platform_list)
			{
				//��ȡƽ̨����ײ��״
				const Platform::CollisionShape& shape = platform.shape;
				//�������Ƿ���ƽ̨������ײ
				//  1.	position.x + size.x����ҵ��ұ߽硣
				//  2.	shape.right��ƽ̨���ұ߽硣
				//	3.	max(position.x + size.x, shape.right)����Һ�ƽ̨�ұ߽��е����ֵ��
				//	4.	position.x����ҵ���߽硣
				/*	5.	shape.left��ƽ̨����߽硣
					6.	min(position.x, shape.left)����Һ�ƽ̨��߽��е���Сֵ��
					7.	max(position.x + size.x, shape.right) - min(position.x, shape.left)����Һ�ƽ̨���ܿ�ȡ�
					8.	size.x����ҵĿ�ȡ�
					9.	shape.right - shape.left��ƽ̨�Ŀ�ȡ�
					10.	size.x + (shape.right - shape.left)����Һ�ƽ̨�Ŀ��֮�͡�
					��δ�����߼��ǣ������Һ�ƽ̨���ܿ��С�ڵ�����Һ�ƽ̨�Ŀ��֮�ͣ�
					��˵����Һ�ƽ̨��ˮƽ�����Ϸ������ص�������������ײ��*/
				bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left)
					<= size.x + (shape.right - shape.left));
				/*��δ������ڼ������ڴ�ֱ�������Ƿ���ƽ̨������ײ����������ײ����߼���
				������˵����ͨ���Ƚ���Һ�ƽ̨�ı߽���ȷ���Ƿ������ص���
				���ڷ�����ײʱ������ҵ�λ�ú��ٶȡ�*/
				bool is_collide_y = (shape.y >= position.y && shape.y <= position.y + size.y);

				/*	���д������ڼ������ڴ�ֱ�������Ƿ���ƽ̨������ײ��
					������˵�������ƽ̨��y�����Ƿ�����ҵĶ����͵ײ�֮�䡣
					1.	if (is_collide_x && is_collide_y)����������ˮƽ����ʹ�ֱ�����϶���ƽ̨��������ײ��
					2.	float delta_pos_y = velocity.y * delta; �������������һ֡���ڴ�ֱ�������ƶ��ľ��롣
					3.	float last_tick_foot_pos_y = position.y + size.y - delta_pos_y; �������������һ֡�ĽŲ�λ�á�
					4.	if (last_tick_foot_pos_y <= shape.y)������������һ֡�ĽŲ�λ����ƽ̨��y����֮�ϡ�
					5.	position.y = shape.y - size.y; ������ҵ�λ�õ�����ƽ̨�Ķ�����
					6.	velocity.y = 0; ������ҵĴ�ֱ�ٶ�����Ϊ0����ֹͣ��ҵ����䡣
					7.	break; ������ѭ������Ϊ����Ѿ���ƽ̨��������ײ��*/
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

		for (Bullet* bullet :bullet_list)
		{
			if (!bullet->get_valid() || bullet->get_collide_target() != id)
			{
				continue;
			}

			if (bullet->check_collision(position,size))
			{
				bullet->on_collide();
				bullet->set_valid(false);
				hp -= bullet->get_damage();
			}

		}
	}


protected:
	const float gravity = 1.6e-3f;		//��ҵ�����
	const float run_velocity = 0.55f;	//��ҵ��ƶ��ٶ�
	const float jump_velocity = -0.85f;	//��ҵ���Ծ�ٶ�


protected:
	int mp = 0;		    //�������ֵ
	int hp = 100;		//��ҵ�����ֵ

	Vector2 size;		//��ҵĳߴ�
	Vector2 position;	//��¼��ҵ�λ��
	Vector2 velocity;	//��¼��ҵ��ٶ�

	Animation animation_idle_left;			//��ҳ������Ĭ�϶���
	Animation animation_idle_right;			//��ҳ����ҵ�Ĭ�϶���
	Animation animation_run_left;	  		//��ҳ�������ܲ�����
	Animation animation_run_right;			//��ҳ����ҵ��ܲ�����
	Animation animation_attack_ex_left;		//��ҳ���������⹥������
	Animation animation_attack_ex_right;	//��ҳ����ҵ����⹥������


	Animation* current_animation = nullptr; //��ǰ���ڲ��ŵĶ���

	PlayerID id = PlayerID::P1;				//���ID

	bool is_left_key_down = false;			//����Ƿ���
	bool is_right_key_down = false;			//�Ҽ��Ƿ���

	bool is_fating_right = true;			//����Ƿ����ұ�

	int attack_cd = 500;					//��ͨ������ȴʱ��
	bool can_attack = true;					//����Ƿ�����ͷ���ͨ����
	Timer timer_attack_cd;					//��ͨ������ȴ��ʱ��

	bool is_attacking_ex = false;			//����Ƿ������ͷ����⹥��
};

