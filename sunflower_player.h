#pragma once

#include "player.h"
#include "sun_bullet.h"
#include "sun_bullet_ex.h"

#include <iostream>


extern Atlas atlas_sunflower_idle_left;  // 龙日葵朝向左的默认动画图集
extern Atlas atlas_sunflower_idle_right; // 龙日葵朝向右的默认动画图集
extern Atlas atlas_sunflower_run_left;   // 龙日葵朝向左的跑步动画图集
extern Atlas atlas_sunflower_run_right;  // 龙日葵朝向右的跑步动画图集
extern Atlas atlas_sunflower_attack_ex_left; // 龙日葵朝向左的特殊攻击动画图集
extern Atlas atlas_sunflower_attack_ex_right; // 龙日葵朝向右的特殊攻击动画图集
extern Atlas atlas_sun_text; // “日” 文本动画图集

extern Player* player_1;
extern Player* player_2;



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

		animation_attack_ex_left.set_atlas(&atlas_sunflower_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_sunflower_attack_ex_right);

		animation_sun_text.set_atlas(&atlas_sun_text);

		//设置动画对象的帧间隔
		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);

		animation_attack_ex_left.set_interval(100);
		animation_attack_ex_right.set_interval(100);

		animation_sun_text.set_interval(100);

		animation_attack_ex_left.set_loop(false);
		animation_attack_ex_right.set_loop(false);
		animation_sun_text.set_loop(false);

		//左右动画播放结束时的逻辑
		animation_idle_left.set_callback([&]() 
			{
				is_attacking_ex = false; 
				is_sun_text_visible = false;
			});
		animation_idle_right.set_callback([&]()
			{
				is_attacking_ex = false;
				is_sun_text_visible = false;
			});

		animation_sun_text.set_callback([&]() {
				is_attacking_ex = false;
				is_sun_text_visible = false;
			});

		//设置玩家的尺寸
		size.x = 96, size.y = 96;

		//设置普通攻击的冷却时间
		attack_cd = 250;

	}
	~SunflowerPlayer() = default;

	void on_update(int delta)
	{
		Player::on_update(delta);

		//当文本可见时更新文本动画
		if (is_sun_text_visible)
		{
			animation_sun_text.on_update(delta);
		}

		
	}

	void on_draw(const Camera& camera)
	{
		Player::on_draw(camera);
		//当文本可见时绘制文本动画
		/*	if (is_sun_text_visible) 检查is_sun_text_visible标志，如果文本可见。
			Vector2 text_position; 声明一个Vector2对象，用于存储文本动画的渲染位置。
			IMAGE* frame = animation_sun_text.get_frame(); 获取当前帧的图像。
			text_position.x = position.x - (size.x - frame->getwidth()) / 2; 计算文本动画的x坐标，使其居中对齐。
			text_position.y = position.y - frame->getheight(); 计算文本动画的y坐标，使其位于玩家的头顶。
			animation_sun_text.on_draw(camera, (int)text_position.x, (int)text_position.y); 绘制文本动画。
			在调用基类Player的on_draw方法后，检查is_sun_text_visible标志，如果文本可见，则绘制文本动画。
		*/
		if (is_sun_text_visible)
		{
			Vector2 text_position;															//存储文本动画的渲染位置
			IMAGE* frame = animation_sun_text.get_frame();									//获取当前帧
			text_position.x = position.x - (size.x - frame->getwidth()) / 2;				//计算文本动画的x坐标
			text_position.y = position.y - frame->getheight();								//计算文本动画的y坐标
			animation_sun_text.on_draw(camera, (int)text_position.x, (int)text_position.y);	//绘制文本动画
		}
	}

	void on_attack()
	{
		Bullet* bullet = new SunBullet();

		Vector2 bullet_position;
		const Vector2& bullet_size = bullet->get_size();
		bullet_position.x = position.x + (size.x - bullet_size.x) / 2;
		bullet_position.y = position.y;

		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_velocity(is_fating_right ? velocity_sun.x : -velocity_sun.x, velocity_sun.y);

		bullet->set_collide_target(id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);

		bullet->set_callback([&]()
			{
				mp += 20;
			});

		bullet_list.push_back(bullet);
	}

	void on_attack_ex()
	{
		is_attacking_ex = true;
		is_sun_text_visible = true;
		
		/*animation_sun_text.pause();*/
		/*animation_sun_text.restart();*/
		animation_sun_text.reset();
		/*if (is_sun_text_visible)
		{
			is_sun_text_visible = false;
		}*/
		is_fating_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();
		

		Bullet* bullet = new SunBulletEx();
		//根据id判断目标对象是哪一个玩家
		Player* target_player = (id == PlayerID::P1 ? player_2 : player_1);

		Vector2 bullet_position, bullet_velocity;
		const Vector2& bullet_size = bullet->get_size();
		const Vector2& target_size = target_player->get_size();
		const Vector2& target_position = target_player->get_position();
		bullet_position.x = target_position.x + (target_size.x - bullet_size.x) / 2; //计算子弹的x坐标，使其水平居中对齐目标玩家。
		bullet_position.y = - size.y; //设置子弹的y坐标，使其从屏幕顶部开始下落。
		bullet_velocity.x = 0; //设置子弹的水平速度为0。
		bullet_velocity.y = speed_sun_ex; //设置子弹的垂直速度,使其向下移动;

		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_velocity(bullet_velocity.x, bullet_velocity.y);

		
		//设置子弹的碰撞回调函数，当子弹碰撞时，增加玩家的mp值。
		bullet->set_callback([&]()
			{
				mp += 30;
			});

		bullet_list.push_back(bullet);

		//播放特殊攻击音效
		mciSendString(_T("play sunText from 0"), NULL, 0, NULL);

		
	}



private:
	const float speed_sun_ex = 0.15f;	//特殊日光子弹的下落的速度
	const Vector2 velocity_sun = { 0.25f,-0.5f };	//日光子弹的速度

private:
	Animation animation_sun_text;		 //头顶“日”字文本动画
	bool is_sun_text_visible = false;	 //是否显示头顶“日”字文本
};
