#pragma once

#include "Scene.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include "GameObject/GameObject.h"

class SceneTitle :public Scene
{
public:
	SceneTitle() {}
	~SceneTitle()override {}

	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;

private:

	//�~��ɓ�����
	//rePoint:��ʒu�Adegree:��]�ʁAradius:���a
	DirectX::XMFLOAT2 moveRoundFloat2(DirectX::XMFLOAT2 rePoint, DirectX::XMFLOAT2 degree, DirectX::XMFLOAT2 radius);

	//�I�����{�^���̖��O
	enum MenuTextString:short
	{
		Tutorial_mst = 0,
		StartGame_mst,
		FinishGame_mst,
		Max_mst,//�ő吔
	};

	enum class OperateType:short
	{
		None = -1,
		Up,
		Down,
	};

	//�^�C�g�����S�摜
	std::unique_ptr<GameObject> titleLogo;
	std::unique_ptr<GameObject> menuBack;



	//�I�����{�^���摜�̔z��
	std::map<std::string,std::unique_ptr<GameObject>> menuText;
	//�摜�̖��O���X�g(String�^)
	std::string menuNameList[MenuTextString::Max_mst];

	//�I�𒆂̃��j���[�^�C�v
	short selectMenuType = MenuTextString::StartGame_mst;
	//�I�����{�^���̌��݂̉�]��
	float currentSelectDegree = -40;
	float currentUISelectDegree = -40;

	//��]�����邻�ꂼ��̍ő�l
	float targetMenuDegreeList[3] = { 0,-40,-80 };

	//���j���[�e�L�X�g���܂�锼�a�T�C�Y
	DirectX::XMFLOAT2 menuRadius = { 0,0 };
	//��]�̒��S�_
	DirectX::XMFLOAT2 circlePivot = { 0,0 };

	//�C�[�W���O�p��UI�o�ߎ���
	float uiTime = 0.0f;

	OperateType operateType = OperateType::None;

};