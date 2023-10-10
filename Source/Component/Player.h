#pragma once

#include <DirectXMath.h>

#include "Component.h"

class Player : public Component
{
public:
	Player() = default;
	~Player() override = default;

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


};