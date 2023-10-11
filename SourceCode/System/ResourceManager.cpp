#include "Stdafx.h"
#include "ResourceManager.h"

std::shared_ptr<ModelData> ResourceManager::LoadModelResource(const char* filename, bool pbr)
{
	//���f��������
    ModelMap::iterator  it = models.find(filename);
    if (it != models.end())
    {
        if (!it->second.expired())
        {
            return it->second.lock();
        }
    }

    //���f���̃��[�h�Ɠo�^
    std::shared_ptr<ModelData> model = std::make_shared<ModelData>(filename, pbr);
    models[filename] = model;
    return model;
}
