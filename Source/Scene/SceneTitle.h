#pragma once

#include "Scene.h"

class GameObject;

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

	//�I�����{�^���̖��O
	enum MenuTextString:int
	{
		Tutorial = 0,
		StartGame,
		FinishGame,
	};

	//�I�����{�^���摜�̃��X�g
	std::map<std::string,std::unique_ptr<GameObject>> menuText;
	std::string menuTextName[5];

};