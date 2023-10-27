

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
		ImGui::DragFloat3("center", &center.x);
		radian = QuaternionToEuler(quaternion);
		ImGui::DragFloat3("radian", &radian.x);
		quaternion = EulerToQuaternion(radian);
		ImGui::DragFloat("radius", &radius);
		ImGui::DragFloat("cylinderSize", &cylinderSize);
		Collider::DebugGui();
		ImGui::TreePop();
	}
}

void CapsuleCollider::CalcCapsuleParam(DirectX::XMFLOAT3 begin, DirectX::XMFLOAT3 end)
{
	XMFLOAT3 vec = end - begin;//begin->end
	float length = LengthFloat3(vec);
	vec = NormalizeFloat3(vec);

	XMFLOAT3 up = { 0.0f,1.0f,0.0f };

	XMFLOAT3 axis = CrossFloat3(up, vec);

	float dot = DotFloat3(up, vec);

	float rad = cosf(dot);

	XMVECTOR Q = XMQuaternionRotationAxis(XMLoadFloat3(&axis), rad);

	Q = XMQuaternionMultiply(XMLoadFloat4(&quaternion), Q);

	XMStoreFloat4(&quaternion, Q);
	radian = QuaternionToEuler(quaternion);

	cylinderSize = length;
}
