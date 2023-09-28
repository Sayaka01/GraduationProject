
#include <Stdafx.h>
#include "Component.h"
#include "../GameObject/GameObject.h"

//親オブジェクトの名前取得
std::string Component::GetParentName() const
{
	return parent->GetName();
}
