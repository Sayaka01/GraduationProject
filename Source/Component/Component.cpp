
#include <Stdafx.h>
#include "Component.h"
#include "../GameObject/GameObject.h"

//�e�I�u�W�F�N�g�̖��O�擾
std::string Component::GetParentName() const
{
	return parent->GetName();
}
