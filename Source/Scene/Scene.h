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

	//€”õŠ®—¹‚µ‚Ä‚¢‚é‚©
	bool IsReady()const { return ready; }
	//€”õŠ®—¹İ’è
	void SetReady() { ready = true; }
	

private:
	bool ready = false;
};
