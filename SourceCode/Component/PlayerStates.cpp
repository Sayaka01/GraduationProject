
#include "Stdafx.h"

#include "PlayerStates.h"

#include "GameObject/GameObject.h"

#include "Component/ModelRenderer.h"

#include "System/SystemManager.h"
#include "System/UserFunction.h"

using namespace PlayerState;

//待機
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
	//Lスティックの入力値を取得
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	DirectX::XMFLOAT2 lStickVec;
	lStickVec = { gamePad.GetAxisLX() ,gamePad.GetAxisLY() };

	//Lスティックの入力があるなら走りステートへ遷移
	if (LengthFloat2(lStickVec) > 0.0f)
	{
		return "Run";
	}
	
	//変更なし
	return "";
}

//走り
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
	//Lスティックの入力値を取得
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	DirectX::XMFLOAT2 lStickVec;
	lStickVec = { gamePad.GetAxisLX() ,gamePad.GetAxisLY() };

	//Lスティックの入力がないなら待機ステートへ遷移
	if (LengthFloat2(lStickVec) < FLT_EPSILON)
	{
		return "Idle";
	}

	//変更なし
	return "";
}