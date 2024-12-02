#pragma once

#include "Util.h"


class StatusBar
{
public:
	StatusBar() = default;
	~StatusBar() = default;

	void set_avatar(IMAGE* img)
	{
		img_avatar = img;
	}

	void set_position(int x, int y)
	{
		position.x = x, position.y = y;
	}

	void set_hp(int val)
	{
		hp = val;
	}

	void set_mp(int val)
	{
		mp = val;
	}

	void on_draw()
	{
		putimage_alpha(position.x, position.y, img_avatar);

		setfillcolor(RGB(5, 5, 5));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + width + 3 * 2, position.y + 36, 8, 8);
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + width + 3 * 2, position.y + 71, 8, 8);
		setfillcolor(RGB(67, 47 ,47));
		solidroundrect(position.x + 100 + 3, position.y + 10, position.x + 100 + 3 + width, position.y + 33, 8, 8);
		solidroundrect(position.x + 100 + 3, position.y + 45, position.x + 100 + 3 + width, position.y + 68, 8, 8);

		//根据当前的生命值和魔法值绘制状态栏
		float hp_bar_width = width * max(0, hp) / 100.0f;
		float mp_bar_width = width * min(100, mp) / 100.0f;
		//使用不同颜色绘制生命值和魔法值
		setfillcolor(RGB(197, 61, 67));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + 3 + (int)hp_bar_width, position.y + 33, 8, 8);
		setfillcolor(RGB(83, 131, 195));
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + 3 + (int)mp_bar_width, position.y + 68, 8, 8);
	}


private:
	int hp = 0;						//需要显示的生命值
	int mp = 0;						//需要显示的魔法值
	POINT position = { 0 };			//状态栏的位置
	IMAGE* img_avatar = nullptr;	//角色头像的图片

	const int width = 275;			//状态栏的宽度
};

