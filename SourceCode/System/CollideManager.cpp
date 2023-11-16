
#include "Stdafx.h"
#include "CollideManager.h"

#include "Component/SphereMeshRenderer.h"
#include "Component/SphereCollider.h"
#include "Component/BoxMeshRenderer.h"
#include "Component/BoxCollider.h"
#include "Component/CapsuleMeshRenderer.h"
#include "Component/CapsuleCollider.h"

#include "Component/ModelRenderer.h"
#include "Component/Transform.h"

#include "GameObject/GameObject.h"

#include "System/PhysicsFunction.h"
#include "System/UserFunction.h"

void CollideManager::Initialize()
{
	//スフィアメッシュの生成
	sphereMesh = new SphereMeshRenderer();
	//ボックスメッシュの生成
	boxMesh = new BoxMeshRenderer();
	//カプセルメッシュの生成
	capsuleMesh = new CapsuleMeshRenderer();
}

void CollideManager::Finalize()
{
	//スフィアメッシュの削除
	SafeDelete(sphereMesh);
	sphereColliders.clear();

	//ボックスメッシュの削除
	SafeDelete(boxMesh);
	boxColliders.clear();

	//カプセルメッシュの削除
	SafeDelete(capsuleMesh);
	capsuleColliders.clear();
}

void CollideManager::Collide()
{
	//1回目だけ通らないようにする
	if (!firstUpdate)
	{
		firstUpdate = true;
		return;
	}

	//当たり判定を行う

	Physics::IntersectionResult result;

	//Sphere
	for (SphereCollider* colliderA : sphereColliders)
	{
		for (SphereCollider* colliderB : sphereColliders)
		{
			if (!colliderA->GetEnable() || !colliderB->GetEnable())continue;
			if (!colliderA->useHitEvent || !colliderB->useHitEvent)continue;
			if (colliderA->GetParent()->GetName() == colliderB->GetParent()->GetName())continue;

			if (Physics::IntersectSphereVsSphere(colliderA, colliderB, &result))
			{
				if (colliderA->priority > colliderB->priority)
				{
					//colliderAを押し出す
					colliderA->center += result.normal * result.penetration;
				}
				else if(colliderA->priority < colliderB->priority)
				{
					//colliderBを押し出す
					colliderB->center -= result.normal * result.penetration;
				}
				else
				{
					//半分ずつ押し出す
					colliderA->center += result.normal * result.penetration * 0.5f;
					colliderB->center -= result.normal * result.penetration * 0.5f;
				}
				
				if (colliderA->type == Collider::Type::Deffense && colliderB->type == Collider::Type::Offense)
					colliderA->OnCollisionEnter(colliderB);
				else if (colliderA->type == Collider::Type::Offense && colliderB->type == Collider::Type::Deffense)
					colliderB->OnCollisionEnter(colliderA);
				else
				{
					colliderA->OnCollisionEnter(colliderB);
					colliderB->OnCollisionEnter(colliderA);
				}
			}
		}
	}

	//Capsule
	for (CapsuleCollider* capsule : capsuleColliders)
	{
		for (SphereCollider* sphere : sphereColliders)
		{
			if (!capsule->GetEnable() || !sphere->GetEnable())continue;
			if (!capsule->useHitEvent || !sphere->useHitEvent)continue;

			if (capsule->GetParent()->GetTag() == sphere->GetParent()->GetTag())continue;


			if (Physics::IntersectSphereVsCapsule(sphere, capsule, &result))
			{
				if (capsule->priority > sphere->priority)
				{
					//capsuleを押し出す
					capsule->begin -= result.normal * result.penetration;
					capsule->end -= result.normal * result.penetration;
				}
				else if(capsule->priority < sphere->priority)
				{
					//colliderBを押し出す
					sphere->center += result.normal * result.penetration;
				}
				else
				{
					//半分ずつ押し出す
					capsule->begin -= result.normal * result.penetration;
					capsule->end -= result.normal * result.penetration;
					sphere->center += result.normal * result.penetration * 0.5f;
				}
				
				if (capsule->type == Collider::Type::Deffense && sphere->type == Collider::Type::Offense)
					capsule->OnCollisionEnter(sphere);
				else if (capsule->type == Collider::Type::Offense && sphere->type == Collider::Type::Deffense)
					sphere->OnCollisionEnter(capsule);
				else
				{
					capsule->OnCollisionEnter(sphere);
					sphere->OnCollisionEnter(capsule);
				}

			}
		}
	}
}

void CollideManager::Draw()
{
#ifdef _DEBUG
	//球描画
	sphereMesh->DrawPrepare();
	for (SphereCollider* collider : sphereColliders)
	{
		if (collider->GetEnable() && collider->drawDebugPrimitive)
			sphereMesh->Draw(collider);
	}

	//Box描画
	boxMesh->DrawPrepare();
	for (BoxCollider* collider : boxColliders)
	{
		if (collider->GetEnable() && collider->drawDebugPrimitive)
			boxMesh->Draw(collider);
	}

	//カプセル描画
	capsuleMesh->DrawPrepare();
	for (CapsuleCollider* collider : capsuleColliders)
	{
		if (collider->GetEnable() && collider->drawDebugPrimitive)
		{
			collider->CalcCapsuleParam();
			capsuleMesh->Draw(collider);
		}
	}
#endif
}

void CollideManager::RemoveSphereCollider(SphereCollider* sphereCollider)
{
	std::vector<SphereCollider*>::iterator it = std::find(sphereColliders.begin(), sphereColliders.end(), sphereCollider);
	if (it != sphereColliders.end())
	{
		sphereCollider->Finalize();
		sphereColliders.erase(it);
	}
}

void CollideManager::RemoveBoxCollider(BoxCollider* boxCollider)
{
	std::vector<BoxCollider*>::iterator it = std::find(boxColliders.begin(), boxColliders.end(), boxCollider);
	if (it != boxColliders.end())
	{
		boxCollider->Finalize();
		boxColliders.erase(it);
	}
}

void CollideManager::RemoveCapsuleCollider(CapsuleCollider* capsuleCollider)
{
	std::vector<CapsuleCollider*>::iterator it = std::find(capsuleColliders.begin(), capsuleColliders.end(), capsuleCollider);
	if (it != capsuleColliders.end())
	{
		capsuleCollider->Finalize();
		capsuleColliders.erase(it);
	}
}

void CollideManager::CreateBoundingBox(ModelRenderer* modelRenderer)
{
	//モデルデータの取得
	ModelData* data = modelRenderer->GetModelResource();

	//メッシュ数の取得
	int meshCount = data->GetMeshesSize();

	for (int meshIndex = 0; meshIndex < meshCount; meshIndex++)
	{
		//頂点数の取得
		int vertexCount = data->GetVerticesSize(meshIndex);

		DirectX::XMFLOAT3 min{ +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX };
		DirectX::XMFLOAT3 max{ -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX };

		//最大、最小位置の頂点を算出
		for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++)
		{
			//頂点の取得
			ModelData::Vertex& vertex = data->GetVertex(meshIndex, vertexIndex);

			min.x = std::min<float>(min.x, vertex.position.x);
			min.y = std::min<float>(min.y, vertex.position.y);
			min.z = std::min<float>(min.z, vertex.position.z);
								   	   
			max.x = std::max<float>(max.x, vertex.position.x);
			max.y = std::max<float>(max.y, vertex.position.y);
			max.z = std::max<float>(max.z, vertex.position.z);
		}

		//ワールド行列の作成
		Transform* transform = modelRenderer->GetParent()->GetComponent<Transform>();
		transform->Update();

		DirectX::XMFLOAT4X4 world = data->GetMeshGlobalTransform(meshIndex) * transform->world;

		//算出した頂点をワールド空間に変換
		min = min * world;
		max = max * world;
		
		//ボックスの作成
		BoxCollider* box = new BoxCollider("Box" + std::to_string(meshIndex));
		box->size = max - min;
		box->center = min + box->size * 0.5f;

		modelRenderer->GetParent()->AddComponent(box);
	}
}
