#pragma once

#include "timer.h"
#include "vector2.h"

class Camera
{
public:
	Camera()
	{
		//终止摄像机抖动
		timer_shake.set_one_shot(true);
		timer_shake.set_callback([&]()
			{
				is_shaking = false;
				reset();
			}
		);
	}
	~Camera() = default;

	//返回摄像机的常量引用
	const Vector2& get_position() const
	{
		return position;
	}

	//将摄像机的位置归零
	void reset()
	{
		position.x = 0;
		position.y = 0;
	}

	void on_update(int delta)
	{
		////给摄像机一个水平向左的速度
		//const Vector2 speed = { -0.35f, 0 };
		////让摄像机的位置随着时间逐渐向左移动
		//position += speed * (float)delta;
		timer_shake.on_update(delta);

		//根据目前摄像机是否正在抖动
		if (is_shaking)
		{
			//将摄像机的位置在抖动强度为半径的圆内进行随机偏移
			//在抖动强度前乘以的这个随机系数是描述了一个单位圆的范围,取值范围为[-1.0,1.0]的浮点数
			position.x = (-50 + rand() % 100) / 50.0f * shaking_strength;
			position.y = (-50 + rand() % 100) / 50.0f * shaking_strength;
		}

	}

	//设置摄像机开始抖动的属性
	//strength:抖动的强度,duration:抖动的持续时间
	void shake(float strength, int duration)
	{
		is_shaking = true;
		shaking_strength = strength; //设置抖动强度

		//设置结束抖动状态的定时器
		timer_shake.set_wait_time(duration); //设置抖动持续时间
		timer_shake.resart(); //重新开始计时
	}

private:
	Vector2 position; //摄像机的位置
	Timer timer_shake; //摄像机抖动的计时器
	bool is_shaking = false; //是否正在抖动
	float shaking_strength = 0; //抖动强度
};
