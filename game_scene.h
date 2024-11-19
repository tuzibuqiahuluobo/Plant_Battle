#pragma once

#include "scene.h"
#include "scene_manager.h"

#include <iostream>

extern SceneManager scene_manager;

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

private:
	void on_enter()
	{
		std::cout << "GameScene on_enter" << std::endl;//控制台打印
	}
	void on_update(int delta)
	{
		std::cout << "GameScene update" << std::endl;
	}
	void on_draw()
	{
		outtextxy(10, 10, _T("游戏内容绘图"));
	}
	void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			scene_manager.switch_to(SceneManager::SceneType::Menu);
		}
	}

	void on_exit()
	{
		std::cout << "GameScene on_exit" << std::endl;
	}

};