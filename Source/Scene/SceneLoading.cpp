
#include "Stdafx.h"

#include "sceneLoading.h"
#include "sceneManager.h"
#include <thread>

SceneLoading::SceneLoading(Scene* nextScene)
{
	this->nextScene = nextScene;
}


void SceneLoading::Initialize()
{
	//スレッド開始
	std::thread thread(LoadingThread, this);
	//スレッドの管理を放棄
	thread.detach();
}
void SceneLoading::Finalize()
{

}
void SceneLoading::Update()
{
	//次のシーンの準備が完了したらシーンを切り替える
	if (nextScene->IsReady())
	{
		SceneManager::Instance().ChangeScene(nextScene, true);
	}
}
void SceneLoading::Draw()
{


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