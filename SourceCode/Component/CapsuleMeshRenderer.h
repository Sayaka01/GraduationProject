#pragma once

#include <DirectXMath.h>

#include "PrimitiveMeshRenderer.h"

class CapsuleCollider;

class CapsuleMeshRenderer : public PrimitiveMeshRenderer
{
public:
	CapsuleMeshRenderer();
	~CapsuleMeshRenderer() override = default;

	//-----< 関数 >-----//

	//初期化
	void Initialize()override;
	//描画準備
	void DrawPrepare();
	//描画
	void Draw(CapsuleCollider* capsuleCollider);

private:
	//-----< 変数 >-----//
	Microsoft::WRL::ComPtr <ID3D11Buffer> cylinderVertexBuffer;//円柱の頂点バッファ
	Microsoft::WRL::ComPtr <ID3D11Buffer> halfSphereUpVertexBuffer;//球（上）の頂点バッファ
	Microsoft::WRL::ComPtr <ID3D11Buffer> halfSphereDownVertexBuffer;//球（下）の頂点バッファ

	UINT cylinderVertexCount = 0;//円柱の頂点数
	UINT halfSphereVertexCount = 0;//球の頂点数（半分）

};