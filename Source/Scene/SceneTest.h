#pragma once

#include "Scene.h"

class GameObject;

class SceneTest :public Scene
{
public:
	SceneTest() {}
	~SceneTest()override {}

	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;

private:
	
	GameObject* gameObject = nullptr;

};