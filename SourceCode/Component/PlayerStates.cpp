
#include "Stdafx.h"

#include "PlayerStates.h"

#include "GameObject/GameObject.h"

#include "Component/Transform.h"
#include "Component/ModelRenderer.h"
#include "Component/SpriteRenderer.h"
#include "Component/Camera.h"
#include "Component/RigidBody.h"
#include "Component/Player.h"
#include "Component/SphereCollider.h"
#include "Component/CapsuleCollider.h"

#include "System/SystemManager.h"
#include "System/UserFunction.h"


using namespace PlayerState;
using namespace DirectX::SimpleMath;

//static�ϐ��̏�����
float Default::attackInterval = 0.0f;
bool Default::acceptAttackButton = false;
bool Default::pushAttackButton = false;
int Default::jumpCount = 0;
DirectX::XMFLOAT3 Default::parameter = { 0.0f,0.0f,0.0f };
bool Default::isAvoid = false;

//-----< ���N���X >-----//
void PlayerState::Default::Update()
{
	//�U���{�^����������Ă��邩�ǂ���
	if (acceptAttackButton && attackInterval > FLT_EPSILON)
	{
		//�Q�[���p�b�h�̎擾
		GamePad gamePad = SystemManager::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_X)
		{
			pushAttackButton = true;
		}
	}

	attackInterval += SystemManager::Instance().GetElapsedTime();

	if (attackInterval > acceptAttackTime)
	{
		parent->GetComponent<Player>()->InitAttackPhase();

		if(pushAttackButton)pushAttackButton = false;
	}

	//�^�[�Q�b�g�X�v���C�g�̈ʒu�v�Z
	SearchThrowObj();
}

void Default::SetMoveVelocity(DirectX::XMFLOAT3 velocity)
{
	//RigidBody�R���|�[�l���g��MoveVelocity��ݒ�
	parent->GetComponent<RigidBody>()->SetVelocity(velocity);
}
void PlayerState::Default::AddMoveVelocity(DirectX::XMFLOAT3 velocity)
{
	//RigidBody�R���|�[�l���g��MoveVelocity��ǉ�
	parent->GetComponent<RigidBody>()->AddVelocity(velocity);
}
DirectX::XMFLOAT3 Default::GetCameraFront()
{
	//�J�����̎擾
	Camera* camera = parent->GetParent()->GetChild("CameraController")->GetComponent<Camera>();

	//�J�����̑O�����x�N�g�����擾
	DirectX::XMFLOAT3 cameraFront = camera->GetCameraFront();
	cameraFront.y = 0.0f;

	return NormalizeFloat3(cameraFront);
}
DirectX::XMFLOAT3 PlayerState::Default::GetCameraRight()
{
	//�J�����̎擾
	Camera* camera = parent->GetParent()->GetChild("CameraController")->GetComponent<Camera>();

	//�J�����̉E�����x�N�g�����擾
	DirectX::XMFLOAT3 cameraRight = camera->GetCameraRight();
	cameraRight.y = 0.0f;

	return NormalizeFloat3(cameraRight);
}
DirectX::XMFLOAT3 Default::CalcMoveVec()
{
	//�J�����̑O�����x�N�g�����擾
	DirectX::XMFLOAT3 cameraFront = GetCameraFront();
	//�J�����̉E�����x�N�g�����擾
	DirectX::XMFLOAT3 cameraRight = GetCameraRight();

	//L�X�e�B�b�N�̓��͒l���擾
	DirectX::XMFLOAT2 lStickVec = GetLStickVec();
	//�J�����̃x�N�g�����琅�������̈ړ��x�N�g�����Z�o
	DirectX::XMFLOAT3 vec =
	{
		cameraFront.x * lStickVec.y + cameraRight.x * lStickVec.x,
		0.0f,
		cameraFront.z * lStickVec.y + cameraRight.z * lStickVec.x,
	};

	return NormalizeFloat3(vec);
}
DirectX::XMFLOAT2 Default::GetLStickVec()
{
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	return DirectX::XMFLOAT2(gamePad.GetAxisLX() ,gamePad.GetAxisLY());
}
void Default::YAxisRotate(DirectX::XMFLOAT3 moveVelocity)
{
	//�����̈ړ��x�N�g�����[���x�N�g���̏ꍇ�͉�]�������s��Ȃ�
	moveVelocity.y = 0.0f;
	const float length = LengthFloat3(moveVelocity);
	if (length < FLT_EPSILON)return;

	//�ړ��x�N�g���𐳋K������
	moveVelocity = NormalizeFloat3(moveVelocity);

	//��]�������߂�
	DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);

	//player�̃��[���h�s��
	DirectX::XMFLOAT4X4 world = parent->GetComponent<Transform>()->world;

	//���g�̉�]�l����O���������߂�
	DirectX::XMFLOAT3 frontVec = NormalizeFloat3({ world._31,world._32 ,world._33 });
	up = frontVec;
	//��]�p�����߂�
	const float dot = (frontVec.x * moveVelocity.x) + (frontVec.z * moveVelocity.z);

	//��]���x�𒲐�����
	float rot = acosf(dot);

	//���E����
	float cross = (frontVec.x * moveVelocity.z) - (frontVec.z * moveVelocity.x);

	//orientation�̎擾
	DirectX::XMFLOAT4 orientation = parent->GetComponent<Transform>()->orientation;
	DirectX::XMVECTOR OldOrientation = DirectX::XMLoadFloat4(&orientation);

	DirectX::XMVECTOR Q;
	float speed = rotateSpeed * SystemManager::Instance().GetElapsedTime();
	if (speed > rot) speed = rot;
	if (cross < 0.0f)
	{
		Q = DirectX::XMQuaternionRotationAxis(Up, speed);
	}
	else
	{
		Q = DirectX::XMQuaternionRotationAxis(Up, -speed);
	}
	DirectX::XMVECTOR Orientation = DirectX::XMQuaternionMultiply(OldOrientation, Q);

	Orientation = DirectX::XMQuaternionSlerp(OldOrientation, Orientation, rotateRatio);

	DirectX::XMStoreFloat4(&orientation, Orientation);

	parent->GetComponent<Transform>()->orientation = orientation;
}
#if _APPEND
void PlayerState::Default::SearchNearEnemy(Transform* transform)
{
	transform = nullptr;

	//���͓G��1�̂Ȃ̂ł��̂����B��Ŋ撣��
	GameObject* enemyManager = parent->GetParent()->GetChild("enemyManager");
	DirectX::SimpleMath::Vector3 playerPos = parent->GetComponent<Transform>()->pos;
	if (enemyManager != nullptr)
	{
		int enemyCounut = int(enemyManager->GetChildrenCount());
		float minLength = FLT_MAX;

		if (enemyCounut > 0)
		{
			for (int i = 0; i < enemyCounut; i++)
			{
				Transform* enemyTransform = enemyManager->GetGameObj(i)->GetComponent<Transform>();
				Vector3 enemyPos= enemyTransform->pos;
				Vector3 vec = playerPos - enemyPos;
				float length = vec.Length();

				if (minLength > length)
				{
					minLength = length;
					parameter = enemyPos;

					transform = enemyTransform;
				}
			}
			return;
		}
	}

	//�G��������Ȃ��ꍇ�͎������g�̈ʒu������
	parameter = playerPos;

	//GameObject* enemy = parent->GetParent()->GetChild("enemy");
	//parameter = enemy->GetComponent<Transform>()->pos;
}
#endif
float PlayerState::Default::GetAnimationSpeed(int index)
{
	return parent->GetComponent<Player>()->GetAnimationSpeed(index);
}
void PlayerState::Default::SetAnimationSpeed(int index, float speed)
{
	parent->GetComponent<Player>()->SetAnimationSpeed(index, speed);
}

bool PlayerState::Default::JudgeIdleState()
{
	//L�X�e�B�b�N�̓��͒l���擾
	DirectX::XMFLOAT2 lStickVec = GetLStickVec();
	//L�X�e�B�b�N�̓��͂��Ȃ��Ȃ�ҋ@�X�e�[�g�֑J��
	return (LengthFloat2(lStickVec) < FLT_EPSILON);
}
bool PlayerState::Default::JudgeRunState()
{
	//L�X�e�B�b�N�̓��͒l���擾
	DirectX::XMFLOAT2 lStickVec = GetLStickVec();
	//L�X�e�B�b�N�̓��͂�����Ȃ瑖��X�e�[�g�֑J��
	return (LengthFloat2(lStickVec) > 0.0f);
}
bool PlayerState::Default::JudgeJumpState()
{
	if (jumpCount >= maxJumpCount)return false;

	//�Q�[���p�b�h�̎擾
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	//A�{�^����������Ă�����true
	return (gamePad.GetButtonDown() & GamePad::BTN_A);
}
bool PlayerState::Default::JudgeAttackState()
{
	int phase = parent->GetComponent<Player>()->GetAttackPhase();

	if (phase == 0)
	{
		//�Q�[���p�b�h�̎擾
		GamePad gamePad = SystemManager::Instance().GetGamePad();
		//A�{�^����������Ă�����true
		return (gamePad.GetButtonDown() & GamePad::BTN_X);
	}
	else
	{
		//A�{�^����������Ă�����true
		return (pushAttackButton && (attackInterval < acceptAttackTime));
	}
}
bool PlayerState::Default::JudgeAimWireState()
{
	return false;

	//�Q�[���p�b�h�̎擾
	GamePad gamePad = SystemManager::Instance().GetGamePad();

	//ZL�{�^����������Ă�����true
	return (gamePad.GetButtonDown() & GamePad::BTN_LEFT_SHOULDER);
}
bool PlayerState::Default::JudgeSwingWireState()
{
	//�Q�[���p�b�h�̎擾
	GamePad gamePad = SystemManager::Instance().GetGamePad();

	//ZR�{�^����������Ă�����true
	return (gamePad.GetButton() & GamePad::BTN_RIGHT_SHOULDER) && (parent->GetComponent<Transform>()->pos.y > 11.5f);
}
bool PlayerState::Default::JudgeAvoidState()
{
	//�Q�[���p�b�h�̎擾
	GamePad gamePad = SystemManager::Instance().GetGamePad();
	//B�{�^����������Ă�����true
	return (gamePad.GetButtonDown() & GamePad::BTN_B);
}
bool PlayerState::Default::JudgeWieldThrowState()
{
	//�Q�[���p�b�h�̎擾
	GamePad gamePad = SystemManager::Instance().GetGamePad();

	//RT�{�^����������Ă��Ȃ����return false
	if (!(gamePad.GetButtonDown() & GamePad::BTN_RIGHT_TRIGGER))return false;

	//�v���C���[�R���|�[�l���g��ThrowObj��null���ǂ���
	return (parent->GetComponent<Player>()->GetThrowObj());
}

void PlayerState::Default::SearchThrowObj()
{
#if _APPEND
	if (name == "WieldThrow")return;
#endif

	//manager�g�̃I�u�W�F�N�g���擾
	GameObject* throwObjects = parent->GetParent()->GetChild("throwObjects");

	GameObject* throwObj = nullptr;//�^�[�Q�b�g�p�I�u�W�F�N�g
	DirectX::XMFLOAT2 nearScreenPos = { 0.0f,0.0f };
	float minScreenDist = FLT_MAX;//�ŒZ����
	float minWorldDist = FLT_MAX;//�ŒZ����

	for (size_t i = 0; i < throwObjects->GetChildrenCount(); i++)
	{
		//�I�u�W�F�N�g�̎擾
		GameObject* obj = throwObjects->GetGameObj(i);
		DirectX::XMFLOAT3 objPos = obj->GetComponent<Transform>()->pos;
		//�v���C���[�̈ʒu���擾
		DirectX::XMFLOAT3 plPos = parent->GetComponent<Transform>()->pos;
		//�v���C���[�ƃI�u�W�F�N�g�̃x�N�g�����v�Z
		DirectX::XMFLOAT3 vec = objPos - plPos;
		//�v���C���[�Ƃ̋������v�Z
		float plObjDist = LengthFloat3(vec);

		//�������T�[�`�͈͂𒴂���Ȃ�continue
		if (plObjDist > searchRange)continue;


		//�J�����̎擾
		Camera* camera = parent->GetParent()->GetChild("CameraController")->GetComponent<Camera>();
		//�J�����̑O�����x�N�g�����擾
		DirectX::XMFLOAT3 cameraFront = camera->GetCameraFront();
		//�J�����̈ʒu���擾
		DirectX::XMFLOAT3 cameraEye = camera->GetEye();
		//�J�����̈ʒu����I�u�W�F�N�g�ւ̃x�N�g�����쐬
		vec = objPos - cameraEye;

		//�J�����̕����ɃI�u�W�F�N�g�����邩�ǂ����v�Z
		float dot = DotFloat3(cameraFront, vec);
		if (dot < 0.0f)continue;

		//�I�u�W�F�N�g�̈ʒu���X�N���[�����W�n�ɕϊ�����
		ID3D11DeviceContext* dc = SystemManager::Instance().GetDeviceContext();
		DirectX::XMFLOAT2 screenPos = GetScreenPosition(dc, objPos, camera->GetProjection(), camera->GetView());

		//��ʊO�ɂ��邩�ǂ���
		float radius = obj->GetComponent<SphereCollider>()->radius;
		DirectX::XMFLOAT2 minSearchPos = { radius,radius };
		DirectX::XMFLOAT2 maxSearchPos = { (float)SCREEN_WIDTH - radius,(float)SCREEN_HEIGHT - radius };
		if (minSearchPos.x < screenPos.x && minSearchPos.y < screenPos.y)
		{
			if (maxSearchPos.x > screenPos.x && maxSearchPos.y > screenPos.y)
			{
				//��ʒ����ʒu
				DirectX::XMFLOAT2 screenCenter = { (float)SCREEN_WIDTH * 0.5f,(float)SCREEN_HEIGHT * 0.5f };
				//��ʒ����Ƃ̋������v�Z
				float distance = LengthFloat2(screenCenter - screenCenter);
				//�ŒZ�����Ȃ�ۑ�
				if (distance < minScreenDist)
				{
					minScreenDist = distance;
					minWorldDist = plObjDist;
					nearScreenPos = screenPos;
					throwObj = obj;
				}
			}
		}
	}

	//�v�Z�����ꏊ�ɃX�v���C�g��\��
	if (throwObj)
	{
		parent->GetComponent<SpriteRenderer>()->SetEnable(true);
		parent->GetComponent<SpriteRenderer>()->pos = nearScreenPos;
		float defaultDistance = 50.0f;
		float scale = (defaultDistance - minWorldDist) / searchRange;
		parent->GetComponent<SpriteRenderer>()->scale = { 1.1f + scale, 1.1f + scale };

		parent->GetComponent<Player>()->SetThrowObj(throwObj);
	}
	else
	{
		parent->GetComponent<SpriteRenderer>()->SetEnable(false);
		parent->GetComponent<Player>()->SetThrowObj(nullptr);
	}
}

//-----< �ҋ@ >-----//
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
	//�A�j���[�V�����̕ύX
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Idle, true);
	//�A�j���[�V�����X�s�[�h�̒���
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Idle));
	
	jumpCount = 0;
}
void Idle::Update()
{
	Default::Update();
}
void Idle::Exit()
{

}
std::string Idle::GetNext()
{
	//�U���X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAttackState())
	{
		int phase = parent->GetComponent<Player>()->GetAttackPhase();
		
		switch (phase)
		{
		case 0://�p���`�i�E�j�X�e�[�g�֑J�ڂł��邩
			return "PunchRight";
		case 1://�p���`�i���j�X�e�[�g�֑J�ڂł��邩
			return "PunchLeft";
		case 2://�L�b�N�X�e�[�g�ɑJ�ڂł��邩
			return "Kick";
		}

	}

	//���C���[�ł̒����ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAimWireState())
	{
		return "AimWire";
	}

	//�W�����v�X�e�[�g�֑J�ڂł��邩
	if (JudgeJumpState())
	{
		return "Jump";
	}

	//����X�e�[�g�֑J�ڂł��邩
	if (JudgeRunState())
	{
		return "Run";
	}

	//����X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAvoidState())
	{
		return "Avoid";
	}

	if (JudgeWieldThrowState())
	{
		return "WieldThrow";
	}
	
	//�ύX�Ȃ�
	return "";
}

//-----< ���� >-----//
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
	//�A�j���[�V�����̐؂�ւ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Running, true);
	//�A�j���[�V�����X�s�[�h�̒���
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Running));
}
void Run::Update()
{
	DirectX::XMFLOAT3 moveVelocity = CalcMoveVec();
	moveVelocity *= parent->GetComponent<Player>()->GetRunSpeed();

	AddMoveVelocity(moveVelocity);
	YAxisRotate(moveVelocity);

	DirectX::XMFLOAT3 p = parent->GetComponent<Transform>()->pos;
	p.y += 5.0f;
	parent->GetComponent<SphereCollider>("DebugSphere")->center = p;

	Default::Update();
}
void Run::Exit()
{

}
std::string Run::GetNext()
{
	//�U���X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAttackState())
	{
		int phase = parent->GetComponent<Player>()->GetAttackPhase();

		switch (phase)
		{
		case 0://�p���`�i�E�j�X�e�[�g�֑J�ڂł��邩
			return "PunchRight";
		case 1://�p���`�i���j�X�e�[�g�֑J�ڂł��邩
			return "PunchLeft";
		case 2://�L�b�N�X�e�[�g�ɑJ�ڂł��邩
			return "Kick";
		}

	}

	//����X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAvoidState())
	{
		return "Avoid";
	}

	//���C���[�ł̒����ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAimWireState())
	{
		return "AimWire";
	}

	//�W�����v�X�e�[�g�֑J�ڂł��邩
	if (JudgeJumpState())
	{
		return "Jump";
	}

	//�ҋ@�X�e�[�g�֑J�ڂł��邩�ǂ���
	if (JudgeIdleState())
	{
		return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< �W�����v >-----//
Jump::Jump()
{
	name = "Jump";
}
Jump::Jump(GameObject* parent)
{
	name = "Jump";
	this->parent = parent;
}
void Jump::Enter()
{
	jumpCount++;
	if (jumpCount <= 1)//�W�����v����
	{
		//�A�j���[�V�����̐؂�ւ�
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Falling, true);
		//�A�j���[�V�����X�s�[�h�̒���
		parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Falling));
	}
	else//����ȍ~
	{
		//�A�j���[�V�����̐؂�ւ�
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::JumpFlip, false);
		//�A�j���[�V�����X�s�[�h�̒���
		parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::JumpFlip));
	}

	parent->GetComponent<RigidBody>()->Jump(parent->GetComponent<Player>()->GetJumpSpeed());

	parent->GetComponent<Player>()->InitAttackPhase();
}
void Jump::Update()
{
	DirectX::XMFLOAT3 moveVelocity = CalcMoveVec();
	moveVelocity *= parent->GetComponent<Player>()->GetRunSpeed();

	AddMoveVelocity(moveVelocity);
	YAxisRotate(moveVelocity);

	if (jumpCount > 1)
	{
		//���̈ʒu�̃��[�V�����ɂ��ړ����~
		parent->GetComponent<ModelRenderer>()->StopMotionVelocity("FallingHip");
	}

	Default::Update();
}
void Jump::Exit()
{

}
std::string Jump::GetNext()
{
	//�󒆍U���X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAttackState())
	{
		return "JumpAttack";
	}

	//����X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAvoidState() && !isAvoid)
	{
		return "AvoidJump";
	}

	//���C���[�ł̌ʂ��������ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeSwingWireState())
	{
		return "SwingWire";
	}

	//���C���[�ł̒����ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAimWireState())
	{
		return "AimWire";
	}

	//�W�����v�X�e�[�g�֑J�ڂł��邩
	if (JudgeJumpState())
	{
		return "Jump";
	}

	//�A�j���[�V�����Đ����I������痎���X�e�[�g�֑J��
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Falling";
	}

	//pos.y < 0.0f �Ȃ璅�n�X�e�[�g��
	if (parent->GetComponent<Transform>()->pos.y < 0.0f)
	{
		return "Landing";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< ���� >-----//
Falling::Falling()
{
	name = "Falling";
}
Falling::Falling(GameObject* parent)
{
	name = "Falling";
	this->parent = parent;
}
void Falling::Enter()
{
	//�A�j���[�V�����̐؂�ւ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Falling, true);
	//�A�j���[�V�����X�s�[�h�̒���
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Falling));

	//�G�ւ̍U���t���O��OFF
	parent->GetComponent<Player>()->SetIsHitAttackToEnemy(false);

}
void Falling::Update()
{
	DirectX::XMFLOAT3 moveVelocity = CalcMoveVec();
	moveVelocity *= parent->GetComponent<Player>()->GetRunSpeed();

	AddMoveVelocity(moveVelocity);
	YAxisRotate(moveVelocity);

	Default::Update();
}
void Falling::Exit()
{

}
std::string Falling::GetNext()
{
	//�󒆍U���X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAttackState())
	{
		return "JumpAttack";
	}

	//����X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAvoidState() && !isAvoid)
	{
		return "AvoidJump";
	}

	//���C���[�ł̌ʂ��������ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeSwingWireState())
	{
		return "SwingWire";
	}

	//���C���[�ł̒����ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAimWireState())
	{
		return "AimWire";
	}

	//pos.y < 0.0f �Ȃ璅�n�X�e�[�g��
	if (parent->GetComponent<Transform>()->pos.y < 0.0f)
	{
		return "Landing";
	}

	//�W�����v�X�e�[�g�֑J�ڂł��邩
	if (JudgeJumpState())
	{
		return "Jump";
	}


	//�ύX�Ȃ�
	return "";
}

//-----< ���n >-----//
Landing::Landing()
{
	name = "Landing";
}
Landing::Landing(GameObject* parent)
{
	name = "Landing";
	this->parent = parent;
}
void Landing::Enter()
{
	//�A�j���[�V�����̐؂�ւ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Landing, false);
	//�A�j���[�V�����X�s�[�h�̒���
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Landing));

	jumpCount = 0;

	isAvoid = false;
}
void Landing::Update()
{
	Default::Update();
}
void Landing::Exit()
{

}
std::string Landing::GetNext()
{
	//���C���[�ł̒����ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAimWireState())
	{
		return "AimWire";
	}

	//����X�e�[�g�֑J�ڂł��邩
	if (JudgeRunState())
	{
		return "Run";
	}

	//����X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAvoidState())
	{
		return "Avoid";
	}

	//�A�j���[�V�����Đ����I�������ҋ@�X�e�[�g�֑J��
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< �p���`�i�E�j >-----//
PunchRight::PunchRight()
{
	name = "PunchRight";
}
PunchRight::PunchRight(GameObject* parent)
{
	name = "PunchRight";
	this->parent = parent;
}
void PunchRight::Enter()
{
	//�A�j���[�V�����̍Đ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::PunchRight, false);
	//�A�j���[�V�����X�s�[�h�̒���
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::PunchRight));

	//������U������̂ŏ�����
	attackInterval = 0.0f;

	//2�i�ȍ~�̍U���̓��͂̎擾���J�n
	acceptAttackButton = true;

	//�U���p�����蔻��̃R���|�[�l���g��L����
	parent->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(true);

	//�U���͂�ݒ�
	attackPower = 1.0f;

	//��ԋ߂��G�̈ʒu��parameter�Ɋi�[
	SearchNearEnemy();

	//�G�ւ̍U���t���O��OFF
	parent->GetComponent<Player>()->SetIsHitAttackToEnemy(false);

	//�U�����t���O��true��
	parent->GetComponent<Player>()->SetIsAttack(true);

	parent->GetComponent<Player>()->NextAttackPhase();
}
void PunchRight::Update()
{
	DirectX::XMFLOAT3 vec = parameter - parent->GetComponent<Transform>()->pos;
	vec.y = 0.0f;
	float length = LengthFloat3(vec);
	if (length < attackRangeMax)
	{
		YAxisRotate(vec);
		if (length > attackRangeMin)
			AddMoveVelocity(NormalizeFloat3(vec) * parent->GetComponent<Player>()->GetRunSpeed());
	}


	Default::Update();
}
void PunchRight::Exit()
{
	parent->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(false);

	//�U�����t���O��false��
	parent->GetComponent<Player>()->SetIsAttack(false);
}
std::string PunchRight::GetNext()
{

	//�A�j���[�V�����Đ����I�������
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		//�p���`�i���j�X�e�[�g�֑J�ڂł��邩
		if (JudgeAttackState())
		{
			return "PunchLeft";
		}

		//�ҋ@�X�e�[�g�֑J��
		return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< �p���`�i���j >-----//
PunchLeft::PunchLeft()
{
	name = "PunchLeft";
}
PunchLeft::PunchLeft(GameObject* parent)
{
	name = "PunchLeft";
	this->parent = parent;
}
void PunchLeft::Enter()
{
	//�A�j���[�V�����̍Đ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::PunchLeft, false);
	//�A�j���[�V�����X�s�[�h�̒���
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::PunchLeft));

	//������U������̂ŏ�����
	attackInterval = 0.0f;

	//�U���̓��̓t���O�Ȃǂ�������
	pushAttackButton = false;

	//�U���p�����蔻��̃R���|�[�l���g��L����
	parent->GetComponent<SphereCollider>("LeftHandSphere")->SetEnable(true);

	//�U���͂�ݒ�
	attackPower = 3.0f;

	//��ԋ߂��G�̈ʒu��parameter�Ɋi�[
	SearchNearEnemy();

	//�G�ւ̍U���t���O��OFF
	parent->GetComponent<Player>()->SetIsHitAttackToEnemy(false);

	//�U�����t���O��true��
	parent->GetComponent<Player>()->SetIsAttack(true);

	parent->GetComponent<Player>()->NextAttackPhase();
}
void PunchLeft::Update()
{
	DirectX::XMFLOAT3 vec = parameter - parent->GetComponent<Transform>()->pos;
	vec.y = 0.0f;
	float length = LengthFloat3(vec);
	if (length < attackRangeMax)
	{
		YAxisRotate(vec);
		if (length > attackRangeMin)
			AddMoveVelocity(NormalizeFloat3(vec) * parent->GetComponent<Player>()->GetRunSpeed());
	}

	Default::Update();
}
void PunchLeft::Exit()
{
	parent->GetComponent<SphereCollider>("LeftHandSphere")->SetEnable(false);

	//�U�����t���O��false��
	parent->GetComponent<Player>()->SetIsAttack(false);
}
std::string PunchLeft::GetNext()
{
	//�A�j���[�V�����Đ����I�������
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		//�p���`�i���j�X�e�[�g�֑J�ڂł��邩
		if (JudgeAttackState())
		{
			return "Kick";
		}

		//�ҋ@�X�e�[�g�֑J��
		return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< �L�b�N >-----//
Kick::Kick()
{
	name = "Kick";
}
Kick::Kick(GameObject* parent)
{
	name = "Kick";
	this->parent = parent;
}
void Kick::Enter()
{
	//�A�j���[�V�����̍Đ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Kick, false);
	//�A�j���[�V�����X�s�[�h�̒���
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Kick));

	//������U������̂ŏ�����
	attackInterval = 0.0f;

	//�U���̓��̓t���O�Ȃǂ�������
	acceptAttackButton = false;
	pushAttackButton = false;

	//�U���p�����蔻��̃R���|�[�l���g��L����
	parent->GetComponent<SphereCollider>("RightAnkleSphere")->SetEnable(true);

	//�U���͂�ݒ�
	attackPower = 5.0f;

	//��ԋ߂��G�̈ʒu��parameter�Ɋi�[
	SearchNearEnemy();

	//�G�ւ̍U���t���O��OFF
	parent->GetComponent<Player>()->SetIsHitAttackToEnemy(false);

	//�U�����t���O��true��
	parent->GetComponent<Player>()->SetIsAttack(true);

	parent->GetComponent<Player>()->NextAttackPhase();
}
void Kick::Update()
{
	DirectX::XMFLOAT3 vec = parameter - parent->GetComponent<Transform>()->pos;
	vec.y = 0.0f;
	float length = LengthFloat3(vec);
	if (length < attackRangeMax)
	{
		YAxisRotate(vec);
		if (length > attackRangeMin)
			AddMoveVelocity(NormalizeFloat3(vec) * parent->GetComponent<Player>()->GetRunSpeed());
	}

	Default::Update();
}
void Kick::Exit()
{
	parent->GetComponent<SphereCollider>("RightAnkleSphere")->SetEnable(false);

	//�U�����t���O��false��
	parent->GetComponent<Player>()->SetIsAttack(false);
}
std::string Kick::GetNext()
{
	//�A�j���[�V�����Đ����I������痎���X�e�[�g�֑J��
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< �_���[�W >-----//
Damage::Damage()
{
	name = "Damage";
}
Damage::Damage(GameObject* parent)
{
	name = "Damage";
	this->parent = parent;
}
void Damage::Enter()
{
	//�A�j���[�V�����̐؂�ւ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Damage, false);
	//�A�j���[�V�����X�s�[�h�̒���
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Damage));
}
void Damage::Update()
{
	//�m�b�N�o�b�N��velocity
	AddMoveVelocity(parameter);

	Default::Update();
}
void Damage::Exit()
{

}
std::string Damage::GetNext()
{
	//�A�j���[�V�����Đ����I�������ҋ@�X�e�[�g�֑J��
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		if (parent->GetComponent<Transform>()->pos.y > 0.0f)
			return "Falling";
		else
			return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< ���S >-----//
Death::Death()
{
	name = "Death";
}
Death::Death(GameObject* parent)
{
	name = "Death";
	this->parent = parent;
}
void Death::Enter()
{
	//�A�j���[�V�����̐؂�ւ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Death, false);
	//�A�j���[�V�����X�s�[�h�̒���
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Death));
}
void Death::Update()
{
	// �A�j���[�V�������I������玀�S�t���O�𗧂Ă�
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		parent->GetComponent<Player>()->SetIsDead(true);
	}
}
void Death::Exit()
{

}
std::string Death::GetNext()
{
	//�ύX�Ȃ�
	return "";
}

//-----< �_�����ʒu�Ƀ��C���[���h���A�����I�ȃ��C���[�ړ� >-----//
AimWire::AimWire()
{
	name = "AimWire";
}
AimWire::AimWire(GameObject* parent)
{
	name = "AimWire";
	this->parent = parent;
}
void AimWire::Enter()
{
	jumpCount = 0;

	//�A�j���[�V�����̐؂�ւ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Jump, false);

}
void AimWire::Update()
{
	Default::Update();
}
void AimWire::Exit()
{

}
std::string AimWire::GetNext()
{
	//�W�����v�X�e�[�g�֑J�ڂł��邩
	if (JudgeJumpState())
	{
		return "Jump";
	}

	//����X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAvoidState() && isAvoid)
	{
		return "AvoidJump";
	}

	//�A�j���[�V�����Đ����I������痎���X�e�[�g�֑J��
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Falling";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< ���C���[�ł̌ʂ��������ړ� >-----//
SwingWire::SwingWire()
{
	name = "SwingWire";
}
SwingWire::SwingWire(GameObject* parent)
{
	name = "SwingWire";
	this->parent = parent;
}
void SwingWire::Enter()
{
	jumpCount = 0;

	//�J�����̑O�����x�N�g�����擾
	DirectX::XMFLOAT3 cameraFront = GetCameraFront();

	//���C���[�̎h���ʒu�i���j
	//���C���[���ΕӂƂ���90��,45��,45���̒��p�O�p�`�Ƃ݂Čv�Z����
	DirectX::XMFLOAT3 pos = parent->GetComponent<Transform>()->pos;
	parameter = pos + (cameraFront * (maxWireLength / sqrtf(2.0f)));//�����̈ʒu���烏�C���[���h��XZ���ʏ�̓_
	parameter.y += maxWireLength / sqrtf(2.0f);//���C���[���h���ʒu

	//������
	oldPosition[0] = pos;
	oldPosition[1] = pos;

	//Debug�`��
	parent->GetComponent<CapsuleCollider>("WireCapsule")->SetEnable(true);
	parent->GetComponent<CapsuleCollider>("WireCapsule")->begin = parameter;
	pos.y += parent->GetComponent<Player>()->GetHeight();
	parent->GetComponent<CapsuleCollider>("WireCapsule")->end = pos;

	//parent->GetComponent<RigidBody>()->SetUseGravity(false);

	//�A�j���[�V�����̐؂�ւ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::DangleWire, true);
	//�A�j���[�V�����X�s�[�h�̒���
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::DangleWire));


	swingTimer = 0.0f;
}
void SwingWire::Update()
{
	//�o�ߎ���
	swingTimer += SystemManager::Instance().GetElapsedTime();

	//�J�����̑O�����ɐi��(XZ���ʁj
	DirectX::XMFLOAT3 velocity = GetCameraFront() * parent->GetComponent<Player>()->GetWireSpeed();
	SetMoveVelocity(velocity);
	YAxisRotate(velocity);

	DirectX::XMFLOAT3 pos = parent->GetComponent<Transform>()->pos;

	//���C���[�̒����Ɋ�Â��ĕ␳
	DirectX::XMFLOAT3 vec = pos - parameter;
	vec = NormalizeFloat3(vec) * maxWireLength;
	pos = parameter + vec;
	parent->GetComponent<Transform>()->pos = pos;

	//�ʒu�̕ۑ�
	oldPosition[0] = oldPosition[1];
	oldPosition[1] = pos;

	//Debug�`��
	pos.y += parent->GetComponent<Player>()->GetHeight();
	parent->GetComponent<CapsuleCollider>("WireCapsule")->end = pos;

	parent->GetComponent<SphereCollider>("DebugSphere")->center = pos;

	//���̈ʒu�̃��[�V�����ɂ��ړ����~
	parent->GetComponent<ModelRenderer>()->StopMotionVelocity("FallingHip");

	Default::Update();
}
void SwingWire::Exit()
{
	parent->GetComponent<CapsuleCollider>("WireCapsule")->SetEnable(false);

	parent->GetComponent<RigidBody>()->SetUseGravity(true);

	parameter = oldPosition[0];
}
std::string SwingWire::GetNext()
{
	//�W�����v�X�e�[�g�֑J�ڂł��邩
	if (JudgeJumpState())
	{
		return "WireJump";
	}

	DirectX::XMFLOAT3 pos = parent->GetComponent<Transform>()->pos;
	float length = LengthFloat3(oldPosition[1] - oldPosition[0]);
	float l = parent->GetComponent<Player>()->GetWireSpeed() * SystemManager::Instance().GetElapsedTime();
	if (length < l * 0.05f || swingTimer > maxSwingTime)
	{
		return "WireJump";
	}

	//ZR�{�^���𗣂�����Jump�X�e�[�g�ɑJ��
	if (SystemManager::Instance().GetGamePad().GetButtonUp() & GamePad::BTN_RIGHT_SHOULDER)
	{
		return "WireJump";
	}

	//pos.y < 0.0f �Ȃ璅�n�X�e�[�g��
	if (parent->GetComponent<Transform>()->pos.y < 0.0f)
	{
		return "Landing";
	}

	//����X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAvoidState() && !isAvoid)
	{
		return "AvoidJump";
	}


	//�ύX�Ȃ�
	return "";
}

//-----< ���C���[�ł̌ʂ��������ړ���̃W�����v >-----//
WireJump::WireJump()
{
	name = "WireJump";
}
WireJump::WireJump(GameObject* parent)
{
	name = "WireJump";
	this->parent = parent;
}
void WireJump::Enter()
{
	//�W�����v����x�N�g�����v�Z
	DirectX::XMFLOAT3 pos = parent->GetComponent<Transform>()->pos;
	DirectX::XMFLOAT3 vec = pos - parameter;
	moveDirection = vec / SystemManager::Instance().GetElapsedTime();
	jumpSpeed = (moveDirection.y < 0.0f ? 0.0f : moveDirection.y) + parent->GetComponent<Player>()->GetJumpSpeed();
	moveDirection.y = 0.0f;

	//������
	oldPosition[0] = pos;
	oldPosition[1] = pos;

	//�W�����v�J�E���g�̍X�V
	jumpCount++;
	//�A�j���[�V�����̐؂�ւ�
	if (jumpCount <= 1)//�W�����v����
	{
		//�A�j���[�V�����̐؂�ւ�
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Falling, true);
		//�A�j���[�V�����X�s�[�h�̒���
		parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Falling));
	}
	else//����ȍ~
	{
		parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::JumpFlip, false);
		//�A�j���[�V�����X�s�[�h�̒���
		parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::JumpFlip));
	}

	//�W�����v
	parent->GetComponent<RigidBody>()->Jump(jumpSpeed);

	coolTimer = maxCoolTime;
	falling = false;
}
void WireJump::Update()
{
	//�ʒu�̕ۑ�
	DirectX::XMFLOAT3 pos = parent->GetComponent<Transform>()->pos;
	oldPosition[0] = oldPosition[1];
	oldPosition[1] = pos;

	DirectX::XMFLOAT3 inputVec = CalcMoveVec();
	DirectX::XMFLOAT3 moveVec{};
	if (LengthFloat3(inputVec) > FLT_EPSILON)
		moveVec = (inputVec * parent->GetComponent<Player>()->GetRunSpeed() * inputRatio) + (moveDirection * (1.0f - inputRatio));
	else
		moveVec = moveDirection;

	//�J�����̑O�����ɐi��(XZ���ʁj
	AddMoveVelocity(moveVec);
	YAxisRotate(inputVec);

	if (jumpCount > 1)
	{
		if (!falling)
		{
			//���̈ʒu�̃��[�V�����ɂ��ړ����~
			parent->GetComponent<ModelRenderer>()->StopMotionVelocity("FallingHip");
			if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
			{
				falling = true;
				//�A�j���[�V�����̐؂�ւ�
				parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Falling, true);
				//�A�j���[�V�����X�s�[�h�̒���
				parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Falling));
			}
		}
	}

	coolTimer -= SystemManager::Instance().GetElapsedTime();

	Default::Update();
}
void WireJump::Exit()
{
}
std::string WireJump::GetNext()
{
	//�󒆍U���X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAttackState())
	{
		return "JumpAttack";
	}

	//���C���[�ł̌ʂ��������ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeSwingWireState() && coolTimer < 0.0f)
	{
		return "SwingWire";
	}

	//���C���[�ł̒����ړ��X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAimWireState() && coolTimer < 0.0f)
	{
		return "AimWire";
	}

	//����X�e�[�g�ɑJ�ڂł��邩
	if (JudgeAvoidState() && !isAvoid)
	{
		return "AvoidJump";
	}

	//�W�����v�X�e�[�g�֑J�ڂł��邩
	if (JudgeJumpState())
	{
		parameter = oldPosition[0];

		return "WireJump";
	}

	//pos.y < 0.0f �Ȃ璅�n�X�e�[�g��
	if (parent->GetComponent<Transform>()->pos.y < 0.0f)
	{
		return "Landing";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< �󒆍U���i�E�p���`�j >-----//
JumpAttack::JumpAttack()
{
	name = "JumpAttack";
}
JumpAttack::JumpAttack(GameObject* parent)
{
	name = "JumpAttack";
	this->parent = parent;
}
void JumpAttack::Enter()
{
	//�A�j���[�V�����̍Đ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::JumpPunch, false);
	//�A�j���[�V�����̍Đ����x�̕ύX
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::JumpPunch));

	//�U���p�����蔻��̃R���|�[�l���g��L����
	parent->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(true);

	//�U���͂�ݒ�
	attackPower = 2.0f;

	//��ԋ߂��G�̈ʒu��parameter�Ɋi�[
	SearchNearEnemy();

	//�ړ��x�N�g�����v�Z
	DirectX::XMFLOAT3 vec = parameter - parent->GetComponent<Transform>()->pos;
	float length = LengthFloat3(vec);

	parameter = { 0.0f,0.0f,0.0f };

	if (length < attackRangeMax)
	{
		if (length > attackRangeMin)
		{
			float animSpeed = parent->GetComponent<ModelRenderer>()->GetAnimationSpeed();
			parameter = NormalizeFloat3(vec) * (length / maxMoveTime * animSpeed);

			//�d�͂𖳎�����
			parent->GetComponent<RigidBody>()->SetUseGravity(false);

			//2�i�ȍ~�̍U���̓��͂̎擾���J�n
			acceptAttackButton = true;

			//������U������̂ŏ�����
			attackInterval = 0.0f;
		}
	}


	//�U��������������
	attackTimer = 0.0f;

	//�G�ւ̍U���t���O��OFF
	parent->GetComponent<Player>()->SetIsHitAttackToEnemy(false);

	//�U�����t���O��true��
	parent->GetComponent<Player>()->SetIsAttack(true);

	parent->GetComponent<Player>()->NextAttackPhase();

}
void JumpAttack::Update()
{
	float animSpeed = parent->GetComponent<ModelRenderer>()->GetAnimationSpeed();
	attackTimer += SystemManager::Instance().GetElapsedTime() * animSpeed;

	if (attackTimer < maxMoveTime && LengthFloat3(parameter) > FLT_EPSILON)
	{
		SetMoveVelocity(parameter);
		YAxisRotate(parameter);
	}
	else
	{
		parent->GetComponent<RigidBody>()->SetUseGravity(true);
	}

	if (LengthFloat3(parameter) < FLT_EPSILON)
	{
		DirectX::XMFLOAT3 moveVelocity = CalcMoveVec();
		moveVelocity *= parent->GetComponent<Player>()->GetRunSpeed();

		AddMoveVelocity(moveVelocity);
		YAxisRotate(moveVelocity);
	}


	Default::Update();

}
void JumpAttack::Exit()
{
	parent->GetComponent<SphereCollider>("RightHandSphere")->SetEnable(false);

	//�U�����t���O��false��
	parent->GetComponent<Player>()->SetIsAttack(false);

}
std::string JumpAttack::GetNext()
{
	//�A�j���[�V�����Đ����I������痎���X�e�[�g�֑J��
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		if (parent->GetComponent<Transform>()->pos.y > 0.0f)
			return "Falling";
		else
			return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< ��� >-----//
Avoid::Avoid()
{
	name = "Avoid";
}
Avoid::Avoid(GameObject* parent)
{
	name = "Avoid";
	this->parent = parent;
}
void Avoid::Enter()
{
	//�A�j���[�V�����̍Đ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::Avoid, false);
	//�A�j���[�V�����̍Đ����x�̕ύX
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Avoid));

	//����x�N�g�����쐬
	avoidVec = CalcMoveVec() * avoidSpeed;

	//L�X�e�B�b�N���͂��Ȃ��Ȃ�J�����̑O�����ɉ��
	if (LengthFloat3(avoidVec) < FLT_EPSILON)
	{
		avoidVec = GetCameraFront() * avoidSpeed;
	}
}
void Avoid::Update()
{
	AddMoveVelocity(avoidVec);
	YAxisRotate(avoidVec);

	//���̈ʒu�̃��[�V�����ɂ��ړ����~
	parent->GetComponent<ModelRenderer>()->StopMotionXZVelocity("IdleHip");

	Default::Update();
}
void Avoid::Exit()
{
}
std::string Avoid::GetNext()
{
	//�A�j���[�V�����Đ����I�������ҋ@�X�e�[�g�֑J��
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< �󒆉�� >-----//
AvoidJump::AvoidJump()
{
	name = "AvoidJump";
}
AvoidJump::AvoidJump(GameObject* parent)
{
	name = "AvoidJump";
	this->parent = parent;
}
void AvoidJump::Enter()
{
	//�A�j���[�V�����̍Đ�
	parent->GetComponent<ModelRenderer>()->PlayAnimation((int)Animation::JumpFlip, false);
	//�A�j���[�V�����̍Đ����x�̕ύX
	parent->GetComponent<ModelRenderer>()->SetAnimationSpeed(GetAnimationSpeed((int)Animation::JumpFlip));

	//����x�N�g�����쐬
	avoidVec = CalcMoveVec() * avoidSpeed;

	//L�X�e�B�b�N���͂��Ȃ��Ȃ�J�����̑O�����ɉ��
	if (LengthFloat3(avoidVec) < FLT_EPSILON)
	{
		avoidVec = GetCameraFront() * avoidSpeed;
	}

	parent->GetComponent<RigidBody>()->Jump(avoidJumpPower);

	isAvoid = true;
}
void AvoidJump::Update()
{
	AddMoveVelocity(avoidVec);
	YAxisRotate(avoidVec);

	//���̈ʒu�̃��[�V�����ɂ��ړ����~
	parent->GetComponent<ModelRenderer>()->StopMotionVelocity("FallingHip");

	Default::Update();
}
void AvoidJump::Exit()
{
}
std::string AvoidJump::GetNext()
{
	//�A�j���[�V�����Đ����I�������ҋ@�X�e�[�g�֑J��
	if (parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Falling";
	}

	//�ύX�Ȃ�
	return "";
}

//-----< �I�u�W�F�N�g�Ƀ��C���[���h���ĐU��񂵂ē����� >-----//
WieldThrow::WieldThrow()
{
	name = "WieldThrow";
}
WieldThrow::WieldThrow(GameObject* parent)
{
	name = "WieldThrow";
	this->parent = parent;
}
void WieldThrow::Enter()
{
#if _APPEND
	//�R���|�[�l���g�̎擾
	ModelRenderer* modelRenderer = parent->GetComponent<ModelRenderer>();

	//�A�j���[�V�����̍Đ�
	modelRenderer->PlayAnimation((int)Animation::Thrust, false);
	//�A�j���[�V�����̍Đ����x�̕ύX
	modelRenderer->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Thrust));

	//�X�e�[�g�̏�����
	state = Thrust;

	Vector3 objPos = parent->GetComponent<Player>()->GetThrowObj()->GetComponent<Transform>()->pos;
	Vector3 pos = parent->GetComponent<Transform>()->pos;

	float samplingRate = modelRenderer->GetSamplingRate();
	float length = (objPos - pos).Length();
	wireSpeed = length * samplingRate * GetAnimationSpeed((int)Animation::Thrust);

	appearWire = false;
	thrust = false;
#endif
}
void WieldThrow::Update()
{
	switch (state)
	{
	case Thrust:
#if _APPEND
	{
		//�I�u�W�F�N�g�̕�������
		GameObject* throwObj = parent->GetComponent<Player>()->GetThrowObj();
		Vector3 objPos = throwObj->GetComponent<Transform>()->pos;
		Vector3 pos = parent->GetComponent<Transform>()->pos;
		Vector3 plObjVec = objPos - pos;
		YAxisRotate(plObjVec);

		//�R���|�[�l���g�̎擾
		ModelRenderer* modelRenderer = parent->GetComponent<ModelRenderer>();

		//�A�j���[�V���������̃t���[�����߂����烏�C���[���o��
		if (modelRenderer->GetPlayAnimTimer() > maxThrustInterval)
		{
			CapsuleCollider* capsule = parent->GetComponent<CapsuleCollider>("WireCapsule");

			//�E��̈ʒu
			Vector3 rightHandPos = modelRenderer->GetBonePositionFromName("rightHand");

			//���C���[�̏o��
			if (!appearWire)
			{
				capsule->SetEnable(true);

				wireTipPos = rightHandPos;

				appearWire = true;
			}

			//���C���[�̈ړ�
			if (!thrust)
			{
				Vector3 handObjVec = objPos - rightHandPos;
				handObjVec.Normalize();
				wireTipPos += handObjVec * wireSpeed * SystemManager::Instance().GetElapsedTime();
			}

			//���C���[�̐�[�ƃI�u�W�F�N�g�̋������I�u�W�F�N�g�̃X�t�B�A�̔��a�ȉ��Ȃ�I�u�W�F�N�g�Ƀ��C���[���h����
			Vector3 wireTipObjvec = objPos - wireTipPos;
			if (wireTipObjvec.Length() < throwObj->GetComponent<SphereCollider>()->radius
				|| plObjVec.Dot(wireTipObjvec) < 0.0f)
			{
				thrust = true;
				wireTipPos = objPos;
			}

			capsule->begin = wireTipPos;
			capsule->end = rightHandPos;

		}

		//�A�j���[�V�������I������玟�̃X�e�[�g��
		if (modelRenderer->IsFinishAnimation())
		{
			//�A�j���[�V�����̍Đ�
			modelRenderer->PlayAnimation((int)Animation::Wield, false);
			//�A�j���[�V�����̍Đ����x�̕ύX
			modelRenderer->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Wield));

			state = Wield;

			//���݂̉�]�p���v�Z
			//rotateRadian = atan2f(plObjVec.z, plObjVec.x);
			//��]���x���v�Z
			float samplingRate = modelRenderer->GetSamplingRate();
			rotateSpeed = maxWieldRadian / ((float)modelRenderer->GetPlayAnimMaxTimer() / (samplingRate * GetAnimationSpeed((int)Animation::Wield)));

			rotateVec = objPos - pos;
		}

		break;
	}
#endif
	case Wield:
#if _APPEND
	{
		//��]��
		const Vector3 axis = { 0.0f,1.0f,0.0f };

		//�N�I�[�^�j�I���̍쐬
		const Quaternion Q = Quaternion::CreateFromAxisAngle(axis, -rotateSpeed * SystemManager::Instance().GetElapsedTime());

		//��]�s��̍쐬
		const Matrix R = Matrix::CreateFromQuaternion(Q);

		//�x�N�g������]������
		Vector4 v = Float4MultiplyFloat4x4({ rotateVec.x,rotateVec.y,rotateVec.z,1.0f }, R);
		rotateVec = { v.x,v.y,v.z };

		//�I�u�W�F�N�g�̈ʒu�̍X�V
		GameObject* throwObj = parent->GetComponent<Player>()->GetThrowObj();
		Vector3 objPos = throwObj->GetComponent<Transform>()->pos;
		Vector3 pos = parent->GetComponent<Transform>()->pos;

		objPos = pos + rotateVec;
		throwObj->GetComponent<Transform>()->pos = objPos;

		//�R���|�[�l���g�̎擾
		ModelRenderer* modelRenderer = parent->GetComponent<ModelRenderer>();

		//�E��̈ʒu���擾
		Vector3 rightHandPos = modelRenderer->GetBonePositionFromName("rightHand");

		//�J�v�Z�����擾
		CapsuleCollider* capsule = parent->GetComponent<CapsuleCollider>("WireCapsule");
		//�J�v�Z���̈ʒu���X�V
		capsule->begin = objPos;
		capsule->end = rightHandPos;

		if (modelRenderer->IsFinishAnimation())
		{
			//�A�j���[�V�����̍Đ�
			modelRenderer->PlayAnimation((int)Animation::Throw, false);
			//�A�j���[�V�����̍Đ����x�̕ύX
			modelRenderer->SetAnimationSpeed(GetAnimationSpeed((int)Animation::Throw));

			state = Throw;

			//��]���x���v�Z
			float samplingRate = modelRenderer->GetSamplingRate();
			float t = (float)modelRenderer->GetPlayAnimMaxTimer();
			rotateSpeed = maxThrowRadian / (maxRotateTime / (samplingRate * GetAnimationSpeed((int)Animation::Throw)));
			rotateAxis = { 0.0f,1.0f,0.0f };
			rotateRadian = 0.0f;

			//�N�ɓ������邩���߂�
			enemyTransform = nullptr;
			SearchNearEnemy(enemyTransform);//��ԋ߂��G��Transform���擾
			
			if (enemyTransform != nullptr)
			{
				//�G�̈ʒu
				Vector3 enePos = enemyTransform->pos;
				//�v���C���[����G�ւ̃x�N�g��
				Vector3 vec = enePos - pos;
				//�G�܂ł̋���
				float distance = vec.Length();
				//�G�܂ł̋���������藣��Ă���Ȃ�N���A
				if (distance > searchRange)
				{
					enemyTransform = nullptr;
				}
			}
		}

		break;
	}
#endif
	case Throw:
#if _APPEND
	{
		//�R���|�[�l���g�̎擾
		ModelRenderer* modelRenderer = parent->GetComponent<ModelRenderer>();

		float animTimer = modelRenderer->GetPlayAnimTimer();
		if (animTimer < maxThrowTime)
		{
			//�E��̈ʒu���擾
			Vector3 rightHandPos = modelRenderer->GetBonePositionFromName("rightHand");
			//�v���C���[�̈ʒu
			Vector3 pos = parent->GetComponent<Transform>()->pos;

			//��]�����v���C���[�̑O���������ɉ�]����
			if (animTimer > maxRotateTime)
			{
				rotateSpeed = DirectX::XMConvertToRadians(360.0f);
				//��]�p�x���v�Z
				float speed = rotateSpeed * SystemManager::Instance().GetElapsedTime();
				rotateRadian += speed;
				constexpr float maxRad = DirectX::XMConvertToRadians(90.0f);
				if (rotateRadian > maxRad)
				{
					speed -= rotateRadian - maxRad;
					rotateRadian = maxRad;
				}

				//�v���C���[�̑O����
				Vector3 axis = parent->GetComponent<Transform>()->GetForward();
				//�N�I�[�^�j�I���̍쐬
				const Quaternion Q = Quaternion::CreateFromAxisAngle(axis, speed);
				//��]�s��̍쐬
				const Matrix R = Matrix::CreateFromQuaternion(Q);

				//�x�N�g������]������
				Vector4 v = Float4MultiplyFloat4x4({ rotateAxis.x,rotateAxis.y,rotateAxis.z,1.0f }, R);

				rotateAxis = { v.x,v.y,v.z };
			}

			//�N�I�[�^�j�I���̍쐬
			const Quaternion Q = Quaternion::CreateFromAxisAngle(rotateAxis, -rotateSpeed * SystemManager::Instance().GetElapsedTime());

			//��]�s��̍쐬
			const Matrix R = Matrix::CreateFromQuaternion(Q);

			//�x�N�g������]������
			Vector4 v = Float4MultiplyFloat4x4({ rotateVec.x,rotateVec.y,rotateVec.z,1.0f }, R);
			rotateVec = { v.x,v.y,v.z };

			//�I�u�W�F�N�g�̈ʒu�̍X�V
			GameObject* throwObj = parent->GetComponent<Player>()->GetThrowObj();
			Vector3 objPos = throwObj->GetComponent<Transform>()->pos;

			objPos = pos + rotateVec;
			throwObj->GetComponent<Transform>()->pos = objPos;

			//�J�v�Z�����擾
			CapsuleCollider* capsule = parent->GetComponent<CapsuleCollider>("WireCapsule");
			//�J�v�Z���̈ʒu���X�V
			capsule->begin = objPos;
			capsule->end = rightHandPos;
		}

		//���̈ʒu�̃��[�V�����ɂ��ړ����~
		modelRenderer->StopMotionXZVelocity("IdleHip");

		break;
	}
#endif
	}

	Default::Update();
}
void WieldThrow::Exit()
{
	//�J�v�Z�����\����
	parent->GetComponent<CapsuleCollider>("WireCapsule")->SetEnable(false);
}
std::string WieldThrow::GetNext()
{
	//�A�j���[�V�����Đ����I�������ҋ@�X�e�[�g�֑J��
	if (state == Throw && parent->GetComponent<ModelRenderer>()->IsFinishAnimation())
	{
		return "Idle";
	}

	//�ύX�Ȃ�
	return "";
}

void WieldThrow::DebugGui()
{
	if (ImGui::TreeNode(name.c_str()))
	{
		float degree = DirectX::XMConvertToDegrees(maxWieldRadian);
		ImGui::DragFloat("maxWieldDegree", &degree);
		maxWieldRadian = DirectX::XMConvertToRadians(degree);
		
		ImGui::DragFloat("maxThrowTime", &maxThrowTime);

		ImGui::TreePop();
	}
}