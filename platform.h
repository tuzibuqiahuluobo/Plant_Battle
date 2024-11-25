#pragma once

#include "Util.h"
#include "camera.h"

extern bool is_debug; // �Ƿ�Ϊ����ģʽ

class Platform
{
public:
	//����ƽ̨����ײ��״
	struct CollisionShape
	{
		float y;	//�����ƽ̨��y����
		float left;	//�����ƽ̨����߽�
		float right;	//�����ƽ̨���ұ߽�
	};

public:
	CollisionShape shape;	//��ײ��״
	IMAGE* img = nullptr;	//ƽ̨ͼƬ
	POINT render_position = { 0 };	//��Ⱦλ��

public:
	Platform() = default;
	~Platform() = default;

	void on_draw(const Camera& camera) const
	{
		putimage_alpha(camera, render_position.x, render_position.y, img);

		if (is_debug)
		{
			setlinecolor(RGB(255, 0, 0));
			line(camera, (int)shape.left, (int)shape.y, (int)shape.right, (int)shape.y);
		}
	}



private:

};

