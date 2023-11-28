#include "Stdafx.h"

#include "SceneCommunicate.h"

#include "SceneManager.h"

#include "../GameObject/GameObject.h"


void SceneCommunicate::Initialize()
{
	gameObject = new GameObject("test");
}

void SceneCommunicate::Finalize()
{
	gameObject->Finalize();
	delete gameObject;
}

void SceneCommunicate::Update()
{
	gameObject->Update();
}

void SceneCommunicate::Draw()
{
	gameObject->Draw3D();
}
