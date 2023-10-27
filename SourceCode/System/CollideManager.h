#pragma once

#include <vector>

class SphereMeshRenderer;
class SphereCollider;
class CapsuleMeshRenderer;
class CapsuleCollider;

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
	//CapsuleCollider�̒ǉ�
	void AddCapsuleCollider(CapsuleCollider* capsuleCollider) { capsuleColliders.emplace_back(capsuleCollider); }

private:
	SphereMeshRenderer* sphereMesh = nullptr;
	std::vector<SphereCollider*> sphereColliders;
	CapsuleMeshRenderer* capsuleMesh = nullptr;
	std::vector<CapsuleCollider*> capsuleColliders;
};