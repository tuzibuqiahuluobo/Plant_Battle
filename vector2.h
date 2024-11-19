#pragma once
//#include <corecrt_math.h>
#include <cmath>
//在跨平台开发中，建议使用 cmath，因为它是C++标准库的一部分，具有更好的可移植性。
// 如果你只在Windows平台上开发，并且需要使用微软特定的功能或优化，
// 可以使用 corecrt_math.h。

class Vector2
{
public:
	float x = 0;
	float y = 0;

	Vector2() = default;
	~Vector2() = default;
	//x(x)：将成员变量 x 初始化为构造函数参数 x 的值。
	//y(y)：将成员变量 y 初始化为构造函数参数 y 的值。
	//{}构造函数体
	Vector2(float x, float y)
		: x(x),y(y){}
	
	Vector2 operator+(const Vector2& vec)const 
	{
		return Vector2(x + vec.x, y + vec.y);
	}

	void operator+=(const Vector2& vec)
	{
		x += vec.x;
		y += vec.y;
	}

	void operator-=(const Vector2& vec)
	{
		x -= vec.x;
		y -= vec.y;
	}

	Vector2 operator-(const Vector2& vec)const
	{
		return Vector2(x - vec.x, y - vec.y);
	}

	float operator*(const Vector2& vec)const
	{
		return x * vec.x + y * vec.y;
	}

	Vector2 operator*(float val)const
	{
		return Vector2(x * val, y * val);
	}

	void operator*=(float val)
	{
		x *= val;
		y *= val;
	}

	//获取长度的length方法
	float length()
	{
		return sqrt(x * x + y * y);
	}

	//将向量标准化的normalize方法
	Vector2 normalize()
	{
		
		float len = length();

		if (len == 0)
		{
			return Vector2(0, 0);
		}
		return Vector2(x / len, y / len);
	}


private:

};

