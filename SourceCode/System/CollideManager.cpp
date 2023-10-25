
#include "Stdafx.h"
#include "CollideManager.h"

#include "Component/SphereMeshRenderer.h"

void CollideManager::Initialize()
{
	sphereMesh = new SphereMeshRenderer();

}

void CollideManager::Finalize()
{
	if (sphereMesh)
	{
		delete sphereMesh;
		sphereMesh = nullptr;
	}

	sphereColliders.clear();
}

void CollideManager::Collide()
{
	//“–‚½‚è”»’è‚ðs‚¤

}

void CollideManager::Draw()
{
	//‹…•`‰æ
	sphereMesh->DrawPrepare();
	for (SphereCollider* collider : sphereColliders)
	{
		sphereMesh->Draw(collider);
	}
}
