#pragma once
//工具函数
#include<graphics.h>

#pragma comment(lib, "MSIMG32.LIB")

inline void flip_image(IMAGE* src, IMAGE* dst)//翻转前的源图片和翻转后的目标图片
{
	int w = src->getwidth();
	int h = src->getheight();
	Resize(dst, w, h);
	DWORD* src_buffer = GetImageBuffer(src);
	DWORD* dst_buffer = GetImageBuffer(dst);
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int idx_src = y * w + x;
			int idx_dst = y * w + (w  - x - 1);
			dst_buffer[idx_dst] = src_buffer[idx_src];
		}
	}
}

inline void putimage_alpha(int dst_x, int dst_y, IMAGE* img)//让图像能在屏幕上显示时带有透明效果
{

	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), dst_x, dst_y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}