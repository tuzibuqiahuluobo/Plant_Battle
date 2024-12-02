#pragma once

#include "bullet.h"
#include "animation.h"

extern Atlas atlas_sun; // ̫���ӵ�ͼƬ
extern Atlas atlas_sun_explode; // ̫���ӵ���ը����ͼ��

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

		//���㱬ը������Ⱦƫ��
		IMAGE* frame_idle = animation_idle.get_frame();
		IMAGE* frame_explode = animation_explode.get_frame();
		//�����ǿ�߷ֱ���������2���õ��ľ���ƫ����
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
		/*  if (valid)������ӵ���Ȼ��Ч��δ��ײ����
			velocity.y += gravity * delta; �������������ٶȸ����ӵ��Ĵ�ֱ�ٶȡ�
			position += velocity * (float)delta; �������ٶȸ����ӵ���λ�á�*/
		if (valid)
		{
			velocity.y += gravity * delta;
			position += velocity * (float)delta;
		}

		/*  if (!valid)������ӵ��Ѿ���ײ��
			animation_explode.on_update(delta); �����±�ը������
			else������ӵ�δ��ײ��
			animation_idle.on_update(delta); ������Ĭ�϶�����*/
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
		/*  if (valid)������ӵ���Ȼ��Ч��δ��ײ����
			animation_idle.on_draw(camera, (int)position.x, (int)position.y); ������Ĭ�϶�����ʹ���ӵ��ĵ�ǰλ�á�
			else������ӵ��Ѿ���ײ��
			animation_explode.on_draw(camera,
				(int)(position.x + explode_render_offset.x),
				(int)(position.y + explode_render_offset.y)); �����Ʊ�ը��������Ӧ����Ⱦƫ�ơ�*/
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
	const float gravity = 1e-3f;	// ̫���ӵ��������ٶ�


private:
	Animation animation_idle;		// ̫���ӵ�Ĭ�϶���
	Animation animation_explode;	// ̫���ӵ���ը����
	Vector2 explode_render_offset;	// ��ը������Ⱦƫ��
};

