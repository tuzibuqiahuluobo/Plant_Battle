#pragma once

#include "Util.h"
#include "camera.h"

extern bool is_debug; // 是否为调试模式

class Platform
{
public:
	//描述平台的碰撞形状
	struct CollisionShape
	{
		float y;	//相对于平台的y坐标
		float left;	//相对于平台的左边界
		float right;	//相对于平台的右边界
	};

public:
	CollisionShape shape;	//碰撞形状
	IMAGE* img = nullptr;	//平台图片
	POINT render_position = { 0 };	//渲染位置

public:
	Platform() = default;
	~Platform() = default;

	void on_draw(const Camera& camera) const
	{
		putimage_alpha(camera, render_position.x, render_position.y, img);

		if (is_debug)
		{
			setlinecolor(RGB(255, 0, 0));
			line(camera, (int)shape.left, (int)shape.y, (int)shape.right, (int)shape.y);
		}
	}



private:

};

