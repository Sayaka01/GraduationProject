#pragma once

#include "Scene.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include "GameObject/GameObject.h"

class SceneGame :public Scene
{
public:
	SceneGame() {}
	~SceneGame()override {}

	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;

private:

	GameObject* gameObjectTest = nullptr;

};