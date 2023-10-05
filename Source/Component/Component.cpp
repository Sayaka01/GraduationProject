
#include "Stdafx.h"
#include "Component.h"

#include "GameObject/GameObject.h"

//このコンポーネントの親オブジェクトの名前の取得
[[nodiscard]] std::string Component::GetParentName() const
{
	return parent->GetName();
}