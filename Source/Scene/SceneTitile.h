#pragma once

#include "Scene.h"

class GameObject;

class SceneTitle :public Scene
{
public:
	SceneTitle() {}
	~SceneTitle()override {}

	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;

private:

	GameObject* gameObject = nullptr;

};