#pragma once

#include <DirectXMath.h>

#include "Component.h"

class ModelRenderer : public Component
{
public:
	ModelRenderer() = default;
	~ModelRenderer() override = default;

	//-----< 関数 >-----//

	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//ImGui
	void DebugGui() override;

	//-----< Getter, Setter >-----//



	//-----< 変数 >-----//



};