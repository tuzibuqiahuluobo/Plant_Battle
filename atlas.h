#pragma once

#include <graphics.h>
#include<vector>

class Atlas {
public:
	Atlas() = default;
	~Atlas() = default;
	
	void load_from_file(LPCTSTR path_template,int num)
	{
		image_list.clear();
		image_list.resize(num);

		TCHAR path_file[256];
		for (int i = 0; i < num; i++)
		{
			//�˴�i+1����ΪͼƬ�ļ�����1��ʼ
			_stprintf_s(path_file, path_template, i+1);
			loadimage(&image_list[i], path_file);
		}
	}

	void clear()//���ͼ�����Ѽ��ص�ͼƬ����
	{
		image_list.clear();
	}

	int get_size()//��ȡͼ����ͼƬ���������
	{
		return (int)image_list.size();
	}

	IMAGE* get_image(int index)//��ȡʵ����Ⱦ�Ķ���֡
	{
		if (index < 0 || index >= image_list.size())		
		{//������������Ƿ���Ч�����Խ���򷵻ؿ�ָ��
		 //��֮���ض�Ӧ������ͼƬ����ĵ�ַ
			return nullptr;
		}

		return &image_list[index];
	}

	void add_image(const IMAGE& image)//���ͼƬ����ͼ����
	{
		image_list.push_back(image);

	}
private:
	//����img_listͼƬ�����б�
	std::vector<IMAGE> image_list;
};

