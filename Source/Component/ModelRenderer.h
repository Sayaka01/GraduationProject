#pragma once

#include <DirectXMath.h>

#include "Component.h"

class ModelRenderer : public Component
{
public:
	ModelRenderer() = default;
	~ModelRenderer() override = default;

	//-----< ŠÖ” >-----//

	//‰Šú‰»
	void Initialize() override;
	//XV
	void Update() override;
	//•`‰æ
	void Draw() override;
	//ImGui
	void DebugGui() override;

	//-----< Getter, Setter >-----//



	//-----< •Ï” >-----//



};