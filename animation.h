#pragma once

#include "atlas.h"
#include "Util.h"
#include "camera.h"
#include <functional>
#include <graphics.h>

class Animation
{
private:
	int timer = 0;			//��ʱ��
	int interval = 0;		//֡���
	int idx_frame = 0;		//��ǰ֡����
	bool is_loop = true;	//�Ƿ�ѭ������

	Atlas* atlas = nullptr;
	std::function<void()> callback;
	//function��һ��ģ����,<>�����������������洢�ĺ�������ֵΪvoid,����Ϊ�յĺ���ָ��

public:
	Animation() = default;
	~Animation() = default;

	void reset()//���ö�����״̬
	{//�ڶ�����Ҫ��ͷ����ʱʹ��
		//���ü�ʱ����֡����
		timer = 0;
		idx_frame = 0;

	}

	void set_atlas(Atlas* new_atlas)//�������ö�����ͼ��
	{
		//�����µ�ͼ��ʱ�����ö���״̬
		reset();
		atlas = new_atlas;
	}

	void set_loop(bool flag)//�����Ƿ�ѭ������
	{
		is_loop = flag;
	}
	

	void set_interval(int ms)//����֡���
	{
		interval = ms;
	}

	int get_idx_frame()//��ȡ��ǰ֡����
	{
		return idx_frame;
	}

	IMAGE* get_frame()//��ȡ��ǰ֡��ͼ��
	{
		return atlas->get_image(idx_frame);
	}

	bool check_finished()//��鶯���Ƿ񲥷����
	{
		if (is_loop)
		{
			return false;
		}
		//�����ǰ֡��������ͼ����С��һ������true
		return (idx_frame == atlas->get_size() - 1);
	}

	void on_update(int delta)//���¶���״̬
	{
		timer += delta;//��ʱ���ۼ�
		if (timer >= interval)//�����ʱ������֡���
		{
			timer = 0;//��ʱ������
			idx_frame++;//֡��������
			if (idx_frame >= atlas->get_size())//�����ǰ֡��������ͼ����С
			{
				//�����Ƿ�ѭ���������õ�ǰ֡�����Ǵ�ͷ��ʼ���Ǳ��ֵ����һ֡
				idx_frame = is_loop ? 0 : atlas->get_size() - 1;
				if (!is_loop && callback)
				{
					//���֡����������ͼ����β�����Ҷ���û����ѭ�������һص���������
					callback();//��ִ�лص�����
				}
			}
		}
	}

	void on_draw(const Camera& camera , int x, int y) const //���Ƶ�ǰ֡
	{
		//���ù��ߺ������ƴ���͸��Ч����ͼ��
		putimage_alpha(camera,x, y, atlas->get_image(idx_frame));
	}

	void set_callback(std::function<void()> callback)//���ö���������Ϻ�Ļص�����
	{
		//���ûص�����
		this->callback = callback;
	}



};

