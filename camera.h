#pragma once

#include "timer.h"
#include "vector2.h"

class Camera
{
public:
	Camera()
	{
		//��ֹ���������
		timer_shake.set_one_shot(true);
		timer_shake.set_callback([&]()
			{
				is_shaking = false;
				reset();
			}
		);
	}
	~Camera() = default;

	//����������ĳ�������
	const Vector2& get_position() const
	{
		return position;
	}

	//���������λ�ù���
	void reset()
	{
		position.x = 0;
		position.y = 0;
	}

	void on_update(int delta)
	{
		////�������һ��ˮƽ������ٶ�
		//const Vector2 speed = { -0.35f, 0 };
		////���������λ������ʱ���������ƶ�
		//position += speed * (float)delta;
		timer_shake.on_update(delta);

		//����Ŀǰ������Ƿ����ڶ���
		if (is_shaking)
		{
			//���������λ���ڶ���ǿ��Ϊ�뾶��Բ�ڽ������ƫ��
			//�ڶ���ǿ��ǰ���Ե�������ϵ����������һ����λԲ�ķ�Χ,ȡֵ��ΧΪ[-1.0,1.0]�ĸ�����
			position.x = (-50 + rand() % 100) / 50.0f * shaking_strength;
			position.y = (-50 + rand() % 100) / 50.0f * shaking_strength;
		}

	}

	//�����������ʼ����������
	//strength:������ǿ��,duration:�����ĳ���ʱ��
	void shake(float strength, int duration)
	{
		is_shaking = true;
		shaking_strength = strength; //���ö���ǿ��

		//���ý�������״̬�Ķ�ʱ��
		timer_shake.set_wait_time(duration); //���ö�������ʱ��
		timer_shake.resart(); //���¿�ʼ��ʱ
	}

private:
	Vector2 position; //�������λ��
	Timer timer_shake; //����������ļ�ʱ��
	bool is_shaking = false; //�Ƿ����ڶ���
	float shaking_strength = 0; //����ǿ��
};
