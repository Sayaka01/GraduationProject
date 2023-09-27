#include "../System/Other/Stdafx.h"

#include "SceneTest.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"

#include "../Component/Transform/Transform.h"


void SceneTest::Initialize(ID3D11Device* device)
{
	gameObject = new GameObject("test");
	auto* transform = new Transform();
	gameObject->AddComponent(transform);
}

void SceneTest::Finalize()
{
	gameObject->Finalize();
	delete gameObject;
}

void SceneTest::Update(float elapsedTime)
{
	gameObject->Update();
}

void SceneTest::Draw(ID3D11DeviceContext* dc)
{
	gameObject->Draw();
}
