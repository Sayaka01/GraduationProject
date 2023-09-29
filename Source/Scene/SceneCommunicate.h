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

	void Initialize(ID3D11Device* device)override;
	void Finalize()override;
	void Update(float elapsedTime)override;
	void Draw(ID3D11DeviceContext* dc)override;

private:

	GameObject* gameObject = nullptr;

};