#pragma once

#include "camera.h"
#include "vector2.h"
#include "particle.h"
#include "player_id.h"
#include "platform.h"
#include "animation.h"	
#include <graphics.h>

extern bool is_debug; // �Ƿ�Ϊ����ģʽ

extern IMAGE img_1P_cursor; // 1P ָʾ���ͼƬ
extern IMAGE img_2P_cursor; // 2P ָʾ���ͼƬ

extern Atlas atlas_run_effect; // ������Чͼ��
extern Atlas atlas_jump_effect; // ��Ծ��Чͼ��
extern Atlas atlas_land_effect; // �����Чͼ��

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;

class Player
{
public:
	Player(bool facing_right = true) : is_facing_right(facing_right)
	{
		current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;

		//��ʼ����ͨ������ȴ��ʱ��
		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]() 
			{ 
				can_attack = true;
			});

		//��ʼ���޵�ʱ�䶨ʱ��
		timer_invulnerable.set_wait_time(750);
		timer_invulnerable.set_one_shot(true);
		timer_invulnerable.set_callback([&]() 
			{ 
				is_invulnerable = false;
			});

		//��ʼ���޵ж�����˸��ʱ��
		timer_invulnerable_blink.set_wait_time(75);
		timer_invulnerable_blink.set_callback([&]()
			{
				is_showing_sketch_frame = !is_showing_sketch_frame;
			});

		//��ʼ��������Ч��ʱ��
		timer_run_effect_generation.set_wait_time(75);
		timer_run_effect_generation.set_callback([&]()//[&] ��ʾ�����ⲿ�������е����б�����
			{
				Vector2 particle_position;//������һ�� Vector2 ���͵ı��� particle_position�����ڴ洢���ӵ�λ��
				IMAGE* frame = atlas_run_effect.get_image(0);//�� atlas_run_effect �л�ȡ��һ��ͼ��֡��������ָ��洢�� frame ������
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				//�����ӵ�λ�����������λ�õ����ġ�position.x ����ҵ� x ��λ�ã�
				// size.x ����ҵĿ�ȣ�frame->getwidth() ��ͼ��֡�Ŀ��
				particle_list.emplace_back(particle_position, &atlas_run_effect, 45);
				//���д����� particle_list �����һ���µ����Ӷ���
				// ���ӵ�λ���� particle_position��
				// ʹ�õ�ͼ���� atlas_run_effect��
				// ���ӵ����������� 45 ���롣
			});

		//��ʼ��������Ч��ʱ��
		timer_die_effect_generation.set_wait_time(35);
		timer_die_effect_generation.set_callback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 150);
			});

		//��ʼ����Ծ��Ч��ʱ��
		animation_jump_effect.set_atlas(&atlas_jump_effect);
		animation_jump_effect.set_interval(25);
		animation_jump_effect.set_loop(false);
		animation_jump_effect.set_callback([&]()
			{
				is_jump_effect_visible = false;
			});

		//��ʼ�������Ч��ʱ��
		animation_land_effect.set_atlas(&atlas_land_effect);
		animation_land_effect.set_interval(50);
		animation_land_effect.set_loop(false);
		animation_land_effect.set_callback([&]()
			{
				is_land_effect_visible = false;
			});

		//��ʼ��ָʾ��
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
		//ͨ����������ķ�ʽ���ж���ҵ��ƶ�����
		int direction = is_right_key_down - is_left_key_down;

		if (direction != 0)
		{
			if (!is_attacking_ex)
			{
				is_facing_right = direction > 0;
			}
			//������ҵ��ƶ��������л�����
			current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
			//����һ֡�������ˮƽ�������ƶ��ľ���洢��distance��
			float distance = direction * run_velocity * delta;
			on_run(distance);
		}
		else
		{
			//������û���ƶ�����ô�Ͳ���Ĭ�϶���
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

		//���¶���
		current_animation->on_update(delta);

		timer_attack_cd.on_update(delta);

		//�����޵�ʱ�䶨ʱ��
		timer_invulnerable.on_update(delta);
		//�����޵ж�����˸��ʱ��
		timer_invulnerable_blink.on_update(delta);

		//���±�����Ч����
		timer_run_effect_generation.on_update(delta);

		//������Ծ��Ч����
		animation_jump_effect.on_update(delta);
		//���������Ч����
		animation_land_effect.on_update(delta);

		//����ָʾ��
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

		// �������Ƿ������������������� is_attacking_ex ״̬
		if (current_animation->check_finished() && is_attacking_ex)
		{
			is_attacking_ex = false;
			// ���ö���ΪĬ�϶���
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
			//�Ե�ǰ���ڲ��ŵĶ������л���
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
		timer_run_effect_generation.resume();
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
		//������Ծ��Ч����
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
	//������ص����д���
	void move_and_collide(int delta)
	{
		float last_velocity_y = velocity.y;

		//�����������ٶȵ�ֵ��������ҵ��ٶ�
		velocity.y += gravity * delta;
		//�����ٶ���������ҵ�λ��
		position += velocity * (float)delta;

		if (hp <= 0)
		{
			return;
		}

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

						//ֻ����ǰһ֡y���ٶȲ�����0�����ҵ�ǰ֡y���ٶȵ���0ʱ�Ŵ�������¼�
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

	Animation animation_die_left;			//��ҳ��������������
	Animation animation_die_right;			//��ҳ����ҵ���������

	Animation animation_jump_effect;		//��Ծ��Ч����
	Animation animation_land_effect;		//�����Ч����

	bool is_jump_effect_visible = false;	//��Ծ��Ч�����Ƿ�ɼ�
	bool is_land_effect_visible = false;	//�����Ч�����Ƿ�ɼ�

	Vector2 position_jump_effect;			//��Ծ��Ч������λ��
	Vector2 position_land_effect;			//�����Ч������λ��

	Animation* current_animation = nullptr; //��ǰ���ڲ��ŵĶ���

	PlayerID id = PlayerID::P1;				//���ID

	bool is_left_key_down = false;			//����Ƿ���
	bool is_right_key_down = false;			//�Ҽ��Ƿ���

	bool is_facing_right = true;			//����Ƿ����ұ�

	int attack_cd = 500;					//��ͨ������ȴʱ��
	bool can_attack = true;					//����Ƿ�����ͷ���ͨ����
	Timer timer_attack_cd;					//��ͨ������ȴ��ʱ��

	bool is_attacking_ex = false;			//����Ƿ������ͷ����⹥��

	bool is_invulnerable = false;			//����Ƿ��޵�
	bool is_showing_sketch_frame = false;	//��ǰ֡�Ƿ�Ӧ����ʾ��Ӱ

	Timer timer_invulnerable;				//�޵�״̬��ʱ��
	Timer timer_invulnerable_blink;			//�޵�״̬��˸��ʱ��

	IMAGE img_sketch;						//��Ӱͼ��

	Timer timer_run_effect_generation;		//������Ч���ӷ��䶨ʱ��
	Timer timer_die_effect_generation;		//������Ч���ӷ��䶨ʱ��

	std::vector<Particle> particle_list;	//���Ӷ�������

	bool is_cursor_visible = true;			//ָʾ����Ƿ�ɼ�
	Timer timer_cursor_visibility;			//ָʾ���ɼ��Զ�ʱ��

	Vector2 last_hurt_direcrion;			//���һ���ܻ��ķ���
};

