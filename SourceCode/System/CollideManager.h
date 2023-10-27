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
	//CapsuleColliderの追加
	void AddCapsuleCollider(CapsuleCollider* capsuleCollider) { capsuleColliders.emplace_back(capsuleCollider); }

private:
	SphereMeshRenderer* sphereMesh = nullptr;
	std::vector<SphereCollider*> sphereColliders;
	CapsuleMeshRenderer* capsuleMesh = nullptr;
	std::vector<CapsuleCollider*> capsuleColliders;
};