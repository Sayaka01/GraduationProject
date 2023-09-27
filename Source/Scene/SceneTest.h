#pragma once

#include "Scene.h"

class GameObject;

class SceneTest :public Scene
{
public:
	SceneTest() {}
	~SceneTest()override {}

	void Initialize(ID3D11Device* device)override;
	void Finalize()override;
	void Update(float elapsedTime)override;
	void Draw(ID3D11DeviceContext* dc)override;

private:
	
	GameObject* gameObject = nullptr;

};