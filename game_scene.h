#pragma once

#include "scene.h"
#include "Util.h"
#include "platform.h"
#include "player.h"
#include "scene_manager.h"

extern Player* player_1;
extern Player* player_2;

extern IMAGE img_sky; // 天空图片
extern IMAGE img_hills; // 山脉图片
extern IMAGE img_platform_large; // 大型平台图片
extern IMAGE img_platform_small; // 小型平台图片

extern Camera main_camera;
extern std::vector<Platform> platform_list;

extern SceneManager scene_manager;

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

private:
	void on_enter()
	{
		//初始化玩家1和玩家2的位置
		player_1->set_position(200, 50);
		player_2->set_position(975, 50);

		pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

		pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;

		//初始化平台
		platform_list.resize(4);

		//大型平台
		Platform& large_platform = platform_list[0];
		large_platform.img = &img_platform_large;
		//渲染位置
		large_platform.render_position.x = 122;
		large_platform.render_position.y = 455;
		//碰撞形状的坐标
		large_platform.shape.left = (float)large_platform.render_position.x + 30;
		large_platform.shape.right = (float)large_platform.render_position.x + large_platform.img->getwidth() - 30;
		large_platform.shape.y = (float)large_platform.render_position.y + 60;

		//三个小型平台
		Platform& small_platform_1 = platform_list[1];
		small_platform_1.img = &img_platform_small;
		small_platform_1.render_position.x = 175;
		small_platform_1.render_position.y = 360;
		small_platform_1.shape.left = (float)small_platform_1.render_position.x + 40;
		small_platform_1.shape.right = (float)small_platform_1.render_position.x + small_platform_1.img->getwidth() - 40;
		small_platform_1.shape.y = (float)small_platform_1.render_position.y + img_platform_small.getheight() / static_cast<float>(2);

		Platform& small_platform_2 = platform_list[2];
		small_platform_2.img = &img_platform_small;
		small_platform_2.render_position.x = 855;
		small_platform_2.render_position.y = 360;
		small_platform_2.shape.left = (float)small_platform_2.render_position.x + 40;
		small_platform_2.shape.right = (float)small_platform_2.render_position.x + small_platform_2.img->getwidth() - 40;
		small_platform_2.shape.y = (float)small_platform_2.render_position.y + img_platform_small.getheight() / static_cast<float>(2);

		Platform& small_platform_3 = platform_list[3];
		small_platform_3.img = &img_platform_small;
		small_platform_3.render_position.x = 515;
		small_platform_3.render_position.y = 225;
		small_platform_3.shape.left = (float)small_platform_3.render_position.x + 40;
		small_platform_3.shape.right = (float)small_platform_3.render_position.x + small_platform_3.img->getwidth() - 40;
		small_platform_3.shape.y = (float)small_platform_3.render_position.y + img_platform_small.getheight() / static_cast<float>(2);


	}
	void on_update(int delta)
	{
		//调用玩家1和玩家2的更新方法
		player_1->on_update(delta);
		player_2->on_update(delta);
	}
	void on_draw(const Camera& camera)
	{
		putimage_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
		putimage_alpha(camera, pos_img_hills.x, pos_img_hills.y, &img_hills);

		//遍历每一个平台对象并渲染
		for (const auto& platform : platform_list)
		{
			platform.on_draw(camera);
		}

		if (is_debug)
		{
			settextcolor(RGB(255, 0, 0));
			outtextxy(15, 15, _T("已开始调试模式，按 ' Q ' 键关闭"));
		}
		player_1->on_draw(camera);
		player_2->on_draw(camera);
	}
	void on_input(const ExMessage& msg)
	{
		//将操作消息传递给玩家1和玩家2
		player_1->on_input(msg);
		player_2->on_input(msg);

		switch (msg.message)
		{
		case WM_KEYUP:
			//'Q'
			if (msg.vkcode == 0x51)
			{
				is_debug = !is_debug;
			}
			break;
		default:
			break;
		}
	}

	void on_exit()
	{

	}

private:
	POINT pos_img_sky = { 0 };			// 天空图片位置
	POINT pos_img_hills = { 0 };		// 山脉图片位置

};