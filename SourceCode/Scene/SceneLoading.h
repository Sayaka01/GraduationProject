#pragma once

#include "Scene.h"
#include "GameObject/GameObject.h"

class SceneLoading : public Scene
{
public:
	SceneLoading(Scene* nextScene);
	~SceneLoading()override {}

	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;
private:
	static void LoadingThread(SceneLoading* scene);
private:
	Scene* nextScene = nullptr;
	GameObject* sprLoading{ nullptr };
};