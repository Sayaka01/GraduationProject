#include "Stdafx.h"
#include "DirectionLight.h"

#include "System/SystemManager.h"


DirectionLight::DirectionLight()
{
	name = "DirectionLight";
	constantBuffer.Initialize(SystemManager::Instance().GetDevice() ,&lightConstants);
}

void DirectionLight::Draw3D()
{
	UpdateBuffer();
}

void DirectionLight::UpdateBuffer()
{
	lightConstants.ambientColor = ambientColor;
	lightConstants.lightDirection = lightDirection;
	lightConstants.lightColor = lightColor;

	//定数バッファ
	constantBuffer.SetConstantBuffer(SystemManager::Instance().GetDeviceContext(), ConstantBuffer::ShaderType::ALL, ConstantBuffer::UsageType::DirectionLight, &lightConstants);
}

void DirectionLight::DebugGui()
{

#ifdef USE_IMGUI

	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::DragFloat4("ambientColor", &ambientColor.x, 0.01f);
		ImGui::DragFloat4("lightDirection", &lightDirection.x, 0.01f);
		ImGui::DragFloat4("lightColor", &lightColor.x, 0.01f);

		ImGui::TreePop();
	}
#endif
}
