#pragma once

#include "Scene.h"
#include <cstdlib>
#include <iostream>
#include <memory>
#include "GameObject/GameObject.h"

class SceneGame :public Scene
{
public:
	SceneGame() {}
	~SceneGame()override {}

	void Initialize()override;
	void Finalize()override;
	void Update()override;
	void Draw()override;

private:

	//円状に動かす
	//rePoint:基準位置、degree:回転量、radius:半径
	DirectX::XMFLOAT2 moveRoundFloat2(DirectX::XMFLOAT2 rePoint, DirectX::XMFLOAT2 degree, DirectX::XMFLOAT2 radius);

	//選択肢ボタンの名前
	enum MenuTextString :int
	{
		Tutorial_mst = 0,
		StartGame_mst,
		FinishGame_mst,
		Max_mst,//最大数
	};

	std::unique_ptr<GameObject> titleLogo;

	//選択肢ボタン画像のリスト
	std::map<std::string, std::unique_ptr<GameObject>> menuText;
	std::string menuTextName[MenuTextString::Max_mst];//画像の名前リスト(String型)

	int selectMenuType = MenuTextString::StartGame_mst;
	float currentSelectDegree = 0.0f;//選択肢ボタンの現在の回転量
	float targetSelectDegree = 0.0f;//選択肢ボタンの回す回転量

	//メニューテキストがまわる半径サイズ
	DirectX::XMFLOAT2 menuRadius = { 0,0 };
	DirectX::XMFLOAT2 circlePivot = { 0,0 };


};