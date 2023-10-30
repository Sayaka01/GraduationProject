#pragma once

#include <DirectXMath.h>

#include "PrimitiveMeshRenderer.h"

class BoxCollider;

class BoxMeshRenderer : public PrimitiveMeshRenderer
{
public:
	BoxMeshRenderer();
	~BoxMeshRenderer() override = default;

	//-----< �֐� >-----//

	//������
	void Initialize()override;
	//�`�揀��
	void DrawPrepare();
	//�`��
	void Draw(BoxCollider* boxCollider);

private:
	//-----< �ϐ� >-----//

};