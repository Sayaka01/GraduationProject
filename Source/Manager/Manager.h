#pragma once

#include <vector>

#include "GameObject/GameObject.h"


class Manager : public GameObject
{
private:
	//�R���X�g���N�^
	Manager() = default;
	//�f�X�g���N�^
	virtual ~Manager() = default;

public:
	//�C���X�^���X�擾
	static Manager& Instance()
	{
		static Manager instance;
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

	//�o�^
	void Register(GameObject* obj);
	//�폜
	void Remove(GameObject* obj);
	//�S�폜
	void Clear();

	//�Q�[���I�u�W�F�N�g�����擾
	[[nodiscard]] int GetGameObjCount()const { return static_cast<int>(gameObjects.size()); }
	//�Q�[���I�u�W�F�N�g���擾
	[[nodiscard]] GameObject* GetGameObj(size_t index)const { return gameObjects.at(index); }

protected:
	std::vector<GameObject*> gameObjects;
	std::vector<GameObject*> removes;

};