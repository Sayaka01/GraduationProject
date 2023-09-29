
#include "Stdafx.h"

#include "Manager.h"


void Manager::Update()
{
	for (const auto& obj : gameObjects)
	{
		obj->Update();
	}

	//�j������
	for (GameObject* obj : removes)
	{
		//std::vector����v�f���폜����Ƃ��̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<GameObject*>::iterator it = std::find(gameObjects.begin(), gameObjects.end(), obj);

		if (it != gameObjects.end())
		{
			gameObjects.erase(it);
		}
		delete obj;
		obj = nullptr;
	}
	removes.clear();
}

void Manager::Draw()
{
	for (const auto& obj : gameObjects)
	{
		obj->Draw();
	}
}

void Manager::Register(GameObject* enemy)
{
	gameObjects.emplace_back(enemy);
}
void Manager::Remove(GameObject* enemy)
{
	//�j�����X�g�ɒǉ�
	removes.emplace_back(enemy);
}

void Manager::Clear()
{
	for (const auto& obj : gameObjects)
	{
		removes.emplace_back(obj);
	}

	//�j������
	for (GameObject* obj : removes)
	{
		//std::vector����v�f���폜����Ƃ��̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<GameObject*>::iterator it = std::find(gameObjects.begin(), gameObjects.end(), obj);

		if (it != gameObjects.end())
		{
			gameObjects.erase(it);
		}
		delete obj;
	}
	removes.clear();
	gameObjects.clear();

}

void Manager::DebugGui()
{
#ifdef USE_IMGUI
	ImGui::Begin("EnemyManager");
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		std::string s = std::string("Enemy") + std::to_string(i);
		if (ImGui::TreeNode(s.c_str()))
		{
			gameObjects.at(i)->DebugGui();
			ImGui::TreePop();
		}
	}
	ImGui::End();
#endif
}