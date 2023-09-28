
#include "Stdafx.h"

#include "sceneLoading.h"
#include "sceneManager.h"
#include <thread>

SceneLoading::SceneLoading(Scene* nextScene)
{
	this->nextScene = nextScene;
}


void SceneLoading::Initialize(ID3D11Device* device)
{
	//スレッド開始
	std::thread thread(LoadingThread, this);
	//スレッドの管理を放棄
	thread.detach();
}
void SceneLoading::Finalize()
{

}
void SceneLoading::Update(float elapsedTime)
{
	//次のシーンの準備が完了したらシーンを切り替える
	if (nextScene->IsReady())
	{
		SceneManager::Instance().ChangeScene(nextScene, true);
	}
}
void SceneLoading::Draw(ID3D11DeviceContext* dc)
{


}

void SceneLoading::LoadingThread(SceneLoading* scene)
{
	//COM関連の初期化でスレッド毎に呼ぶ必要がある
	CoInitialize(nullptr);

	ID3D11Device* device = SceneManager::Instance().GetDevice();

	//次のシーンの初期化を行う
	scene->nextScene->Initialize(device);

	//スレッドが終わる前にCOM関連の終了化
	CoUninitialize();

	//次のシーンの準備完了設定
	scene->nextScene->SetReady();
	
}