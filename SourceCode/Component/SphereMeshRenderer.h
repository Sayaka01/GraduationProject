#pragma once

#include <DirectXMath.h>

#include "PrimitiveMeshRenderer.h"

class SphereCollider;

class SphereMeshRenderer : public PrimitiveMeshRenderer
{
public:
	SphereMeshRenderer();
	~SphereMeshRenderer() override = default;

	//-----< �֐� >-----//

	//������
	void Initialize()override;
	//�`�揀��
	void DrawPrepare();
	//�`��
	void Draw(SphereCollider* sphereCollider);

	//-----< Getter, Setter >-----//

	//-----< �\���� >-----//

private:
	//-----< �ϐ� >-----//

};