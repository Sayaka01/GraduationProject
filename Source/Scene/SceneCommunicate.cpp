#include "Stdafx.h"

#include "SceneCommunicate.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"


void SceneCommunicate::Initialize(ID3D11Device* device)
{
	gameObject = new GameObject("test");
}

void SceneCommunicate::Finalize()
{
	gameObject->Finalize();
	delete gameObject;
}

void SceneCommunicate::Update(float elapsedTime)
{
	gameObject->Update();
}

void SceneCommunicate::Draw(ID3D11DeviceContext* dc)
{
	gameObject->Draw();
}
