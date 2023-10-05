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

	//~óÉ®©·
	//rePoint:îÊuAdegree:ñ]ÊAradius:¼a
	DirectX::XMFLOAT2 moveRoundFloat2(DirectX::XMFLOAT2 rePoint, DirectX::XMFLOAT2 degree, float radius);

	//Ið{^Ì¼O
	enum MenuTextString:int
	{
		Tutorial_mst = 0,
		StartGame_mst,
		FinishGame_mst,
		Max_mst,//Åå
	};

	//Ið{^æÌXg
	std::map<std::string,std::unique_ptr<GameObject>> menuText;
	std::string menuTextName[MenuTextString::Max_mst];//æÌ¼OXg(String^)

	int selectMenuType = MenuTextString::StartGame_mst;
	float currentSelectDegree = 0.0f;//Ið{^Ì»ÝÌñ]Ê
	float targetSelectDegree = 0.0f;//Ið{^Ìñ·ñ]Ê
	

};