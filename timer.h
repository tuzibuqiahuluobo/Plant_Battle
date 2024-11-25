#pragma once
#include <functional>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	//���ö�ʱ����״̬
	void resart()
	{
		pass_time = 0;
		shotted = false;
	}

	//���ö�ʱ���ȴ���ʱ��
	void set_wait_time(int val)
	{
		wait_time = val;
	}

	//�Ƿ񵥴δ���
	void set_one_shot(bool flag)
	{
		one_shot = flag;
	}

	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}

	//��ʱ������ͣ
	void pause()
	{
		paused = true;
	}

	//��ʱ���Ļָ�
	void resume()
	{
		paused = false;
	}

	void on_update(int delta)
	{
		if (paused)
		{
			return;
		}
		//�ۼ�֡����ʵ�ʹ�ȥ��ʱ��
		pass_time += delta;
		//������ʱ�����������Ѿ���ȥ��ʱ������������ʱ����бȽ�
		if (pass_time >= wait_time)
		{
			//����Ѿ���ʱ,���Դ�����ʱ���Ļص�
			if ((!one_shot || (one_shot && !shotted)) && callback)
			{
				//���ȼ���˶�ʱ���Ƿ��ܹ�������
				//�����ʱ�����ǵ��δ���  !one_shot 
				//���߶�ʱ���ǵ��δ�������û�б�������  (one_shot && !shotted)
				//���ʾ���Լ�������
				//ͬʱ����Ҫ���callback�Ƿ���Ч  callback
				callback();
			}
			shotted = true;
			pass_time = 0;
		}
	}


private:
	int pass_time = 0;				//�ѹ�ʱ��
	int wait_time = 0;				//�ȴ�ʱ��
	bool paused = false;			//�Ƿ���ͣ
	bool shotted = false;			//�Ƿ񴥷�
	bool one_shot = false;			//���δ���
	std::function<void()> callback; //�����ص�����
};
