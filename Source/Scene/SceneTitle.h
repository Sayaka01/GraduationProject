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

	//選択肢ボタンの名前
	enum MenuTextString:int
	{
		Tutorial = 0,
		StartGame,
		FinishGame,
	};

	//選択肢ボタン画像のリスト
	std::map<std::string,std::unique_ptr<GameObject>> menuText;
	std::string menuTextName[5];

};