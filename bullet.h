#pragma once

#include "vector2.h"
#include "player_id.h"
#include "camera.h"

#include <graphics.h>
#include <functional>

extern bool is_debug; // �Ƿ�Ϊ����ģʽ

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	//�����ӵ����˺�
	void set_damage(int val)
	{
		damage = val;
	}

	//��ȡ�ӵ����˺�
	int get_damage() const
	{
		return damage;
	}

	//�����ӵ���λ��
	void set_position(float x,float y)
	{
		position.x = x,position.y = y;
	}

	//��ȡ�ӵ���λ��
	const Vector2& get_position() const
	{
		return position;
	}

	//��ȡ�ӵ��ĳߴ�
	const Vector2& get_size() const
	{
		return size;
	}

	//�����ӵ����ٶ�
	void set_velocity(float x, float y)
	{
		velocity.x = x, velocity.y = y;
	}

	//��ײĿ�������
	void set_collide_target(PlayerID target)
	{
		target_id = target;
	}

	//��ײĿ��Ļ�ȡ
	PlayerID get_collide_target() const
	{
		return target_id;
	}

	//�ӵ���ײ��Ļص������߼��������ӵ������ڲ�
	void set_callback(std::function<void()>callback)
	{
		this->callback = callback;
	}

	//�ӵ��Ƿ���Լ�����ײ
	void set_valid(bool flag)
	{
		valid = flag;
	}

	//�ӵ��Ƿ���Լ�����ײ
	bool get_valid() const
	{
		return valid;
	}

	//�ӵ��Ƿ�����Ƴ�
	bool check_can_remove() const
	{
		return can_remove;
	}

	//�ɱ��ӵ���д���麯������
	virtual void on_collide()
	{
		if (callback)
		{
			callback();
		}
	}

	//����ӵ��Ƿ���Ŀ���������ײ
	/*	1.	this->position.x + this->size.x / 2���ӵ���ˮƽ����λ�á�
		2.	position.x��Ŀ��������߽硣
		3.	position.x + size.x��Ŀ�������ұ߽硣
		4.	this->position.y + this->size.y / 2���ӵ��Ĵ�ֱ����λ�á�
		5.	position.y��Ŀ�������ϱ߽硣
		6.	position.y + size.y��Ŀ�������±߽硣
		��δ�����߼��ǣ�����ӵ�������λ����Ŀ�����ľ��α߽��ڣ���˵���ӵ���Ŀ�����������ײ��
		(��Ϊ�������ײ��״�Ǹ�����)
		*/
	virtual bool check_collision(const Vector2& position, const Vector2& size)
	{
		return this->position.x + this->size.x / 2 >= position.x
			&& this->position.x + this->size.x / 2 <= position.x + size.x
			&& this->position.y + this->size.y / 2 >= position.y 
			&& this->position.y + this->size.y / 2 <= position.y + size.y;
	}

	//�ӵ��ĸ����߼�
	virtual void on_update(int delta)
	{
		
	}

	//�ӵ��Ļ����߼�
	virtual void on_draw(const Camera& camera) const
	{
		if (is_debug)
		{
			setfillcolor(RGB(255, 255, 255));
			setlinecolor(RGB(255, 255, 255));
			rectangle((int)position.x, (int)position.y, 
				(int)(position.x + size.x), (int)(position.y + size.y));
			solidcircle((int)(position.x + size.x / 2), (int)(position.y + size.y / 2), 5);
		}
	}

protected:
	//����ӵ��ľ��α߽��Ƿ�λ����Ļ����֮��
	bool check_if_exceeds_screen()
	{
		return (position.x + size.x <= 0 || position.x >= getwidth()
			 || position.y + size.y <= 0 || position.y >= getheight());
	}

protected:
	Vector2 size;							//�ӵ��ĳߴ�
	Vector2 position;						//�ӵ���λ��
	Vector2 velocity;						//�ӵ����ٶ�
	int damage = 10;						//�ӵ����˺�ֵ
	bool valid = true;						//�ӵ��Ƿ���Ч
	bool can_remove = false;				//�ӵ��Ƿ�����Ƴ�
	std::function<void()> callback;			//�ӵ��Ƴ�ʱ�Ļص�����
	PlayerID target_id = PlayerID::P1;		//�ӵ���Ŀ�����ID

};

