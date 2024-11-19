#ifndef _SCENE_H_
#define _SCENE_H_

#include"scene.h"
#include"atlas.h"
#include"timer.h"
#include"camera.h"
#include"animation.h"
#include"scene_manager.h"

#include <iostream> 

//使用extern声明外部的 豌豆动画图集对象
extern Atlas atlas_peashooter_run_right;
//使用extern声明外部的 scene_manager对象
extern SceneManager scene_manager;

class MenuScene : public Scene //继承Scene基类
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter()
	{
		//设置豌豆射手跑步动画所使用的图集、帧间隔、循环状态
		animation_peashooter_run_right.set_atlas(&atlas_peashooter_run_right);
		animation_peashooter_run_right.set_interval(75);
		animation_peashooter_run_right.set_loop(true);

		timer.set_wait_time(1000);
		timer.set_one_shot(false);
		timer.set_callback(
			[]()
			{
				std::cout << "Shot!" << std::endl;
			}
		);

		//lambda匿名函数
		//[捕获列表](参数列表) mutable(可选) 异常属性 -> 返回类型 {
		// 函数体
		//}
		/*animation_peashooter_run_right.set_callback(
			[]()
			{
				scene_manager.switch_to(SceneManager::SceneType::Game);
			}
		);*/
	}
	void on_update(int delta) 
	{
		timer.on_update(delta);

		camera.on_update(delta);
		animation_peashooter_run_right.on_update(delta);
	}
	void on_draw()
	{
		const Vector2& pos_camera = camera.get_position();
		animation_peashooter_run_right.on_draw((int)(100 - pos_camera.x), (int)(100 - pos_camera.y));
		//animation_peashooter_run_right.on_draw(100, 100);
	}
	void on_input(const ExMessage& msg) 
	{
		/*if (msg.message == WM_KEYDOWN)
		{
			scene_manager.switch_to(SceneManager::SceneType::Selector);
		}*/
		//按下任意键时，摄像机抖动
		if (msg.message == WM_KEYDOWN)
		{
			camera.shake(10, 350);
		}

	}

	void on_exit()
	{
		std::cout << "MenuScene on_exit" << std::endl;
	}



private:
	Timer timer; //定时器
	Camera camera;
	Animation animation_peashooter_run_right;
};






#endif // !_SCENE_H_
