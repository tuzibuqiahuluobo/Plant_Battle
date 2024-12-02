#pragma once

#include "player.h"

#include <iostream>


extern Atlas atlas_sunflower_idle_left;  // 龙日葵朝向左的默认动画图集
extern Atlas atlas_sunflower_idle_right; // 龙日葵朝向右的默认动画图集
extern Atlas atlas_sunflower_run_left;   // 龙日葵朝向左的跑步动画图集
extern Atlas atlas_sunflower_run_right;  // 龙日葵朝向右的跑步动画图集

class SunflowerPlayer : public Player
{
public:
	SunflowerPlayer()
	{
		//初始化玩家的动画
		animation_idle_left.set_atlas(&atlas_sunflower_idle_left);
		animation_idle_right.set_atlas(&atlas_sunflower_idle_right);
		animation_run_left.set_atlas(&atlas_sunflower_run_left);
		animation_run_right.set_atlas(&atlas_sunflower_run_right);

		//设置动画对象的帧间隔
		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);

	}
	~SunflowerPlayer() = default;

	void on_update(int delta)
	{
		Player::on_update(delta);

		std::cout << "SunflowerPlayer::on_update" << std::endl;
	}

private:

};
