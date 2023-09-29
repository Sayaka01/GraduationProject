#pragma once

#include <vector>

#include "GameObject/GameObject.h"


class SingletonObject : public GameObject
{
private:
	//�R���X�g���N�^
	SingletonObject() = default;
	//�f�X�g���N�^
	~SingletonObject() override = default;

public:
	//�C���X�^���X�擾
	static SingletonObject& Instance()
	{
		static SingletonObject instance;
		return instance;
	}

	//������
	void Initialize() override;
	//�X�V
	void Update() override;
	//�`��
	void Draw() override;
	//ImGui
	void DebugGui() override;
	

};