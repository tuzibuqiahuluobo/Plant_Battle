#pragma once

#include "bullet.h"
#include "animation.h"


extern IMAGE img_pea; // 豌豆子弹图片
extern Atlas atlas_pea_break;; // 豌豆子弹爆炸动画图集


class PeaBullet : public Bullet
{
public:
	PeaBullet()
	{
		//设置子弹的尺寸
		size.x = 64, size.y = 64;

		//设置子弹的伤害
		damage = 10;

		//子弹破碎的动画配置
		animation_break.set_atlas(&atlas_pea_break);
		animation_break.set_interval(100);
		animation_break.set_loop(false);
		/*	当animation_break动画播放完毕时，会执行回调函数，将can_remove标志设置为true，表示该子弹可以被移除。
			animation_break.set_callback(...)：设置animation_break动画的回调函数。
			[&]() { can_remove = true; }：这是一个捕获外部变量的lambda表达式。
			[&]表示按引用捕获外部变量，
			{ can_remove = true; }是lambda表达式的主体，
			当动画播放完毕时会执行这段代码，将can_remove设置为true。*/
		animation_break.set_callback([&]() { can_remove = true;});
	}

	~PeaBullet() = default;

	void on_collide()
	{
		//在重写父类方法时还需要执行父类逻辑时，需要显式调用父类方法
		Bullet::on_collide();

		switch (rand() % 3)
		{
		case 0:
			mciSendString(_T("play peaBreak1 from 0"), NULL, 0, NULL);
			break;
		case 1:
			mciSendString(_T("play peaBreak2 from 0"), NULL, 0, NULL);
			break;
		case 2:
			mciSendString(_T("play peaBreak3 from 0"), NULL, 0, NULL);
			break;
		}
	}

	void on_update(int delta)
	{
		position.x += velocity.x * (float)delta;

		//如果子弹已经碰撞，那么就不再更新动画
		if (!valid)
		{
			animation_break.on_update(delta);
		}

		//检测子弹是否超出屏幕
		if (check_if_exceeds_screen())
		{
			can_remove = true;
		}
	}

	void on_draw(const Camera& camera) const 
	{
		if (valid)
		{
			putimage_alpha(camera, (int)position.x ,(int)position.y, &img_pea);
		}
		else
		{
			animation_break.on_draw(camera, (int)position.x, (int)position.y);
		}

		Bullet::on_draw(camera);
	}



private:
	Animation animation_break; //豌豆子弹爆炸动画

};

