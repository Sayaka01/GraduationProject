#pragma once

#include <DirectXMath.h>

#include "PrimitiveMeshRenderer.h"

class BoxCollider;

class BoxMeshRenderer : public PrimitiveMeshRenderer
{
public:
	BoxMeshRenderer();
	~BoxMeshRenderer() override = default;

	//-----< ŠÖ” >-----//

	//‰Šú‰»
	void Initialize()override;
	//•`‰æ€”õ
	void DrawPrepare();
	//•`‰æ
	void Draw(BoxCollider* boxCollider);

private:
	//-----< •Ï” >-----//

};