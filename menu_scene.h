#pragma once
#include"scene.h"
//#include"atlas.h"
//#include"timer.h"
//#include"camera.h"
//#include"animation.h"
#include"scene_manager.h"

#include <iostream> 



//ʹ��extern�����ⲿ�� �㶹����ͼ������
//extern Atlas atlas_peashooter_run_right;
//ʹ��extern�����ⲿ�� scene_manager����
extern SceneManager scene_manager;

extern IMAGE img_menu_background;

class MenuScene : public Scene //�̳�Scene����
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter()
	{
		////�����㶹�����ܲ�������ʹ�õ�ͼ����֡�����ѭ��״̬
		//animation_peashooter_run_right.set_atlas(&atlas_peashooter_run_right);
		//animation_peashooter_run_right.set_interval(75);
		//animation_peashooter_run_right.set_loop(true);

		//timer.set_wait_time(1000);
		//timer.set_one_shot(false);
		//timer.set_callback(
		//	[]()
		//	{
		//		std::cout << "Shot!" << std::endl;
		//	}
		//);

		//lambda��������
		//[�����б�](�����б�) mutable(��ѡ) �쳣���� -> �������� {
		// ������
		//}
		/*animation_peashooter_run_right.set_callback(
			[]()
			{
				scene_manager.switch_to(SceneManager::SceneType::Game);
			}
		);*/

		mciSendString(_T("play bgmMenu repeat from 0"), NULL, 0, NULL);
	}
	void on_update(int delta) 
	{
		/*timer.on_update(delta);

		camera.on_update(delta);
		animation_peashooter_run_right.on_update(delta);*/
	}
	void on_draw(const Camera& camera)
	{
		//��Ⱦ����ͼƬ
		putimage(0, 0, &img_menu_background);
		//const Vector2& pos_camera = camera.get_position();
		//animation_peashooter_run_right.on_draw((int)(100 - pos_camera.x), (int)(100 - pos_camera.y));
		////animation_peashooter_run_right.on_draw(100, 100);
	}
	void on_input(const ExMessage& msg) 
	{
		if (msg.message == WM_KEYUP)
		{
			mciSendString(_T("play uiConfirm from 0"), NULL, 0, NULL);
			scene_manager.switch_to(SceneManager::SceneType::Selector);
		}

		/*if (msg.message == WM_KEYDOWN)
		{
			scene_manager.switch_to(SceneManager::SceneType::Selector);
		}*/
		////���������ʱ�����������
		//if (msg.message == WM_KEYDOWN)
		//{
		//	camera.shake(10, 350);
		//}

	}

	/*void on_exit()
	{
		std::cout << "MenuScene on_exit" << std::endl;
	}*/



private:
	//Timer timer; //��ʱ��
	//Camera camera;
	//Animation animation_peashooter_run_right;
};





