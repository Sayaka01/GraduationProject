#pragma once

#include <DirectXMath.h>

#include "Component.h"

class Player : public Component
{
public:
	Player() = default;
	~Player() override = default;

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


};