#pragma once

#include "player.h"
#include "sun_bullet.h"
#include "sun_bullet_ex.h"

#include <iostream>


extern Atlas atlas_sunflower_idle_left;  // ���տ��������Ĭ�϶���ͼ��
extern Atlas atlas_sunflower_idle_right; // ���տ������ҵ�Ĭ�϶���ͼ��
extern Atlas atlas_sunflower_run_left;   // ���տ���������ܲ�����ͼ��
extern Atlas atlas_sunflower_run_right;  // ���տ������ҵ��ܲ�����ͼ��
extern Atlas atlas_sunflower_attack_ex_left; // ���տ�����������⹥������ͼ��
extern Atlas atlas_sunflower_attack_ex_right; // ���տ������ҵ����⹥������ͼ��
extern Atlas atlas_sun_text; // ���ա� �ı�����ͼ��

extern Player* player_1;
extern Player* player_2;



class SunflowerPlayer : public Player
{
public:
	SunflowerPlayer()
	{
		//��ʼ����ҵĶ���
		animation_idle_left.set_atlas(&atlas_sunflower_idle_left);
		animation_idle_right.set_atlas(&atlas_sunflower_idle_right);
		animation_run_left.set_atlas(&atlas_sunflower_run_left);
		animation_run_right.set_atlas(&atlas_sunflower_run_right);

		animation_attack_ex_left.set_atlas(&atlas_sunflower_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_sunflower_attack_ex_right);

		animation_sun_text.set_atlas(&atlas_sun_text);

		//���ö��������֡���
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

		//���Ҷ������Ž���ʱ���߼�
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

		//������ҵĳߴ�
		size.x = 96, size.y = 96;

		//������ͨ��������ȴʱ��
		attack_cd = 250;

	}
	~SunflowerPlayer() = default;

	void on_update(int delta)
	{
		Player::on_update(delta);

		//���ı��ɼ�ʱ�����ı�����
		if (is_sun_text_visible)
		{
			animation_sun_text.on_update(delta);
		}

		
	}

	void on_draw(const Camera& camera)
	{
		Player::on_draw(camera);
		//���ı��ɼ�ʱ�����ı�����
		/*	if (is_sun_text_visible) ���is_sun_text_visible��־������ı��ɼ���
			Vector2 text_position; ����һ��Vector2�������ڴ洢�ı���������Ⱦλ�á�
			IMAGE* frame = animation_sun_text.get_frame(); ��ȡ��ǰ֡��ͼ��
			text_position.x = position.x - (size.x - frame->getwidth()) / 2; �����ı�������x���꣬ʹ����ж��롣
			text_position.y = position.y - frame->getheight(); �����ı�������y���꣬ʹ��λ����ҵ�ͷ����
			animation_sun_text.on_draw(camera, (int)text_position.x, (int)text_position.y); �����ı�������
			�ڵ��û���Player��on_draw�����󣬼��is_sun_text_visible��־������ı��ɼ���������ı�������
		*/
		if (is_sun_text_visible)
		{
			Vector2 text_position;															//�洢�ı���������Ⱦλ��
			IMAGE* frame = animation_sun_text.get_frame();									//��ȡ��ǰ֡
			text_position.x = position.x - (size.x - frame->getwidth()) / 2;				//�����ı�������x����
			text_position.y = position.y - frame->getheight();								//�����ı�������y����
			animation_sun_text.on_draw(camera, (int)text_position.x, (int)text_position.y);	//�����ı�����
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
		//����id�ж�Ŀ���������һ�����
		Player* target_player = (id == PlayerID::P1 ? player_2 : player_1);

		Vector2 bullet_position, bullet_velocity;
		const Vector2& bullet_size = bullet->get_size();
		const Vector2& target_size = target_player->get_size();
		const Vector2& target_position = target_player->get_position();
		bullet_position.x = target_position.x + (target_size.x - bullet_size.x) / 2; //�����ӵ���x���꣬ʹ��ˮƽ���ж���Ŀ����ҡ�
		bullet_position.y = - size.y; //�����ӵ���y���꣬ʹ�����Ļ������ʼ���䡣
		bullet_velocity.x = 0; //�����ӵ���ˮƽ�ٶ�Ϊ0��
		bullet_velocity.y = speed_sun_ex; //�����ӵ��Ĵ�ֱ�ٶ�,ʹ�������ƶ�;

		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_velocity(bullet_velocity.x, bullet_velocity.y);

		
		//�����ӵ�����ײ�ص����������ӵ���ײʱ��������ҵ�mpֵ��
		bullet->set_callback([&]()
			{
				mp += 30;
			});

		bullet_list.push_back(bullet);

		//�������⹥����Ч
		mciSendString(_T("play sunText from 0"), NULL, 0, NULL);

		
	}



private:
	const float speed_sun_ex = 0.15f;	//�����չ��ӵ���������ٶ�
	const Vector2 velocity_sun = { 0.25f,-0.5f };	//�չ��ӵ����ٶ�

private:
	Animation animation_sun_text;		 //ͷ�����ա����ı�����
	bool is_sun_text_visible = false;	 //�Ƿ���ʾͷ�����ա����ı�
};
