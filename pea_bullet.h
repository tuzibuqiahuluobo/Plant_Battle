#pragma once

#include "bullet.h"
#include "animation.h"


extern IMAGE img_pea; // �㶹�ӵ�ͼƬ
extern Atlas atlas_pea_break;; // �㶹�ӵ���ը����ͼ��


class PeaBullet : public Bullet
{
public:
	PeaBullet()
	{
		//�����ӵ��ĳߴ�
		size.x = 64, size.y = 64;

		//�����ӵ����˺�
		damage = 10;

		//�ӵ�����Ķ�������
		animation_break.set_atlas(&atlas_pea_break);
		animation_break.set_interval(100);
		animation_break.set_loop(false);
		/*	��animation_break�����������ʱ����ִ�лص���������can_remove��־����Ϊtrue����ʾ���ӵ����Ա��Ƴ���
			animation_break.set_callback(...)������animation_break�����Ļص�������
			[&]() { can_remove = true; }������һ�������ⲿ������lambda���ʽ��
			[&]��ʾ�����ò����ⲿ������
			{ can_remove = true; }��lambda���ʽ�����壬
			�������������ʱ��ִ����δ��룬��can_remove����Ϊtrue��*/
		animation_break.set_callback([&]() { can_remove = true;});
	}

	~PeaBullet() = default;

	void on_collide()
	{
		//����д���෽��ʱ����Ҫִ�и����߼�ʱ����Ҫ��ʽ���ø��෽��
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

		//����ӵ��Ѿ���ײ����ô�Ͳ��ٸ��¶���
		if (!valid)
		{
			animation_break.on_update(delta);
		}

		//����ӵ��Ƿ񳬳���Ļ
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
	Animation animation_break; //�㶹�ӵ���ը����

};

