#pragma once
#include <functional>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	//重置定时器的状态
	void resart()
	{
		pass_time = 0;
		shotted = false;
	}

	//设置定时器等待的时间
	void set_wait_time(int val)
	{
		wait_time = val;
	}

	//是否单次触发
	void set_one_shot(bool flag)
	{
		one_shot = flag;
	}

	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}

	//计时器的暂停
	void pause()
	{
		paused = true;
	}

	//计时器的恢复
	void resume()
	{
		paused = false;
	}

	void on_update(int delta)
	{
		if (paused)
		{
			return;
		}
		//累加帧更新实际过去的时间
		pass_time += delta;
		//并将定时器启动以来已经过去的时间和期望到达的时间进行比较
		if (pass_time >= wait_time)
		{
			//如果已经超时,则尝试触发定时器的回调
			if ((!one_shot || (one_shot && !shotted)) && callback)
			{
				//首先检查了定时器是否能够被触发
				//如果定时器不是单次触发  !one_shot 
				//或者定时器是单次触发并且没有被触发过  (one_shot && !shotted)
				//则表示可以继续触发
				//同时还需要检查callback是否有效  callback
				callback();
			}
			shotted = true;
			pass_time = 0;
		}
	}


private:
	int pass_time = 0;				//已过时间
	int wait_time = 0;				//等待时间
	bool paused = false;			//是否暂停
	bool shotted = false;			//是否触发
	bool one_shot = false;			//单次触发
	std::function<void()> callback; //触发回调函数
};
