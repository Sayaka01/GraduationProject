
#include "Stdafx.h"
#include "Player.h"

void Player::Initialize()
{

}

void Player::Update()
{

}

void Player::Draw()
{

}

void Player::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{

		ImGui::TreePop();
	}
}