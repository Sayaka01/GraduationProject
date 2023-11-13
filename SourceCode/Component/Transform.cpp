
#include <Stdafx.h>
#include "Transform.h"

//èâä˙âª
void Transform::Initialize()
{
	name = "Transform";
	coordinateSystem = CoordinateSystem::RHS_YUP;
}

//çXêV
void Transform::Update()
{
	//çsóÒçÏê¨
	const DirectX::XMMATRIX S{ DirectX::XMMatrixScaling(scale.x,scale.y,scale.z) };
	const DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&orientation));
	const DirectX::XMMATRIX T{ DirectX::XMMatrixTranslation(pos.x,pos.y,pos.z) };
	const DirectX::XMMATRIX C = DirectX::XMLoadFloat4x4(&GetCoordinateSystemTransform());
	DirectX::XMStoreFloat4x4(&world, C * S * R * T);
}

//ï`âÊ
void Transform::Draw()
{
	
}

//ImGui
void Transform::DebugGui()
{
	if(ImGui::TreeNode(name.c_str()))
	{
		ImGui::DragFloat3("pos", &pos.x);
		ImGui::DragFloat3("scale", &scale.x);
		ImGui::DragFloat4("orientation", &orientation.x);
		if (ImGui::Button("ResetPosition"))
		{
			pos = { 0.0f,0.0f,0.0f };
		}
		ImGui::TreePop();
	}
}

DirectX::XMFLOAT4X4 Transform::GetCoordinateSystemTransform(float scaleFactor)
{
	const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
		   {-1,0,0,0,   0,1,0,0,   0,0,1,0,   0,0,0,1},    // 0:RHS Y-UP
		   { 1,0,0,0,   0,1,0,0,   0,0,1,0,   0,0,0,1},	// 1:LHS Y-UP
		   {-1,0,0,0,   0,0,-1,0,  0,1,0,0,   0,0,0,1},	// 2:RHS Z-UP
		   { 1,0,0,0,   0,0,1,0,   0,1,0,0,   0,0,0,1},	// 3:LHS Z-UP
	};
	// To change the units from centimeters to meters, set 'scale_factor' to 0.01.
	DirectX::XMFLOAT4X4 mat{};
	DirectX::XMStoreFloat4x4(&mat, DirectX::XMMATRIX{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[static_cast<int>(coordinateSystem)])
		* DirectX::XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor) });

	return mat;
}