
#include "Stdafx.h"

#include "PlayerStates.h"

#include "GameObject/GameObject.h"

#include "Component/ModelRenderer.h"

#include "System/SystemManager.h"
#include "System/UserFunction.h"

using namespace PlayerState;

//�ҋ@
Idle::Idle()
{
	name = "Idle";
}
Idle::Idle(GameObject* parent)
{
	name = "Idle";
	this->parent = parent;
}
void Idle::Enter()
{
	parent->GetComponent<ModelRenderer>()->PlayAnimation(5, true);
}
void Idle::Update()
{

}
void Idle::Exit()
{

}
std::string Idle::Judge()
{
	//L�X�e�B�b�N�̓��͒l���擾
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	DirectX::XMFLOAT2 lStickVec;
	lStickVec = { gamePad.GetAxisLX() ,gamePad.GetAxisLY() };

	//L�X�e�B�b�N�̓��͂�����Ȃ瑖��X�e�[�g�֑J��
	if (LengthFloat2(lStickVec) > 0.0f)
	{
		return "Run";
	}
	
	//�ύX�Ȃ�
	return "";
}

//����
Run::Run()
{
	name = "Run";
}
Run::Run(GameObject* parent)
{
	name = "Run";
	this->parent = parent;
}
void Run::Enter()
{

}
void Run::Update()
{

}
void Run::Exit()
{

}
std::string Run::Judge()
{
	//L�X�e�B�b�N�̓��͒l���擾
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	DirectX::XMFLOAT2 lStickVec;
	lStickVec = { gamePad.GetAxisLX() ,gamePad.GetAxisLY() };

	//L�X�e�B�b�N�̓��͂��Ȃ��Ȃ�ҋ@�X�e�[�g�֑J��
	if (LengthFloat2(lStickVec) < FLT_EPSILON)
	{
		return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}