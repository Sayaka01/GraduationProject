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

	//円状に動かす
	//rePoint:基準位置、degree:回転量、radius:半径
	DirectX::XMFLOAT2 moveRoundFloat2(DirectX::XMFLOAT2 rePoint, DirectX::XMFLOAT2 degree, DirectX::XMFLOAT2 radius);

	//選択肢ボタンの名前
	enum MenuTextString:short
	{
		Tutorial_mst = 0,
		StartGame_mst,
		FinishGame_mst,
		Max_mst,//最大数
	};

	enum class OperateType:short
	{
		None = -1,
		Up,
		Down,
	};

	//タイトルロゴ画像
	std::unique_ptr<GameObject> titleLogo;
	std::unique_ptr<GameObject> menuBack;



	//選択肢ボタン画像の配列
	std::map<std::string,std::unique_ptr<GameObject>> menuText;
	//画像の名前リスト(String型)
	std::string menuNameList[MenuTextString::Max_mst];

	//選択中のメニュータイプ
	short selectMenuType = MenuTextString::StartGame_mst;
	//選択肢ボタンの現在の回転量
	float currentSelectDegree = -40;
	float currentUISelectDegree = -40;

	//回転させるそれぞれの最大値
	float targetMenuDegreeList[3] = { 0,-40,-80 };

	//メニューテキストがまわる半径サイズ
	DirectX::XMFLOAT2 menuRadius = { 0,0 };
	//回転の中心点
	DirectX::XMFLOAT2 circlePivot = { 0,0 };

	//イージング用のUI経過時間
	float uiTime = 0.0f;

	OperateType operateType = OperateType::None;

};