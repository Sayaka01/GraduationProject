#pragma once

#include <vector>

class SphereMeshRenderer;
class SphereCollider;

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

private:
	SphereMeshRenderer* sphereMesh = nullptr;
	std::vector<SphereCollider*> sphereColliders;
};