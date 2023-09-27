#pragma once
#include <d3d11.h>

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	virtual void Initialize(ID3D11Device* device) = 0;
	virtual void Finalize() = 0;
	virtual void Update(float elapsedTime) = 0;
	virtual void Draw(ID3D11DeviceContext* dc) = 0;

	//�����������Ă��邩
	bool IsReady()const { return ready; }
	//���������ݒ�
	void SetReady() { ready = true; }
	

private:
	bool ready = false;
};
