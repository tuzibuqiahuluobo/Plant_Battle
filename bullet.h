#pragma once

#include "vector2.h"
#include "player_id.h"
#include "camera.h"

#include <graphics.h>
#include <functional>

extern bool is_debug; // 是否为调试模式

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	//设置子弹的伤害
	void set_damage(int val)
	{
		damage = val;
	}

	//获取子弹的伤害
	int get_damage() const
	{
		return damage;
	}

	//设置子弹的位置
	void set_position(float x,float y)
	{
		position.x = x,position.y = y;
	}

	//获取子弹的位置
	const Vector2& get_position() const
	{
		return position;
	}

	//获取子弹的尺寸
	const Vector2& get_size() const
	{
		return size;
	}

	//设置子弹的速度
	void set_velocity(float x, float y)
	{
		velocity.x = x, velocity.y = y;
	}

	//碰撞目标的设置
	void set_collide_target(PlayerID target)
	{
		target_id = target;
	}

	//碰撞目标的获取
	PlayerID get_collide_target() const
	{
		return target_id;
	}

	//子弹碰撞后的回调函数逻辑保存在子弹对象内部
	void set_callback(std::function<void()>callback)
	{
		this->callback = callback;
	}

	//子弹是否可以继续碰撞
	void set_valid(bool flag)
	{
		valid = flag;
	}

	//子弹是否可以继续碰撞
	bool get_valid() const
	{
		return valid;
	}

	//子弹是否可以移除
	bool check_can_remove() const
	{
		return can_remove;
	}

	//可被子弹重写的虚函数方法
	virtual void on_collide()
	{
		if (callback)
		{
			callback();
		}
	}

	//检测子弹是否与目标对象发生碰撞
	/*	1.	this->position.x + this->size.x / 2：子弹的水平中心位置。
		2.	position.x：目标对象的左边界。
		3.	position.x + size.x：目标对象的右边界。
		4.	this->position.y + this->size.y / 2：子弹的垂直中心位置。
		5.	position.y：目标对象的上边界。
		6.	position.y + size.y：目标对象的下边界。
		这段代码的逻辑是：如果子弹的中心位置在目标对象的矩形边界内，则说明子弹与目标对象发生了碰撞。
		(因为对象的碰撞形状是个矩形)
		*/
	virtual bool check_collision(const Vector2& position, const Vector2& size)
	{
		return this->position.x + this->size.x / 2 >= position.x
			&& this->position.x + this->size.x / 2 <= position.x + size.x
			&& this->position.y + this->size.y / 2 >= position.y 
			&& this->position.y + this->size.y / 2 <= position.y + size.y;
	}

	//子弹的更新逻辑
	virtual void on_update(int delta)
	{
		
	}

	//子弹的绘制逻辑
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
	//检测子弹的矩形边界是否位于屏幕矩形之外
	bool check_if_exceeds_screen()
	{
		return (position.x + size.x <= 0 || position.x >= getwidth()
			 || position.y + size.y <= 0 || position.y >= getheight());
	}

protected:
	Vector2 size;							//子弹的尺寸
	Vector2 position;						//子弹的位置
	Vector2 velocity;						//子弹的速度
	int damage = 10;						//子弹的伤害值
	bool valid = true;						//子弹是否有效
	bool can_remove = false;				//子弹是否可以移除
	std::function<void()> callback;			//子弹移除时的回调函数
	PlayerID target_id = PlayerID::P1;		//子弹的目标玩家ID

};

