#pragma once

#include "scene.h"

//获取到这两个全局指针的引用
extern Scene* menu_scene;
extern Scene* game_scene;
extern Scene* selector_scene;


class SceneManager
{
public:
	enum class SceneType
	{
		Menu,
		Game,
		Selector
	};

public:
	SceneManager()= default;
	~SceneManager() = default; 

	void set_current_scene(Scene* scene)
	{
		current_scene = scene;
		current_scene->on_enter();
	}

	void switch_to(SceneType type)
	{
		current_scene->on_exit();//退出当前场景
		switch (type)//根据枚举参数切换场景
		{
		case SceneType::Menu:
			current_scene = menu_scene;
			break;
		case SceneType::Selector:
			current_scene = selector_scene;
			break;
		case SceneType::Game:
			current_scene = game_scene;
			break;
		default:
			break;
		}
		current_scene->on_enter();//进入新场景
	}

	void on_update(int delta)
	{
		current_scene->on_update(delta);
	}

	void on_draw()
	{
		current_scene->on_draw();
	}

	void on_input(const ExMessage& msg)
	{
		current_scene->on_input(msg);
	}
private:
	Scene* current_scene = nullptr;
};

