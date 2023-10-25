#pragma once

#include <DirectXMath.h>

#include "PrimitiveMeshRenderer.h"

class SphereCollider;

class SphereMeshRenderer : public PrimitiveMeshRenderer
{
public:
	SphereMeshRenderer();
	~SphereMeshRenderer() override = default;

	//-----< ŠÖ” >-----//

	//‰Šú‰»
	void Initialize()override;
	//•`‰æ€”õ
	void DrawPrepare();
	//•`‰æ
	void Draw(SphereCollider* sphereCollider);

	//-----< Getter, Setter >-----//

	//-----< \‘¢‘Ì >-----//

private:
	//-----< •Ï” >-----//

};