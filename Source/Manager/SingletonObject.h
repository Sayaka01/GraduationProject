#pragma once

#include "GameObject/GameObject.h"


class SingletonObject : public GameObject
{
private:
	//�R���X�g���N�^
	SingletonObject() = default;
	//�f�X�g���N�^
	virtual ~SingletonObject() override = default;

public:
	//�C���X�^���X�擾
	static SingletonObject& Instance()
	{
		static SingletonObject instance;
		return instance;
	}
};