
#include "Stdafx.h"
#include "CollideManager.h"

#include "Component/SphereMeshRenderer.h"
#include "Component/SphereCollider.h"
#include "Component/CapsuleMeshRenderer.h"
#include "Component/CapsuleCollider.h"

#include "System/PhysicsFunction.h"
#include "System/UserFunction.h"

void CollideManager::Initialize()
{
	//�X�t�B�A���b�V���̐���
	sphereMesh = new SphereMeshRenderer();
	//�J�v�Z�����b�V���̐���
	capsuleMesh = new CapsuleMeshRenderer();
}

void CollideManager::Finalize()
{
	//�X�t�B�A���b�V���̍폜
	SafeDelete(sphereMesh);
	sphereColliders.clear();

	//�J�v�Z�����b�V���̍폜
	SafeDelete(capsuleMesh);
	capsuleColliders.clear();
}

void CollideManager::Collide()
{
	//�����蔻����s��

	Physics::IntersectionResult result;

	//Sphere
	for (SphereCollider* colliderA : sphereColliders)
	{
		for (SphereCollider* colliderB : sphereColliders)
		{
			if (!colliderA->GetEnable())continue;
			if (!colliderB->GetEnable())continue;
			if (colliderA->GetParentName() == colliderB->GetParentName())continue;
			if (colliderA == colliderB)continue;

			if (Physics::IntersectSphereVsSphere(colliderA, colliderB, &result))
			{
				DirectX::XMFLOAT3 centerA = colliderA->GetCenter();
				DirectX::XMFLOAT3 centerB = colliderA->GetCenter();

				if (colliderA->priority > colliderB->priority)
				{
					//colliderA�������o��
					centerA += result.normal * result.penetration;
				}
				else if(colliderA->priority < colliderB->priority)
				{
					//colliderB�������o��
					centerB -= result.normal * result.penetration;
				}
				else
				{
					//�����������o��
					centerA += result.normal * result.penetration * 0.5f;
					centerB -= result.normal * result.penetration * 0.5f;
				}

				colliderA->SetCenter(centerA);
				colliderB->SetCenter(centerB);
				
				colliderA->OnCollisionEnter(colliderB);
				colliderB->OnCollisionEnter(colliderA);

			}
		}
	}

	//Capsule
	for (CapsuleCollider* capsule : capsuleColliders)
	{
		for (SphereCollider* sphere : sphereColliders)
		{
			if (!capsule->GetEnable())continue;
			if (!sphere->GetEnable())continue;

			if (capsule->GetParentName() == sphere->GetParentName())continue;


			if (Physics::IntersectSphereVsCapsule(sphere, capsule, &result))
			{
				DirectX::XMFLOAT3 sphereCenter = sphere->GetCenter();

				if (capsule->priority > sphere->priority)
				{
					//capsule�������o��
					capsule->begin -= result.normal * result.penetration;
					capsule->end -= result.normal * result.penetration;
				}
				else if(capsule->priority < sphere->priority)
				{
					//colliderB�������o��
					sphereCenter += result.normal * result.penetration;
				}
				else
				{
					//�����������o��
					capsule->begin -= result.normal * result.penetration;
					capsule->end -= result.normal * result.penetration;
					sphereCenter += result.normal * result.penetration * 0.5f;
				}

				sphere->SetCenter(sphereCenter);
				
				capsule->OnCollisionEnter(sphere);
				sphere->OnCollisionEnter(capsule);

			}
		}
	}
}

void CollideManager::Draw()
{
	//���`��
	sphereMesh->DrawPrepare();
	for (SphereCollider* collider : sphereColliders)
	{
		if (collider->GetEnable() && collider->drawDebugPrimitive)
			sphereMesh->Draw(collider);
	}

	//�J�v�Z���`��
	capsuleMesh->DrawPrepare();
	for (CapsuleCollider* collider : capsuleColliders)
	{
		if (collider->GetEnable() && collider->drawDebugPrimitive)
		{
			capsuleMesh->Draw(collider);
		}
	}
}
