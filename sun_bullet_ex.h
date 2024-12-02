#pragma once

#include "bullet.h"
#include "animation.h"

extern Atlas atlas_sun_ex;			// 特殊日光动画图集
extern Atlas atlas_sun_ex_explode;	// 特殊日光爆炸动画图集

extern Camera main_camera;

class SunBulletEx : public Bullet
{
public:
	SunBulletEx()
	{
		size.x = 288, size.y = 288;

		damage = 20;

		animation_ex_idle.set_atlas(&atlas_sun_ex);
		animation_ex_idle.set_interval(50);

		animation_ex_explode.set_atlas(&atlas_sun_ex_explode);
		animation_ex_explode.set_interval(50);
		animation_ex_explode.set_loop(false);
		animation_ex_explode.set_callback([&]() { can_remove = true; });

		IMAGE* frame_idle = animation_ex_idle.get_frame();
		IMAGE* frame_explode = animation_ex_explode.get_frame();
		explode_render_offset.x = (frame_idle->getwidth() - frame_explode->getwidth()) / 2.0f;
		explode_render_offset.y = (frame_idle->getheight() - frame_explode->getheight()) / 2.0f;
	}
	~SunBulletEx() = default;

	void on_collide()
	{
		Bullet::on_collide();
		main_camera.shake(20, 350);
		mciSendString(_T("play sunExplodeEx from 0"), NULL, 0, NULL);
	}

	//通过比较子弹和目标对象的边界来确定是否发生了重叠。
	bool check_conllision(const Vector2& position, const Vector2& size)
	{
		/*	1.	this->position.x + this->size.x：子弹的右边界。
			2.	position.x + size.x：目标对象的右边界。
			3.	max(this->position.x + this->size.x, position.x + size.x)：子弹和目标对象右边界中的最大值。
			4.	this->position.x：子弹的左边界。
			5.	position.x：目标对象的左边界。
			6.	min(this->position.x, position.x)：子弹和目标对象左边界中的最小值。
			7.	max(this->position.x + this->size.x, position.x + size.x)
			- min(this->position.x, position.x)：子弹和目标对象的总宽度。
			8.	this->size.x：子弹的宽度。
			9.	size.x：目标对象的宽度。
			10.	this->size.x + size.x：子弹和目标对象的宽度之和。
			这段代码的逻辑是：如果子弹和目标对象的总宽度小于等于子弹和目标对象的宽度之和，
			则说明子弹和目标对象在水平方向上发生了重叠，即发生了碰撞。
			同样的逻辑也适用于垂直方向的碰撞检测。*/
		bool is_collide_x = (max(this->position.x + this->size.x,position.x + size.x)
			- min(this->position.x, position.x) <= this->size.x + size.x);
		bool is_collide_y = (max(this->position.y + this->size.y,position.y + size.y)
			- min(this->position.y, position.y) <= this->size.y + size.y);

		return is_collide_x && is_collide_y;
	}

	void on_update(int delta)
	{
		if (valid)
		{
			position += velocity * (float)delta;
		}
		if (!valid)
		{
			animation_ex_explode.on_update(delta);
		}
		else
		{
			animation_ex_idle.on_update(delta);
		}
		if (check_if_exceeds_screen())
		{
			can_remove = true;
		}
	}

	void on_draw(const Camera& camera) const
	{
		if (valid)
		{
			animation_ex_idle.on_draw(camera, (int)position.x, (int)position.y);
		}
		else
		{
			animation_ex_explode.on_draw(camera, 
				(int)(position.x + explode_render_offset.x),
				(int)(position.y + explode_render_offset.y));
		}
	}


private:
	Animation animation_ex_idle;		// 特殊太阳子弹默认动画
	Animation animation_ex_explode;		// 特殊太阳子弹爆炸动画
	Vector2 explode_render_offset;		// 爆炸动画渲染偏移
};

