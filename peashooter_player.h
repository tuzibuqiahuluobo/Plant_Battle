#pragma once


#include "player.h"
#include "pea_bullet.h"

#include <iostream>

//使用extern关键字声明全局变量
extern Atlas atlas_peashooter_idle_left;   // 婉逗射手朝向左的默认动画图集
extern Atlas atlas_peashooter_idle_right;  // 婉逗射手朝向右的默认动画图集
extern Atlas atlas_peashooter_run_left;    // 婉逗射手朝向左的跑步动画图集
extern Atlas atlas_peashooter_run_right;   // 婉逗射手朝向右的跑步动画图集
extern Atlas atlas_peashooter_attack_ex_left; // 婉逗射手朝向左的特殊攻击动画图集
extern Atlas atlas_peashooter_attack_ex_right; // 婉逗射手朝向右的特殊攻击动画图集
extern Atlas atlas_peashooter_die_left;    // 婉逗射手朝向左的死亡动画图集
extern Atlas atlas_peashooter_die_right;   // 婉逗射手朝向右的死亡动画图集



class PeashooterPlayer : public Player
{
public:
	PeashooterPlayer(bool facing_right = true) : Player(facing_right)
	{
		//初始化玩家的动画
		animation_idle_left.set_atlas(&atlas_peashooter_idle_left);
		animation_idle_right.set_atlas(&atlas_peashooter_idle_right);
		animation_run_left.set_atlas(&atlas_peashooter_run_left);
		animation_run_right.set_atlas(&atlas_peashooter_run_right);

		animation_attack_ex_left.set_atlas(&atlas_peashooter_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_peashooter_attack_ex_right);

		animation_die_left.set_atlas(&atlas_peashooter_die_left);
		animation_die_right.set_atlas(&atlas_peashooter_die_right);
		
		//设置动画对象的帧间隔
		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);

		animation_attack_ex_left.set_interval(75);
		animation_attack_ex_right.set_interval(75);

		animation_die_left.set_interval(150);
		animation_die_right.set_interval(150);


		//设置动画对象的循环播放属性
		animation_die_left.set_loop(false);
		animation_die_right.set_loop(false);
		 
		
		//设置玩家的尺寸
		size.x = 96, size.y = 96;

		/*  1.	timer_attack_ex.set_wait_time(attack_ex_duration);
		设置timer_attack_ex定时器的等待时间为attack_ex_duration，即特殊攻击的持续时间。
			2.	timer_attack_ex.set_one_shot(true); 
		设置timer_attack_ex定时器为一次性定时器，即只触发一次。
			3.	timer_attack_ex.set_callback([&]() { is_attacking_ex = false; }); 
		设置timer_attack_ex定时器的回调函数，当定时器触发时，将is_attacking_ex标志设置为false，表示特殊攻击结束。*/
		timer_attack_ex.set_wait_time(attack_ex_duration);
		timer_attack_ex.set_one_shot(true);
		timer_attack_ex.set_callback([&]() 
			{ 
				is_attacking_ex = false;
			});

		/*
		4.	timer_spwan_pea_ex.set_wait_time(100); 
		设置timer_spwan_pea_ex定时器的等待时间为100毫秒。
		5.	timer_spwan_pea_ex.set_callback([&]() { spawn_pea_bullet(speed_pea_ex); }); 
		设置timer_spwan_pea_ex定时器的回调函数，当定时器触发时，
			调用spawn_pea_bullet函数生成一个速度为speed_pea_ex的豌豆子弹。*/
		timer_spwan_pea_ex.set_wait_time(100);
		timer_spwan_pea_ex.set_callback([&]()
			{ 
				spawn_pea_bullet(speed_pea_ex);
			});

		attack_cd = 100; //普通攻击冷却时间100毫秒
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
	const float speed_pea = 0.75f;			//豌豆子弹的速度
	const float speed_pea_ex = 1.5f;		//特殊豌豆子弹的速度
	const int attack_ex_duration = 2500;	//特殊攻击持续时间

private:
	void spawn_pea_bullet(float speed)
	{
		/*1.	创建一个新的PeaBullet子弹对象。*/
		Bullet* bullet = new PeaBullet(); 
		/*2.	获取子弹的尺寸。*/
		Vector2 bullet_position, bullet_velocity;
		const Vector2& bullet_size = bullet->get_size();
		//根据玩家的朝向来设置子弹的位置和速度
		/*  如果玩家朝向右边，子弹的位置在玩家的右边。*/
		bullet_position.x = is_facing_right
			? position.x + size.x - bullet_size.x / 2
			: position.x - bullet_size.x / 2;
		/*	如果玩家朝向左边，子弹的位置在玩家的左边。*/
		bullet_position.y = position.y;
		//	子弹的水平速度根据玩家的朝向设置，垂直速度为0。
		bullet_velocity.x = is_facing_right ? speed : -speed;
		bullet_velocity.y = 0;
		//4.	设置子弹的位置和速度。
		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_velocity(bullet_velocity.x, bullet_velocity.y);

		//设置子弹的碰撞目标为对方玩家
		bullet->set_collide_target(id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);

		//6.	设置子弹的碰撞回调函数，当子弹碰撞时，增加玩家的mp值。
		bullet->set_callback([&]()
			{
				mp += 25;
			});

		//7.	将子弹对象添加到子弹列表中。
		bullet_list.push_back(bullet);
	}

	Timer timer_attack_ex;		 //特殊攻击状态定时器
	Timer timer_spwan_pea_ex;	 //豌豆子弹发射定时器

	bool is_facing_right = true; //玩家是否朝向右边
};


