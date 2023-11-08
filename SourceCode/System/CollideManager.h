#pragma once

#include <vector>

class SphereMeshRenderer;
class SphereCollider;
class BoxMeshRenderer;
class BoxCollider;
class CapsuleMeshRenderer;
class CapsuleCollider;

class ModelRenderer;

class CollideManager
{
private:
	CollideManager() {}
	~CollideManager() {}

public:

	//�C���X�^���X�̎擾
	static CollideManager& Instance()
	{
		static CollideManager instance;
		return instance;
	}

	//������
	void Initialize();
	//�I������
	void Finalize();
	//�����蔻��
	void Collide();
	//�`��
	void Draw();

	//SphereCollider�̒ǉ�
	void AddSphereCollider(SphereCollider* sphereCollider) { sphereColliders.emplace_back(sphereCollider); }
	//BoxCollider�̒ǉ�
	void AddBoxCollider(BoxCollider* boxCollider) { boxColliders.emplace_back(boxCollider); }
	//CapsuleCollider�̒ǉ�
	void AddCapsuleCollider(CapsuleCollider* capsuleCollider) { capsuleColliders.emplace_back(capsuleCollider); }

	//BoundingBox�̍쐬
	void CreateBoundingBox(ModelRenderer* modelRenderer);

private:
	SphereMeshRenderer* sphereMesh = nullptr;
	std::vector<SphereCollider*> sphereColliders;
	BoxMeshRenderer* boxMesh = nullptr;
	std::vector<BoxCollider*> boxColliders;
	CapsuleMeshRenderer* capsuleMesh = nullptr;
	std::vector<CapsuleCollider*> capsuleColliders;

	bool firstUpdate = false;
};