#pragma once

#include <DirectXMath.h>

#include "PrimitiveMeshRenderer.h"

class SphereCollider;

class SphereMeshRenderer : public PrimitiveMeshRenderer
{
public:
	SphereMeshRenderer();
	~SphereMeshRenderer() override = default;

	//-----< 関数 >-----//

	//初期化
	void Initialize()override;
	//描画準備
	void DrawPrepare();
	//描画
	void Draw(SphereCollider* sphereCollider);

private:
	//-----< 変数 >-----//

};