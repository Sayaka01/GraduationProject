#pragma once
#include <d3d11.h>

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//�����������Ă��邩
	bool IsReady()const { return ready; }
	//���������ݒ�
	void SetReady() { ready = true; }
	

private:
	bool ready = false;
};
