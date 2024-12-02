#pragma once

#include "atlas.h"
#include "Util.h"
#include "camera.h"
#include <functional>
#include <graphics.h>

class Animation
{
private:
	int timer = 0;			//计时器
	int interval = 0;		//帧间隔
	int idx_frame = 0;		//当前帧索引
	bool is_loop = true;	//是否循环播放

	Atlas* atlas = nullptr;
	std::function<void()> callback;
	//function是一个模板类,<>里面是这个函数对象存储的函数返回值为void,参数为空的函数指针

public:
	Animation() = default;
	~Animation() = default;

	void reset()//重置动画的状态
	{//在动画需要从头播放时使用
		//重置计时器、帧索引
		timer = 0;
		idx_frame = 0;

	}

	void set_atlas(Atlas* new_atlas)//用来设置动画的图集
	{
		//设置新的图集时，重置动画状态
		reset();
		atlas = new_atlas;
	}

	void set_loop(bool flag)//设置是否循环播放
	{
		is_loop = flag;
	}
	

	void set_interval(int ms)//设置帧间隔
	{
		interval = ms;
	}

	int get_idx_frame()//获取当前帧索引
	{
		return idx_frame;
	}

	IMAGE* get_frame()//获取当前帧的图像
	{
		return atlas->get_image(idx_frame);
	}

	bool check_finished()//检查动画是否播放完毕
	{
		if (is_loop)
		{
			return false;
		}
		//如果当前帧索引等于图集大小减一，返回true
		return (idx_frame == atlas->get_size() - 1);
	}

	void on_update(int delta)//更新动画状态
	{
		timer += delta;//计时器累加
		if (timer >= interval)//如果计时器超过帧间隔
		{
			timer = 0;//计时器清零
			idx_frame++;//帧索引递增
			if (idx_frame >= atlas->get_size())//如果当前帧索引超出图集大小
			{
				//根据是否循环播放设置当前帧索引是从头开始还是保持到最后一帧
				idx_frame = is_loop ? 0 : atlas->get_size() - 1;
				if (!is_loop && callback)
				{
					//如果帧索引到达了图集的尾部，且动画没设置循环播放且回调函数存在
					callback();//则执行回调函数
				}
			}
		}
	}

	void on_draw(const Camera& camera , int x, int y) const //绘制当前帧
	{
		//调用工具函数绘制带有透明效果的图像
		putimage_alpha(camera,x, y, atlas->get_image(idx_frame));
	}

	void set_callback(std::function<void()> callback)//设置动画播放完毕后的回调函数
	{
		//设置回调函数
		this->callback = callback;
	}



};

