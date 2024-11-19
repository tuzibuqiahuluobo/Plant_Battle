#pragma once
#include "scene.h"
#include "scene_manager.h"

#include <iostream>

extern SceneManager scene_manager;

class SelectorScene : public Scene
{
public:
	SelectorScene() = default;
	~SelectorScene() = default;

	
	void on_enter()
	{
		std::cout << "SelectorScene on_enter" << std::endl;//控制台打印
	}
	void on_update(int delta)
	{
		std::cout << "SelectorScene update" << std::endl;
	}

	void on_draw()
	{
		outtextxy(10, 10, _T("游戏选人绘图内容"));
	}

	void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			scene_manager.switch_to(SceneManager::SceneType::Game);
		}
	}

	void on_exit()
	{
		std::cout << "SelectorScene on_exit" << std::endl;
	}

private:

};

