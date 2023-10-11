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

	//€”õŠ®—¹‚µ‚Ä‚¢‚é‚©
	bool IsReady()const { return ready; }
	//€”õŠ®—¹İ’è
	void SetReady() { ready = true; }
	

private:
	bool ready = false;
};
