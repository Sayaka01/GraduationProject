#pragma once

#include "Scene.h"

class GameObject;

//*************************************************************
// �ʐM�p�̃V�[��(Communicate Scene)
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