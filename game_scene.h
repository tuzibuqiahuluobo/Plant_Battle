#pragma once

#include "scene.h"
#include "Util.h"
#include "platform.h"
#include "player.h"
#include "scene_manager.h"
#include "status_bar.h"

extern Player* player_1;
extern Player* player_2;

extern IMAGE img_sky; // 天空图片
extern IMAGE img_hills; // 山脉图片
extern IMAGE img_platform_large; // 大型平台图片
extern IMAGE img_platform_small; // 小型平台图片

extern IMAGE img_1P_winner; // 1P 获胜文本图片
extern IMAGE img_2P_winner; // 2P 获胜文本图片
extern IMAGE img_winner_bar; // 获胜玩家背景文本图片

extern Camera main_camera;
extern std::vector<Platform> platform_list;

extern IMAGE* img_player_1_avatar; // 玩家1头像图片
extern IMAGE* img_player_2_avatar; // 玩家2头像图片

extern SceneManager scene_manager;

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

private:
	void on_enter()
	{
		is_game_over = false;
		is_slide_out_started = false;

		pos_img_winner_bar.x = -img_winner_bar.getwidth();
		pos_img_winner_bar.y = (getheight() - img_winner_bar.getheight()) / 2;
		pos_x_img_winner_bar_dst = (getwidth() - img_winner_bar.getwidth()) / 2;

		pos_img_winner_text.x = pos_img_winner_bar.x;
		pos_img_winner_text.y = (getheight() - img_1P_winner.getheight()) / 2;
		pos_x_img_winner_text_dst = (getwidth() - img_1P_winner.getwidth()) / 2;

		timer_winner_slide_in.restart();
		timer_winner_slide_in.set_wait_time(2500);
		timer_winner_slide_in.set_one_shot(true);
		timer_winner_slide_in.set_callback([&]()
			{
				is_slide_out_started = true;
			});

		timer_winner_slide_out.restart();
		timer_winner_slide_out.set_wait_time(1000);
		timer_winner_slide_out.set_one_shot(true);
		timer_winner_slide_out.set_callback([&]()
			{
				scene_manager.switch_to(SceneManager::SceneType::Menu);
			});


		//初始化玩家头像和位置
		status_bar_1P.set_avatar(img_player_1_avatar);
		status_bar_2P.set_avatar(img_player_2_avatar);

		status_bar_1P.set_position(235, 625);
		status_bar_2P.set_position(675, 625); 

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

		mciSendString(_T("play bgmGame repeat from 0"), NULL, 0, NULL);
	}

	void on_update(int delta)
	{
		//调用玩家1和玩家2的更新方法
		player_1->on_update(delta);
		player_2->on_update(delta);

		main_camera.on_update(delta);

		//调用 std::vector::erase 函数来删除 bullet_list 中的元素
		bullet_list.erase(std::remove_if( //使用 std::remove_if 函数重新排列 bullet_list 中的元素，使得所有需要删除的元素都被移动到容器的末尾
			bullet_list.begin(), 
			bullet_list.end(), 
			[](const Bullet* bullet) //Lambda 表达式，用于判断每个子弹是否需要删除。
			{ 
				bool deletable = bullet->check_can_remove(); //调用 Bullet 类的 check_can_remove 方法，检查子弹是否可以删除
				if (deletable) 
				{
					delete bullet;
				}
				return deletable;
			}),
			bullet_list.end());

		for (Bullet* bullet : bullet_list)
		{
			bullet->on_update(delta);
		}

		const Vector2& position_player_1 = player_1->get_position();
		const Vector2& position_player_2 = player_2->get_position();

		if (position_player_1.y >= getheight())
		{
			player_1->set_hp(0);
		}
		if (position_player_2.y >= getheight())
		{
			player_2->set_hp(0);
		}
		if (player_1->get_hp() <= 0 || player_2->get_hp() <= 0)
		{
			if (!is_game_over)
			{
				mciSendString(_T("stop bgmGame"), NULL, 0, NULL);
				mciSendString(_T("play uiWin from 0"), NULL, 0, NULL);
			}

			is_game_over = true;
		}


		status_bar_1P.set_hp(player_1->get_hp());
		status_bar_1P.set_mp(player_1->get_mp());
		status_bar_2P.set_hp(player_2->get_hp());
		status_bar_2P.set_mp(player_2->get_mp());

		if (is_game_over)
		{
			pos_img_winner_bar.x += (int)(speed_winner_bar * delta);
			pos_img_winner_text.x += (int)(speed_winner_text * delta);

			if (!is_slide_out_started)
			{
				timer_winner_slide_in.on_update(delta);
				if (pos_img_winner_bar.x > pos_x_img_winner_bar_dst)
				{
					pos_img_winner_bar.x = pos_x_img_winner_bar_dst;
				}
				if (pos_img_winner_text.x > pos_x_img_winner_text_dst)
				{
					pos_img_winner_text.x = pos_x_img_winner_text_dst;
				}
			}
			else
			{
				timer_winner_slide_out.on_update(delta);
			}
		}
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

		for (const Bullet* bullet : bullet_list)
		{
			bullet->on_draw(camera);
		}

		if (is_game_over)
		{
			putimage_alpha(pos_img_winner_bar.x, pos_img_winner_bar.y, &img_winner_bar);
			putimage_alpha(pos_img_winner_text.x, pos_img_winner_text.y,
				player_1->get_hp() > 0 ? &img_1P_winner : &img_2P_winner);
		}
		else
		{
			status_bar_1P.on_draw();
			status_bar_2P.on_draw();
		}
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
		delete player_1;
		player_1 = nullptr;
		delete player_2;
		player_2 = nullptr;

		is_debug = false;

		bullet_list.clear();
		main_camera.reset();
	}

private:
	const double speed_winner_bar = 1;
	const double speed_winner_text = 1;

	POINT pos_img_sky = { 0 };				// 天空图片位置
	POINT pos_img_hills = { 0 };			// 山脉图片位置

	StatusBar status_bar_1P;				// 玩家1的状态栏
	StatusBar status_bar_2P;				// 玩家2的状态栏

	bool is_game_over = false;				// 游戏是否结束

	POINT pos_img_winner_bar = { 0 };		// 结算动效背景图片位置
	POINT pos_img_winner_text = { 0 };		// 结算动效文本位置

	int pos_x_img_winner_bar_dst = 0;		// 结算动效背景移动的目标位置
	int pos_x_img_winner_text_dst = 0;		// 结算动效文本移动的目标位置

	Timer timer_winner_slide_in;			// 结算动效滑入计时器
	Timer timer_winner_slide_out;			// 结算动效滑出计时器

	bool is_slide_out_started = false;		// 结算动效是否开始滑出
};