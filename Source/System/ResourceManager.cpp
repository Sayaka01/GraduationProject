#include "Stdafx.h"
#include "ResourceManager.h"

std::shared_ptr<SkinnedMesh> ResourceManager::LoadModelResource(ID3D11Device* device, const char* filename, bool pbr)
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
    std::shared_ptr<SkinnedMesh> model = std::make_shared<SkinnedMesh>(device, filename, pbr);
    models[filename] = model;
    return model;
}
