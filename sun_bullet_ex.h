#pragma once

#include "bullet.h"
#include "animation.h"

extern Atlas atlas_sun_ex;			// �����չ⶯��ͼ��
extern Atlas atlas_sun_ex_explode;	// �����չⱬը����ͼ��

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

	//ͨ���Ƚ��ӵ���Ŀ�����ı߽���ȷ���Ƿ������ص���
	bool check_conllision(const Vector2& position, const Vector2& size)
	{
		/*	1.	this->position.x + this->size.x���ӵ����ұ߽硣
			2.	position.x + size.x��Ŀ�������ұ߽硣
			3.	max(this->position.x + this->size.x, position.x + size.x)���ӵ���Ŀ������ұ߽��е����ֵ��
			4.	this->position.x���ӵ�����߽硣
			5.	position.x��Ŀ��������߽硣
			6.	min(this->position.x, position.x)���ӵ���Ŀ�������߽��е���Сֵ��
			7.	max(this->position.x + this->size.x, position.x + size.x)
			- min(this->position.x, position.x)���ӵ���Ŀ�������ܿ�ȡ�
			8.	this->size.x���ӵ��Ŀ�ȡ�
			9.	size.x��Ŀ�����Ŀ�ȡ�
			10.	this->size.x + size.x���ӵ���Ŀ�����Ŀ��֮�͡�
			��δ�����߼��ǣ�����ӵ���Ŀ�������ܿ��С�ڵ����ӵ���Ŀ�����Ŀ��֮�ͣ�
			��˵���ӵ���Ŀ�������ˮƽ�����Ϸ������ص�������������ײ��
			ͬ�����߼�Ҳ�����ڴ�ֱ�������ײ��⡣*/
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
	Animation animation_ex_idle;		// ����̫���ӵ�Ĭ�϶���
	Animation animation_ex_explode;		// ����̫���ӵ���ը����
	Vector2 explode_render_offset;		// ��ը������Ⱦƫ��
};

