
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
//�V�[����؂�ւ�
void SceneManager::ChangeScene(Scene* scene, bool loading)
{
	//�Â��V�[���̏I������
	Clear();

	//�V�����V�[����ݒ�
	currentScene.reset(scene);
	//�V�[������������
	if (!scene->IsReady())
	{
		currentScene->Initialize(device);
	}

}