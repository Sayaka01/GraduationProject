
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
	//�X�t�B�A���b�V���̐���
	sphereMesh = new SphereMeshRenderer();
	//�{�b�N�X���b�V���̐���
	boxMesh = new BoxMeshRenderer();
	//�J�v�Z�����b�V���̐���
	capsuleMesh = new CapsuleMeshRenderer();
}

void CollideManager::Finalize()
{
	//�X�t�B�A���b�V���̍폜
	SafeDelete(sphereMesh);
	sphereColliders.clear();

	//�{�b�N�X���b�V���̍폜
	SafeDelete(boxMesh);
	boxColliders.clear();

	//�J�v�Z�����b�V���̍폜
	SafeDelete(capsuleMesh);
	capsuleColliders.clear();
}

void CollideManager::Collide()
{
	//1��ڂ����ʂ�Ȃ��悤�ɂ���
	if (!firstUpdate)
	{
		firstUpdate = true;
		return;
	}

	//�����蔻����s��

	Physics::IntersectionResult result;

	//Sphere
	for (SphereCollider* colliderA : sphereColliders)
	{
		for (SphereCollider* colliderB : sphereColliders)
		{
			if (!colliderA->GetEnable())continue;
			if (!colliderB->GetEnable())continue;
			if (colliderA->GetParent()->GetTag() == colliderB->GetParent()->GetTag())continue;

			if (Physics::IntersectSphereVsSphere(colliderA, colliderB, &result))
			{
				if (colliderA->priority > colliderB->priority)
				{
					//colliderA�������o��
					colliderA->center += result.normal * result.penetration;
				}
				else if(colliderA->priority < colliderB->priority)
				{
					//colliderB�������o��
					colliderB->center -= result.normal * result.penetration;
				}
				else
				{
					//�����������o��
					colliderA->center += result.normal * result.penetration * 0.5f;
					colliderB->center -= result.normal * result.penetration * 0.5f;
				}
				
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

			if (capsule->GetParent()->GetTag() == sphere->GetParent()->GetTag())continue;


			if (Physics::IntersectSphereVsCapsule(sphere, capsule, &result))
			{
				if (capsule->priority > sphere->priority)
				{
					//capsule�������o��
					capsule->begin -= result.normal * result.penetration;
					capsule->end -= result.normal * result.penetration;
				}
				else if(capsule->priority < sphere->priority)
				{
					//colliderB�������o��
					sphere->center += result.normal * result.penetration;
				}
				else
				{
					//�����������o��
					capsule->begin -= result.normal * result.penetration;
					capsule->end -= result.normal * result.penetration;
					sphere->center += result.normal * result.penetration * 0.5f;
				}
				
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

	//Box�`��
	boxMesh->DrawPrepare();
	for (BoxCollider* collider : boxColliders)
	{
		if (collider->GetEnable() && collider->drawDebugPrimitive)
			boxMesh->Draw(collider);
	}

	//�J�v�Z���`��
	capsuleMesh->DrawPrepare();
	for (CapsuleCollider* collider : capsuleColliders)
	{
		if (collider->GetEnable() && collider->drawDebugPrimitive)
		{
			collider->CalcCapsuleParam();
			capsuleMesh->Draw(collider);
		}
	}
}

void CollideManager::CreateBoundingBox(ModelRenderer* modelRenderer)
{
	//���f���f�[�^�̎擾
	ModelData* data = modelRenderer->GetModelResource();

	//���b�V�����̎擾
	int meshCount = data->GetMeshesSize();

	for (int meshIndex = 0; meshIndex < meshCount; meshIndex++)
	{
		//���_���̎擾
		int vertexCount = data->GetVerticesSize(meshIndex);

		DirectX::XMFLOAT3 min{ +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX };
		DirectX::XMFLOAT3 max{ -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX };

		//�ő�A�ŏ��ʒu�̒��_���Z�o
		for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++)
		{
			//���_�̎擾
			ModelData::Vertex& vertex = data->GetVertex(meshIndex, vertexIndex);

			min.x = std::min<float>(min.x, vertex.position.x);
			min.y = std::min<float>(min.y, vertex.position.y);
			min.z = std::min<float>(min.z, vertex.position.z);
								   	   
			max.x = std::max<float>(max.x, vertex.position.x);
			max.y = std::max<float>(max.y, vertex.position.y);
			max.z = std::max<float>(max.z, vertex.position.z);
		}

		//���[���h�s��̍쐬
		Transform* transform = modelRenderer->GetParent()->GetComponent<Transform>();
		transform->Update();

		DirectX::XMFLOAT4X4 world = data->GetMeshGlobalTransform(meshIndex) * transform->world;

		//�Z�o�������_�����[���h��Ԃɕϊ�
		min = min * world;
		max = max * world;
		
		//�{�b�N�X�̍쐬
		BoxCollider* box = new BoxCollider("Box" + std::to_string(meshIndex));
		box->size = max - min;
		box->center = min + box->size * 0.5f;

		modelRenderer->GetParent()->AddComponent(box);
	}
}
