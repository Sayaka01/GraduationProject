#pragma once
#include <d3d11.h>
#include <memory>

#include "scene.h"

#include <mutex>

class SceneManager
{
private:
	SceneManager() {}
	~SceneManager() {}
public:
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	void Initialize();
	void Update();
	void Draw();

	void Clear();
	void ChangeScene(Scene* scene, bool loading = false);

	std::mutex& GetMutex() { return mutex; }

private:
	std::unique_ptr<Scene> currentScene = nullptr;

	std::mutex mutex;

};