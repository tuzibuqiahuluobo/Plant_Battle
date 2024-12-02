#pragma once


#include "player.h"
#include "pea_bullet.h"

#include <iostream>

//ʹ��extern�ؼ�������ȫ�ֱ���
extern Atlas atlas_peashooter_idle_left;   // �����ֳ������Ĭ�϶���ͼ��
extern Atlas atlas_peashooter_idle_right;  // �����ֳ����ҵ�Ĭ�϶���ͼ��
extern Atlas atlas_peashooter_run_left;    // �����ֳ�������ܲ�����ͼ��
extern Atlas atlas_peashooter_run_right;   // �����ֳ����ҵ��ܲ�����ͼ��
extern Atlas atlas_peashooter_attack_ex_left; // �����ֳ���������⹥������ͼ��
extern Atlas atlas_peashooter_attack_ex_right; // �����ֳ����ҵ����⹥������ͼ��
extern Atlas atlas_peashooter_die_left;    // �����ֳ��������������ͼ��
extern Atlas atlas_peashooter_die_right;   // �����ֳ����ҵ���������ͼ��



class PeashooterPlayer : public Player
{
public:
	PeashooterPlayer(bool facing_right = true) : Player(facing_right)
	{
		//��ʼ����ҵĶ���
		animation_idle_left.set_atlas(&atlas_peashooter_idle_left);
		animation_idle_right.set_atlas(&atlas_peashooter_idle_right);
		animation_run_left.set_atlas(&atlas_peashooter_run_left);
		animation_run_right.set_atlas(&atlas_peashooter_run_right);

		animation_attack_ex_left.set_atlas(&atlas_peashooter_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_peashooter_attack_ex_right);

		animation_die_left.set_atlas(&atlas_peashooter_die_left);
		animation_die_right.set_atlas(&atlas_peashooter_die_right);
		
		//���ö��������֡���
		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);

		animation_attack_ex_left.set_interval(75);
		animation_attack_ex_right.set_interval(75);

		animation_die_left.set_interval(150);
		animation_die_right.set_interval(150);


		//���ö��������ѭ����������
		animation_die_left.set_loop(false);
		animation_die_right.set_loop(false);
		 
		
		//������ҵĳߴ�
		size.x = 96, size.y = 96;

		/*  1.	timer_attack_ex.set_wait_time(attack_ex_duration);
		����timer_attack_ex��ʱ���ĵȴ�ʱ��Ϊattack_ex_duration�������⹥���ĳ���ʱ�䡣
			2.	timer_attack_ex.set_one_shot(true); 
		����timer_attack_ex��ʱ��Ϊһ���Զ�ʱ������ֻ����һ�Ρ�
			3.	timer_attack_ex.set_callback([&]() { is_attacking_ex = false; }); 
		����timer_attack_ex��ʱ���Ļص�����������ʱ������ʱ����is_attacking_ex��־����Ϊfalse����ʾ���⹥��������*/
		timer_attack_ex.set_wait_time(attack_ex_duration);
		timer_attack_ex.set_one_shot(true);
		timer_attack_ex.set_callback([&]() 
			{ 
				is_attacking_ex = false;
			});

		/*
		4.	timer_spwan_pea_ex.set_wait_time(100); 
		����timer_spwan_pea_ex��ʱ���ĵȴ�ʱ��Ϊ100���롣
		5.	timer_spwan_pea_ex.set_callback([&]() { spawn_pea_bullet(speed_pea_ex); }); 
		����timer_spwan_pea_ex��ʱ���Ļص�����������ʱ������ʱ��
			����spawn_pea_bullet��������һ���ٶ�Ϊspeed_pea_ex���㶹�ӵ���*/
		timer_spwan_pea_ex.set_wait_time(100);
		timer_spwan_pea_ex.set_callback([&]()
			{ 
				spawn_pea_bullet(speed_pea_ex);
			});

		attack_cd = 100; //��ͨ������ȴʱ��100����
	}
	~PeashooterPlayer() = default;

	void on_update(int delta) override
	{
		Player::on_update(delta);

		if (is_attacking_ex)
		{
			main_camera.shake(5, 100);
			timer_attack_ex.on_update(delta);
			timer_spwan_pea_ex.on_update(delta);
		}
	}

	void on_attack() 
	{
		spawn_pea_bullet(speed_pea);

		switch (rand() % 2)
		{
		case 0:
			mciSendString(_T("play peaShoot1 from 0"), NULL, 0, NULL);
			break;		
		case 1:
			mciSendString(_T("play peaShoot2 from 0"), NULL, 0, NULL);
			break;
		}
	}

	void on_attack_ex()
	{
		
		is_attacking_ex = true;
		timer_attack_ex.restart();
		
		//****
		is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();
		mciSendString(_T("play peaShootEx from 0"), NULL, 0, NULL);
	}



private:
	const float speed_pea = 0.75f;			//�㶹�ӵ����ٶ�
	const float speed_pea_ex = 1.5f;		//�����㶹�ӵ����ٶ�
	const int attack_ex_duration = 2500;	//���⹥������ʱ��

private:
	void spawn_pea_bullet(float speed)
	{
		/*1.	����һ���µ�PeaBullet�ӵ�����*/
		Bullet* bullet = new PeaBullet(); 
		/*2.	��ȡ�ӵ��ĳߴ硣*/
		Vector2 bullet_position, bullet_velocity;
		const Vector2& bullet_size = bullet->get_size();
		//������ҵĳ����������ӵ���λ�ú��ٶ�
		/*  �����ҳ����ұߣ��ӵ���λ������ҵ��ұߡ�*/
		bullet_position.x = is_facing_right
			? position.x + size.x - bullet_size.x / 2
			: position.x - bullet_size.x / 2;
		/*	�����ҳ�����ߣ��ӵ���λ������ҵ���ߡ�*/
		bullet_position.y = position.y;
		//	�ӵ���ˮƽ�ٶȸ�����ҵĳ������ã���ֱ�ٶ�Ϊ0��
		bullet_velocity.x = is_facing_right ? speed : -speed;
		bullet_velocity.y = 0;
		//4.	�����ӵ���λ�ú��ٶȡ�
		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_velocity(bullet_velocity.x, bullet_velocity.y);

		//�����ӵ�����ײĿ��Ϊ�Է����
		bullet->set_collide_target(id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);

		//6.	�����ӵ�����ײ�ص����������ӵ���ײʱ��������ҵ�mpֵ��
		bullet->set_callback([&]()
			{
				mp += 25;
			});

		//7.	���ӵ�������ӵ��ӵ��б��С�
		bullet_list.push_back(bullet);
	}

	Timer timer_attack_ex;		 //���⹥��״̬��ʱ��
	Timer timer_spwan_pea_ex;	 //�㶹�ӵ����䶨ʱ��

	bool is_facing_right = true; //����Ƿ����ұ�
};


