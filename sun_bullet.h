#pragma once

#include "bullet.h"
#include "animation.h"

extern Atlas atlas_sun; // 太阳子弹图片
extern Atlas atlas_sun_explode; // 太阳子弹爆炸动画图集

extern Camera main_camera;

class SunBullet : public Bullet
{
public:
	SunBullet()
	{
		size.x = 96, size.y = 96;

		damage = 20;

		animation_idle.set_atlas(&atlas_sun);
		animation_idle.set_interval(50);

		animation_explode.set_atlas(&atlas_sun_explode);
		animation_explode.set_interval(50);
		animation_explode.set_loop(false);
		animation_explode.set_callback([&]() { can_remove = true; });

		//计算爆炸动画渲染偏移
		IMAGE* frame_idle = animation_idle.get_frame();
		IMAGE* frame_explode = animation_explode.get_frame();
		//对他们宽高分别做差后除以2，得到的就是偏移量
		explode_render_offset.x = (frame_idle->getwidth() - frame_explode->getwidth()) / 2.0f;
		explode_render_offset.y = (frame_idle->getheight() - frame_explode->getheight()) / 2.0f;
	}

	~SunBullet() = default;

	void on_collide()
	{
		Bullet::on_collide();

		main_camera.shake(5, 250);

		mciSendString(_T("play sunExplode from 0"), NULL, 0, NULL);
	}

	void on_update(int delta)
	{
		/*  if (valid)：如果子弹仍然有效（未碰撞）。
			velocity.y += gravity * delta; ：根据重力加速度更新子弹的垂直速度。
			position += velocity * (float)delta; ：根据速度更新子弹的位置。*/
		if (valid)
		{
			velocity.y += gravity * delta;
			position += velocity * (float)delta;
		}

		/*  if (!valid)：如果子弹已经碰撞。
			animation_explode.on_update(delta); ：更新爆炸动画。
			else：如果子弹未碰撞。
			animation_idle.on_update(delta); ：更新默认动画。*/
		if (!valid)
		{
			animation_explode.on_update(delta);
		}
		else
		{
			animation_idle.on_update(delta);
		}

		if (check_if_exceeds_screen())
		{
			can_remove = true;
		}
	}

	void on_draw(const Camera& camera) const
	{
		/*  if (valid)：如果子弹仍然有效（未碰撞）。
			animation_idle.on_draw(camera, (int)position.x, (int)position.y); ：绘制默认动画，使用子弹的当前位置。
			else：如果子弹已经碰撞。
			animation_explode.on_draw(camera,
				(int)(position.x + explode_render_offset.x),
				(int)(position.y + explode_render_offset.y)); ：绘制爆炸动画，并应用渲染偏移。*/
		if (valid)
		{
			animation_idle.on_draw(camera, (int)position.x, (int)position.y);
		}
		else
		{
			animation_explode.on_draw(camera, 
				(int)(position.x + explode_render_offset.x),
				(int)(position.y + explode_render_offset.y));
		}
	}


private:
	const float gravity = 1e-3f;	// 太阳子弹重力加速度


private:
	Animation animation_idle;		// 太阳子弹默认动画
	Animation animation_explode;	// 太阳子弹爆炸动画
	Vector2 explode_render_offset;	// 爆炸动画渲染偏移
};

