
#include "Stdafx.h"
#include "Component.h"

#include "GameObject/GameObject.h"

//���̃R���|�[�l���g�̐e�I�u�W�F�N�g�̖��O�̎擾
[[nodiscard]] std::string Component::GetParentName() const
{
	return parent->GetName();
}