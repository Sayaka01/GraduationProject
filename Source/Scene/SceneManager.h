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
	void Update(float elapsedTime);
	void Draw(ID3D11DeviceContext* dc);

	void Clear();
	void ChangeScene(Scene* scene, bool loading = false);

	ID3D11Device* GetDevice() const { return device; }
	void SetDevice(ID3D11Device* d) { device = d; }

	std::mutex& GetMutex() { return mutex; }

private:
	ID3D11Device* device = nullptr;

	std::unique_ptr<Scene> currentScene = nullptr;

	std::mutex mutex;

};