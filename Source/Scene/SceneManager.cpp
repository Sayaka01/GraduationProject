
#include "Stdafx.h"

#include "sceneManager.h"

void SceneManager::Initialize()
{

}

void SceneManager::Update(float elapsedTime)
{

	if (currentScene != nullptr)
	{
		currentScene->Update(elapsedTime);
	}
	
}
void SceneManager::Draw(ID3D11DeviceContext* dc)
{
	if (currentScene != nullptr)
	{
		currentScene->Draw(dc);
	}
}
void SceneManager::Clear()
{
	if (currentScene != nullptr)
	{
		currentScene->Finalize();
		currentScene = nullptr;
	}
}
//シーンを切り替え
void SceneManager::ChangeScene(Scene* scene, bool loading)
{
	//古いシーンの終了処理
	Clear();

	//新しいシーンを設定
	currentScene.reset(scene);
	//シーン初期化処理
	if (!scene->IsReady())
	{
		currentScene->Initialize(device);
	}

}