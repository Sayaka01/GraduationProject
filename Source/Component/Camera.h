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