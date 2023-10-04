#pragma once

#include <DirectXMath.h>

#include "Component.h"

class ModelRenderer : public Component
{
public:
	ModelRenderer() = default;
	~ModelRenderer() override = default;

	//-----< �֐� >-----//

	//������
	void Initialize() override;
	//�X�V
	void Update() override;
	//�`��
	void Draw() override;
	//ImGui
	void DebugGui() override;

	//-----< Getter, Setter >-----//



	//-----< �ϐ� >-----//



};