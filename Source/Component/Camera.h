#pragma once

#pragma once

#include <DirectXMath.h>

#include "Component.h"

#include "System/ConstantBuffer.h"

class Camera : public Component
{
public:
	Camera() = default;
	~Camera() override = default;

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