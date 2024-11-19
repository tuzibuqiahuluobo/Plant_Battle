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
			//此处i+1是因为图片文件名从1开始
			_stprintf_s(path_file, path_template, i+1);
			loadimage(&image_list[i], path_file);
		}
	}

	void clear()//清空图集中已加载的图片对象
	{
		image_list.clear();
	}

	int get_size()//获取图集中图片对象的数量
	{
		return (int)image_list.size();
	}

	IMAGE* get_image(int index)//获取实际渲染的动画帧
	{
		if (index < 0 || index >= image_list.size())		
		{//检查索引参数是否有效，如果越界则返回空指针
		 //反之返回对应索引的图片对象的地址
			return nullptr;
		}

		return &image_list[index];
	}

	void add_image(const IMAGE& image)//添加图片对象到图集中
	{
		image_list.push_back(image);

	}
private:
	//定义img_list图片对象列表
	std::vector<IMAGE> image_list;
};

