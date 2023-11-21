

#include "Stdafx.h"
#include "CapsuleCollider.h"

#include "Component/Camera.h"

#include "GameObject/GameObject.h"

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
	CollideManager::Instance().RemoveCapsuleCollider(this);
}

void CapsuleCollider::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		ImGui::DragFloat3("center", &center.x);

		ImGui::DragFloat3("begin", &begin.x);
		ImGui::DragFloat3("end", &end.x);

		//radian = QuaternionToEuler(quaternion);
		ImGui::DragFloat3("radian", &radian.x);
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

	DirectX::XMFLOAT3 axis = CrossFloat3(oldVec, vec);
	axis = NormalizeFloat3(axis);
	if (LengthFloat3(axis) < FLT_EPSILON)return;

	float dot = DotFloat3(oldVec, vec);

	float rad = acosf(dot);
	if (rad < FLT_EPSILON)return;

	////カメラの取得
	//Camera* camera = parent->GetParent()->GetChild("CameraController")->GetComponent<Camera>();
	////カメラの右方向ベクトルを取得
	//DirectX::XMFLOAT3 cameraRight = camera->GetCameraRight();
	//cameraRight.y = 0.0f;
	////カメラの右ベクトルと内積を取り左右判定を行う
	//if (DotFloat3(NormalizeFloat3(cameraRight), axis) >= 0.0f)rad = -rad;

	//クオータニオンの作成
	DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat3(&axis), rad);

	Q = DirectX::XMQuaternionMultiply(DirectX::XMLoadFloat4(&quaternion), Q);

	DirectX::XMStoreFloat4(&quaternion, Q);
	radian = QuaternionToEuler(quaternion);

	cylinderSize = length;

	oldVec = vec;

	//UpdateCenter();
}

void CapsuleCollider::UpdateCenter()
{
	XMFLOAT3 vec = end - begin;//begin->end
	float length = LengthFloat3(vec);
	vec = NormalizeFloat3(vec);

	center = begin + vec * length * 0.5f;
}
