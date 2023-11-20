
#include "Stdafx.h"

#include "sceneLoading.h"
#include "sceneManager.h"
#include <thread>
#include "Component/SpriteRenderer.h"
#include <SimpleMath.h>
#include "System/SystemManager.h"

SceneLoading::SceneLoading(Scene* nextScene)
{
	this->nextScene = nextScene;
}


void SceneLoading::Initialize()
{
	sprLoading = new GameObject("loading");
	sprLoading->AddComponent(new SpriteRenderer(L"./Resources/Sprite/LoadingIcon.png"));
	sprLoading->GetComponent<SpriteRenderer>()->pos = {1150,600};
	sprLoading->GetComponent<SpriteRenderer>()->scale = { 1,1 };
	sprLoading->GetComponent<SpriteRenderer>()->pivot = sprLoading->GetComponent<SpriteRenderer>()->GetSpriteSize()*DirectX::SimpleMath::Vector2(0.5f, 0.5f);


	//スレッド開始
	std::thread thread(LoadingThread, this);
	//スレッドの管理を放棄
	thread.detach();
}
void SceneLoading::Finalize()
{
	sprLoading->Finalize();
	delete sprLoading;
}
void SceneLoading::Update()
{
	sprLoading->GetComponent<SpriteRenderer>()->degree += 90 * SystemManager::Instance().GetElapsedTime();
	sprLoading->Update();

	//次のシーンの準備が完了したらシーンを切り替える
	if (nextScene->IsReady())
	{
		SceneManager::Instance().ChangeScene(nextScene, true);
	}
}
void SceneLoading::Draw()
{
	sprLoading->Draw();
}

void SceneLoading::LoadingThread(SceneLoading* scene)
{
	//COM関連の初期化でスレッド毎に呼ぶ必要がある
	CoInitialize(nullptr);

	//次のシーンの初期化を行う
	scene->nextScene->Initialize();

	//スレッドが終わる前にCOM関連の終了化
	CoUninitialize();

	//次のシーンの準備完了設定
	scene->nextScene->SetReady();
	
}