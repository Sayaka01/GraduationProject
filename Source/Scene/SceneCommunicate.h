#pragma once

#include "Scene.h"

class GameObject;

//*************************************************************
// 通信用のシーン(Communicate Scene)
//*************************************************************

class SceneCommunicate :public Scene
{
public:
	SceneCommunicate() {}
	~SceneCommunicate()override {}

	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;

private:

	GameObject* gameObject = nullptr;

};