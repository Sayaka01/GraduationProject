#pragma once

#include <DirectXMath.h>

#include "PrimitiveMeshRenderer.h"

class BoxCollider;

class BoxMeshRenderer : public PrimitiveMeshRenderer
{
public:
	BoxMeshRenderer();
	~BoxMeshRenderer() override = default;

	//-----< 関数 >-----//

	//初期化
	void Initialize()override;
	//描画準備
	void DrawPrepare();
	//描画
	void Draw(BoxCollider* boxCollider);

private:
	//-----< 変数 >-----//

};