

#include "Stdafx.h"
#include "CapsuleCollider.h"

#include "System/CollideManager.h"
#include "System/UserFunction.h"

CapsuleCollider::CapsuleCollider()
{
	CollideManager::Instance().AddCapsuleCollider(this);

	name = "CapsuleCollider";
}

CapsuleCollider::CapsuleCollider(std::string compName)
{
	CollideManager::Instance().AddCapsuleCollider(this);

	name = compName;

	quaternion = EulerToQuaternion(radian);
}

CapsuleCollider::~CapsuleCollider()
{
}

void CapsuleCollider::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::DragFloat3("begin", &begin.x);
		ImGui::DragFloat3("end", &end.x);

		//radian = QuaternionToEuler(quaternion);
		//ImGui::DragFloat3("radian", &radian.x);
		//quaternion = EulerToQuaternion(radian);
		
		ImGui::DragFloat("radius", &radius);

		ImGui::DragFloat("cylinderSize", &cylinderSize);

		Collider::DebugGui();
		ImGui::TreePop();
	}
}

void CapsuleCollider::CalcCapsuleParam()
{
	DirectX::XMFLOAT3 vec = end - begin;//begin->end
	float length = LengthFloat3(vec);
	vec = NormalizeFloat3(vec);

	DirectX::XMFLOAT3 up = { 0.0f,1.0f,0.0f };

	DirectX::XMFLOAT3 axis = CrossFloat3(up, vec);
	axis = NormalizeFloat3(axis);
	if (LengthFloat3(axis) < FLT_EPSILON)return;

	float dot = DotFloat3(up, vec);

	float rad = cosf(dot);
	if (rad < FLT_EPSILON)return;

	DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat3(&axis), rad);

	Q = DirectX::XMQuaternionMultiply(DirectX::XMLoadFloat4(&quaternion), Q);

	DirectX::XMStoreFloat4(&quaternion, Q);
	radian = QuaternionToEuler(quaternion);

	cylinderSize = length;

	UpdateCenter();
}

void CapsuleCollider::UpdateCenter()
{
	XMFLOAT3 vec = end - begin;//begin->end
	float length = LengthFloat3(vec);
	vec = NormalizeFloat3(vec);

	center = begin + vec * length * 0.5f;
}
