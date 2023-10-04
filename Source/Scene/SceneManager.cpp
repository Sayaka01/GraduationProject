
#include "Stdafx.h"

#include "sceneManager.h"

void SceneManager::Initialize()
{

}

void SceneManager::Update()
{

	if (currentScene != nullptr)
	{
		currentScene->Update();
	}
	
}
void SceneManager::Draw()
{
	if (currentScene != nullptr)
	{
		currentScene->Draw();
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
		currentScene->Initialize();
	}

}