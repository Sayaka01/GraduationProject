#pragma once

#include "Scene.h"

class SceneLoading : public Scene
{
public:
	SceneLoading(Scene* nextScene);
	~SceneLoading()override {}

	void Initialize(ID3D11Device* device)override;
	void Finalize()override;
	void Update(float elapsedTime)override;
	void Draw(ID3D11DeviceContext* dc)override;
private:
	static void LoadingThread(SceneLoading* scene);
private:
	Scene* nextScene = nullptr;
};