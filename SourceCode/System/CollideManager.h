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

	//インスタンスの取得
	static CollideManager& Instance()
	{
		static CollideManager instance;
		return instance;
	}

	//初期化
	void Initialize();
	//終了処理
	void Finalize();
	//当たり判定
	void Collide();
	//描画
	void Draw();

	//SphereColliderの追加
	void AddSphereCollider(SphereCollider* sphereCollider) { sphereColliders.emplace_back(sphereCollider); }
	//BoxColliderの追加
	void AddBoxCollider(BoxCollider* boxCollider) { boxColliders.emplace_back(boxCollider); }
	//CapsuleColliderの追加
	void AddCapsuleCollider(CapsuleCollider* capsuleCollider) { capsuleColliders.emplace_back(capsuleCollider); }

	//BoundingBoxの作成
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