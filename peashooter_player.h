#pragma once


#include "player.h"

#include <iostream>

//使用extern关键字声明全局变量
extern Atlas atlas_peashooter_idle_left;   // 婉逗射手朝向左的默认动画图集
extern Atlas atlas_peashooter_idle_right;  // 婉逗射手朝向右的默认动画图集
extern Atlas atlas_peashooter_run_left;    // 婉逗射手朝向左的跑步动画图集
extern Atlas atlas_peashooter_run_right;   // 婉逗射手朝向右的跑步动画图集

class PeashooterPlayer : public Player
{
public:
	PeashooterPlayer()
	{
		//初始化玩家的动画
		animation_idle_left.set_atlas(&atlas_peashooter_idle_left);
		animation_idle_right.set_atlas(&atlas_peashooter_idle_right);
		animation_run_left.set_atlas(&atlas_peashooter_run_left);
		animation_run_right.set_atlas(&atlas_peashooter_run_right);
		
		//设置动画对象的帧间隔
		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);
	}
	~PeashooterPlayer() = default;

	void on_update(int delta) override
	{
		Player::on_update(delta);

		std::cout << "PeashooterPlayer::on_update" << std::endl;
	}

private:

};


