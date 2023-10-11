#include "Stdafx.h"

#include "ModelData.h"
#include <functional>
#include "System/CreateShader.h"
#include <filesystem>
#include <fstream>
#include "System/Misc.h"

#include "System/SystemManager.h"

using namespace DirectX;

struct BoneInfluence
{
    uint32_t boneIndex;
    float boneWeight;
};
using BoneInfluencesPerControlPoint = std::vector<BoneInfluence>;
//プロトタイプ宣言
void FetchBoneInfluences(const FbxMesh* fbxMesh, std::vector<BoneInfluencesPerControlPoint>& boneInfluences);

//inline
inline XMFLOAT4X4 ConvertToXMFloat4x4(const FbxAMatrix& fbxamatrix)
{
    XMFLOAT4X4 xmfloat4x4{};
    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            xmfloat4x4.m[row][column] = static_cast<float>(fbxamatrix[row][column]);
        }
    }
    return xmfloat4x4;
}
inline XMFLOAT3 ConvertToXMFloat3(const FbxDouble3& fbxdouble3)
{
    XMFLOAT3 xmfloat3{};
    xmfloat3.x = static_cast<float>(fbxdouble3[0]);
    xmfloat3.y = static_cast<float>(fbxdouble3[1]);
    xmfloat3.z = static_cast<float>(fbxdouble3[2]);
    return xmfloat3;
}
inline XMFLOAT4 ConvertToXMFloat4(const FbxDouble4& fbxdouble4)
{
    XMFLOAT4 xmfloat4{};
    xmfloat4.x = static_cast<float>(fbxdouble4[0]);
    xmfloat4.y = static_cast<float>(fbxdouble4[1]);
    xmfloat4.z = static_cast<float>(fbxdouble4[2]);
    xmfloat4.w = static_cast<float>(fbxdouble4[3]);
    return xmfloat4;
}


ModelData::ModelData(const char* fbxFilename, float samplingRate, bool triangulate)
{
    std::filesystem::path cerealFilename(fbxFilename);
    cerealFilename.replace_extension("cereal");
    if (std::filesystem::exists(cerealFilename.c_str()))
    {
        std::ifstream ifs(cerealFilename.c_str(), std::ios::binary);
        cereal::BinaryInputArchive deserialization(ifs);
        deserialization(sceneView, meshes, materials, animationClips);
    }
    else
    {

        FbxManager* fbxManager{ FbxManager::Create() };
        FbxScene* fbxScene{ FbxScene::Create(fbxManager, "") };

        FbxImporter* fbxImporter{ FbxImporter::Create(fbxManager, "") };
        bool importStatus{ false };
        importStatus = fbxImporter->Initialize(fbxFilename);
        _ASSERT_EXPR_A(importStatus, fbxImporter->GetStatus().GetErrorString());

        importStatus = fbxImporter->Import(fbxScene);
        _ASSERT_EXPR_A(importStatus, fbxImporter->GetStatus().GetErrorString());

        FbxGeometryConverter fbxConverter(fbxManager);
        if (triangulate)
        {
            fbxConverter.Triangulate(fbxScene, true/*replace*/, false/*legacy*/);
            fbxConverter.RemoveBadPolygonsFromMeshes(fbxScene);
        }

        std::function<void(FbxNode*)> traverse{ [&](FbxNode* fbxNode)
        {
                ModelScene::Node& node{ sceneView.nodes.emplace_back() };
                node.attribute = fbxNode->GetNodeAttribute() ?
                    fbxNode->GetNodeAttribute()->GetAttributeType() : FbxNodeAttribute::EType::eUnknown;
                node.name = fbxNode->GetName();
                node.uniqueId = fbxNode->GetUniqueID();
                node.parentIndex = sceneView.IndexOf(fbxNode->GetParent() ?
                    fbxNode->GetParent()->GetUniqueID() : 0);
                for (int childIndex = 0; childIndex < fbxNode->GetChildCount(); ++childIndex)
                {
                    traverse(fbxNode->GetChild(childIndex));
                }
        }
        };

#if 0
        for (const Scene::Node& node : scene_view.nodes)
        {
            FbxNode* fbx_node{ fbx_scene->FindNodeByName(node.name.c_str()) };
            // Display node data in the output window as debug
            std::string node_name = fbx_node->GetName();
            uint64_t uid = fbx_node->GetUniqueID();
            uint64_t parent_uid = fbx_node->GetParent() ? fbx_node->GetParent()->GetUniqueID() : 0;
            int32_t type = fbx_node->GetNodeAttribute() ? fbx_node->GetNodeAttribute()->GetAttributeType() : 0;
            std::stringstream debug_string;
            debug_string << node_name << ":" << uid << ":" << parent_uid << ":" << type << "\n";
            OutputDebugStringA(debug_string.str().c_str());
        }
#endif

        traverse(fbxScene->GetRootNode());

        FetchMeshes(fbxScene, meshes);
        
    	FetchMaterials(fbxScene, materials);

        FetchAnimations(fbxScene, animationClips, samplingRate);

        fbxManager->Destroy();

        std::ofstream ofs(cerealFilename.c_str(), std::ios::binary);
        cereal::BinaryOutputArchive serialization(ofs);
        serialization(sceneView, meshes, materials, animationClips);
    }

    CreateComObjects(fbxFilename);
}

void ModelData::Draw(const DirectX::XMFLOAT4X4& world, Animation::Keyframe* keyframe, /*Frustum* frustum, */DirectX::XMFLOAT4 materialColor)
{
    SystemManager::Instance().SetRasterizerState(SystemManager::RasterizerState::SolidCullingOn);
    SystemManager::Instance().SetDepthStencilState(SystemManager::DepthStencilState::SortOnWriteOn);

    ID3D11DeviceContext* dc = SystemManager::Instance().GetDeviceContext();

    for (const Mesh& mesh : meshes)
    {
        uint32_t stride{ sizeof(Vertex) };
        uint32_t offset{ 0 };
        dc->IASetVertexBuffers(0, 1, mesh.vertexBuffer.GetAddressOf(), &stride, &offset);
        dc->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        dc->IASetInputLayout(defaultInputLayout.Get());

        if (replacedVertexShader)
            dc->VSSetShader(replacedVertexShader.Get(), nullptr, 0);
        else
            dc->VSSetShader(defaultVertexShader.Get(), nullptr, 0);
        if (replacedPixelShader)
            dc->PSSetShader(replacedPixelShader.Get(), nullptr, 0);
        else
            dc->PSSetShader(defaultPixelShader.Get(), nullptr, 0);


        if (keyframe && !keyframe->nodes.empty())
        {
            currentKeyframe = keyframe;

            const Animation::Keyframe::Node& meshNode{ keyframe->nodes.at(mesh.nodeIndex) };
            XMStoreFloat4x4(&constantDefault.world, XMLoadFloat4x4(&meshNode.globalTransform) * XMLoadFloat4x4(&world));

            const size_t boneCount{ mesh.bindPose.bones.size() };
            _ASSERT_EXPR(boneCount < MAX_BONES, L"The value of the 'bone_count' has exceeded MAX_BONES.");

            for (size_t boneIndex = 0; boneIndex < boneCount; ++boneIndex)
            {
                const Skeleton::Bone& bone{ mesh.bindPose.bones.at(boneIndex) };
                const Animation::Keyframe::Node& bone_node{ keyframe->nodes.at(bone.nodeIndex) };
                XMStoreFloat4x4(&constantDefault.boneTransforms[boneIndex],
                    XMLoadFloat4x4(&bone.offsetTransform) *
                    XMLoadFloat4x4(&bone_node.globalTransform) *
                    XMMatrixInverse(nullptr, XMLoadFloat4x4(&mesh.defaultGlobalTransform))
                );
            }
        }
        else
        {
            XMStoreFloat4x4(&constantDefault.world, XMLoadFloat4x4(&mesh.defaultGlobalTransform) * XMLoadFloat4x4(&world));
            for (size_t boneIndex = 0; boneIndex < MAX_BONES; ++boneIndex)
            {
                constantDefault.boneTransforms[boneIndex] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
            }
        }

        //フラスタムカリング
        //mesh.boundingBox->SetOwnerWorldMat(constantDefault.world);
        //if (frustum != nullptr)
        //{
        //    //カメラの視錐台と交差判定をして視錐台の外なら描画しない
        //    if (!(Collision::HitDetection3D(frustum, mesh.boundingBox.get())))
        //        continue;
        //}

        for (const Mesh::Subset& subset : mesh.subsets)
        {
            Material& material{ materials.at(subset.materialUniqueId) };

            XMStoreFloat4(&constantDefault.materialColor, XMLoadFloat4(&materialColor) * XMLoadFloat4(&material.Kd));

            defaultConstantBuffer.SetConstantBuffer(dc, ConstantBuffer::ShaderType::ALL, ConstantBuffer::UsageType::Object, &constantDefault);

            for (int i = 0; i < PBRMaxTexture; i++)
            {
                material.texture[i].SetPixelShader();
            }

            dc->DrawIndexed(subset.indexCount, subset.startIndexLocation, 0);
        }
    }

#ifdef USE_IMGUI
    //if (materials.size() > 1)
    //{
    //    ImGui::Begin("Skin");

    //    const Material& material{ materials.at(0) };
    //    ImGui::Image(material.texture[0].GetShaderResourceView(), { 128,128 });//536,7,8,9

    //    ImGui::End();
    //}
#endif

    //replacedVertexShader = nullptr;
    //replacedPixelShader = nullptr;

}

//void ModelData::DrawInstance(DirectX::XMFLOAT4X4 worlds[], int instanceCount, Animation::Keyframe* keyframe, Frustum* frustum, DirectX::XMFLOAT4 materialColor)
//{
//    SystemManager::Instance().SetRasterizerState(SystemManager::RasterizerState::SolidCullingOn);
//    SystemManager::Instance().SetDepthStencilState(SystemManager::DepthStencilState::SortOnWriteOn);
//
//    ID3D11DeviceContext* dc = SystemManager::Instance().GetDeviceContext();
//
//    assert(instanceCount <= MaxInstanceCount);
//
//    for (const Mesh& mesh : meshes)
//    {
//        uint32_t stride{ sizeof(Vertex) };
//        uint32_t offset{ 0 };
//        dc->IASetVertexBuffers(0, 1, mesh.vertexBuffer.GetAddressOf(), &stride, &offset);
//        dc->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
//        dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//        dc->IASetInputLayout(instancingInputLayout.Get());
//
//        //シェーダーをセット
//        if (replacedVertexShader)
//            dc->VSSetShader(replacedVertexShader.Get(), nullptr, 0);
//        else
//            dc->VSSetShader(instancingVertexShader.Get(), nullptr, 0);
//        if (replacedPixelShader)
//            dc->PSSetShader(replacedPixelShader.Get(), nullptr, 0);
//        else
//            dc->PSSetShader(instancingPixelShader.Get(), nullptr, 0);
//
//
//        if (keyframe && !keyframe->nodes.empty())
//        {
//            currentKeyframe = keyframe;
//
//            const Animation::Keyframe::Node& meshNode{ keyframe->nodes.at(mesh.nodeIndex) };
//
//            for (int i = 0; i < instanceCount; i++)
//            {
//                XMStoreFloat4x4(&constantInstancing.instancingData[i].world, XMLoadFloat4x4(&meshNode.globalTransform) * XMLoadFloat4x4(&worlds[i]));
//            }
//
//
//            const size_t bone_count{ mesh.bindPose.bones.size() };
//            _ASSERT_EXPR(bone_count < MAX_BONES, L"The value of the 'bone_count' has exceeded MAX_BONES.");
//
//            for (size_t bone_index = 0; bone_index < bone_count; ++bone_index)
//            {
//                const Skeleton::Bone& bone{ mesh.bindPose.bones.at(bone_index) };
//                const Animation::Keyframe::Node& bone_node{ keyframe->nodes.at(bone.nodeIndex) };
//                XMStoreFloat4x4(&constantInstancing.boneTransforms[bone_index],
//                    XMLoadFloat4x4(&bone.offsetTransform) *
//                    XMLoadFloat4x4(&bone_node.globalTransform) *
//                    XMMatrixInverse(nullptr, XMLoadFloat4x4(&mesh.defaultGlobalTransform))
//                );
//            }
//        }
//        else
//        {
//            for (int i = 0; i < instanceCount; i++)
//            {
//                XMStoreFloat4x4(&constantInstancing.instancingData[i].world, XMLoadFloat4x4(&mesh.defaultGlobalTransform) * XMLoadFloat4x4(&worlds[i]));
//            }
//            for (size_t bone_index = 0; bone_index < MAX_BONES; ++bone_index)
//            {
//                constantInstancing.boneTransforms[bone_index] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
//            }
//        }
//
//        //フラスタムカリング
//        //mesh.boundingBox->SetOwnerWorldMat(constant.world);
//        //if (frustum != nullptr)
//        //{
//        //    //カメラの視錐台と交差判定をして視錐台の外なら描画しない
//        //    if (!(Collision::HitDetection3D(frustum, mesh.boundingBox.get())))
//        //        continue;
//        //}
//
//        for (const Mesh::Subset& subset : mesh.subsets)
//        {
//            Material& material{ materials.at(subset.materialUniqueId) };
//
//            XMStoreFloat4(&constantInstancing.materialColor, XMLoadFloat4(&materialColor) * XMLoadFloat4(&material.Kd));
//
//            instancingConstantBuffer.SetConstantBuffer(dc, ConstantBuffer::ShaderType::ALL, ConstantBuffer::UsageType::Object, &constantInstancing);
//
//            for (int i = 0; i < PBRMaxTexture; i++)
//            {
//                material.texture[i].SetPixelShader(dc);
//            }
//
//            dc->DrawInstanced(subset.indexCount, instanceCount, subset.startIndexLocation, 0);
//        }
//    }
//
//#ifdef USE_IMGUI
//    //if (materials.size() > 1)
//    //{
//    //    ImGui::Begin("Skin");
//
//    //    const Material& material{ materials.at(0) };
//    //    ImGui::Image(material.texture[0].GetShaderResourceView(), { 128,128 });//536,7,8,9
//
//    //    ImGui::End();
//    //}
//#endif
//
//    //replacedVertexShader = nullptr;
//    //replacedPixelShader = nullptr;
//
//}


void ModelData::UpdateGlobalTransform(Animation::Keyframe& keyframe) const
{
    size_t node_count{ keyframe.nodes.size() };
    for (size_t node_index = 0; node_index < node_count; ++node_index)
    {
        Animation::Keyframe::Node& node{ keyframe.nodes.at(node_index) };
        XMMATRIX S{ XMMatrixScaling(node.scaling.x, node.scaling.y, node.scaling.z) };
        XMMATRIX R{ XMMatrixRotationQuaternion(XMLoadFloat4(&node.rotation)) };
        XMMATRIX T{ XMMatrixTranslation(node.translation.x, node.translation.y, node.translation.z) };

        int64_t parent_index{ sceneView.nodes.at(node_index).parentIndex };
        XMMATRIX P{ parent_index < 0 ? XMMatrixIdentity() : XMLoadFloat4x4(&keyframe.nodes.at(parent_index).globalTransform) };

        XMStoreFloat4x4(&node.globalTransform, S * R * T * P);
    }
}


void ModelData::FetchMeshes(FbxScene* fbxScene, std::vector<Mesh>& meshes) const
{
#if 1
    for (const ModelScene::Node& node : sceneView.nodes)
    {
        if (node.attribute != FbxNodeAttribute::EType::eMesh)
        {
            continue;
        }

        FbxNode* fbxNode{ fbxScene->FindNodeByName(node.name.c_str()) };
        FbxMesh* fbxMesh{ fbxNode->GetMesh() };

        Mesh& mesh{ meshes.emplace_back() };
        mesh.uniqueId = fbxMesh->GetNode()->GetUniqueID();
        mesh.name = fbxMesh->GetNode()->GetName();
        mesh.nodeIndex = sceneView.IndexOf(mesh.uniqueId);
        mesh.defaultGlobalTransform = ConvertToXMFloat4x4(fbxMesh->GetNode()->EvaluateGlobalTransform());

        //unit99
        DirectX::XMFLOAT4X4 geometricTransform = ConvertToXMFloat4x4(FbxAMatrix(
            fbxMesh->GetNode()->GetGeometricTranslation(FbxNode::eSourcePivot),
            fbxMesh->GetNode()->GetGeometricRotation(FbxNode::eSourcePivot),
            fbxMesh->GetNode()->GetGeometricScaling(FbxNode::eSourcePivot)
        ));
        DirectX::XMMATRIX GeometricTransform = DirectX::XMLoadFloat4x4(&geometricTransform);
        DirectX::XMStoreFloat4x4(&mesh.defaultGlobalTransform,
            DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&geometricTransform), DirectX::XMLoadFloat4x4(&mesh.defaultGlobalTransform)));

        //ボーンの影響
        std::vector<BoneInfluencesPerControlPoint> boneInfluences;
        FetchBoneInfluences(fbxMesh, boneInfluences);
        //FBXメッシュからバインドポーズの情報を抽出
        FetchSkeleton(fbxMesh, mesh.bindPose);

        std::vector<Mesh::Subset>& subsets{ mesh.subsets };
        const int material_count{ fbxMesh->GetNode()->GetMaterialCount() };
        subsets.resize((material_count > 0) ? material_count : 1);
        for (int material_index = 0; material_index < material_count; ++material_index)
        {
            const FbxSurfaceMaterial* fbx_material{ fbxMesh->GetNode()->GetMaterial(material_index) };
            subsets.at(material_index).materialName = fbx_material->GetName();
            subsets.at(material_index).materialUniqueId = fbx_material->GetUniqueID();
        }
        if (material_count > 0)
        {
            const int polygon_count{ fbxMesh->GetPolygonCount() };
            for (int polygon_index = 0; polygon_index < polygon_count; ++polygon_index)
            {
                const int material_index
                { fbxMesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index) };
                subsets.at(material_index).indexCount += 3;
            }
            uint32_t offset{ 0 };
            for (Mesh::Subset& subset : subsets)
            {
                subset.startIndexLocation = offset;
                offset += subset.indexCount;
                // This will be used as counter in the following procedures, reset to zero
                subset.indexCount = 0;
            }
        }

        const int polygonCount{ fbxMesh->GetPolygonCount() };

        mesh.vertices.resize(polygonCount * 3LL);
        mesh.indices.resize(polygonCount * 3LL);

        FbxStringList uv_names;
        fbxMesh->GetUVSetNames(uv_names);

        const FbxVector4* controlPoints{ fbxMesh->GetControlPoints() };

        for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
        {
            const int material_index{ material_count > 0 ? fbxMesh->GetElementMaterial()->GetIndexArray().GetAt(polygonIndex) : 0 };
            Mesh::Subset& subset{ subsets.at(material_index) };
            const uint32_t offset{ subset.startIndexLocation + subset.indexCount };

            for (int positionInPolygon = 0; positionInPolygon < 3; ++positionInPolygon)
            {
                const int vertexIndex{ polygonIndex * 3 + positionInPolygon };

                Vertex vertex;
                const int polygonVertex{ fbxMesh->GetPolygonVertex(polygonIndex, positionInPolygon) };
                vertex.position.x = static_cast<float>(controlPoints[polygonVertex][0]);
                vertex.position.y = static_cast<float>(controlPoints[polygonVertex][1]);
                vertex.position.z = static_cast<float>(controlPoints[polygonVertex][2]);
                DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&vertex.position);
                Position = DirectX::XMVector3TransformCoord(Position, GeometricTransform);
                DirectX::XMStoreFloat3(&vertex.position, Position);

                const BoneInfluencesPerControlPoint& influencesPerControlPoint{ boneInfluences.at(polygonVertex) };
                // ウェイトの合計を出す
                float boneWeightTotal = 0;
                for (size_t influenceIndex = 0; influenceIndex < influencesPerControlPoint.size(); ++influenceIndex)
                {
                    boneWeightTotal += influencesPerControlPoint.at(influenceIndex).boneWeight;
                }
                for (size_t influenceIndex = 0; influenceIndex < influencesPerControlPoint.size(); ++influenceIndex)
                {

#if 0               // ※影響を受けるボーンは最大４つまでとする
                    // ※下記コードは影響を受けるボーン数が４つを超える場合は、アサートを出している
                    assert(influence_index < MAX_BONE_INFLUENCES && "!ボーンの影響数が4を超えています!");
                    {
                        vertex.boneWeights[influence_index] = influences_per_control_point.at(influence_index).boneWeight / bone_weight_total; // 正規化
                        vertex.boneIndices[influence_index] = influences_per_control_point.at(influence_index).boneIndex;
                    }
#else               // ※影響を受けるボーンが４つ超えてても大丈夫
                    if (influenceIndex < MAX_BONE_INFLUENCES)
                    {
                        vertex.boneWeights[influenceIndex] = influencesPerControlPoint.at(influenceIndex).boneWeight / boneWeightTotal;
                        vertex.boneIndices[influenceIndex] = influencesPerControlPoint.at(influenceIndex).boneIndex;
                    }
                    else
                    {
                        size_t minimumValueIndex = 0;
                        float minimumValue = FLT_MAX;
                        for (size_t i = 0; i < MAX_BONE_INFLUENCES; ++i)
                        {
                            if (minimumValue > vertex.boneWeights[i])
                            {
                                minimumValue = vertex.boneWeights[i];
                                minimumValueIndex = i;
                            }
                        }
                        vertex.boneWeights[minimumValueIndex] += influencesPerControlPoint.at(influenceIndex).boneWeight / boneWeightTotal;
                        vertex.boneIndices[minimumValueIndex] = influencesPerControlPoint.at(influenceIndex).boneIndex;
                    }
#endif
                }

                if (fbxMesh->GetElementNormalCount() > 0)
                {
                    FbxVector4 normal;
                    fbxMesh->GetPolygonVertexNormal(polygonIndex, positionInPolygon, normal);
                    vertex.normal.x = static_cast<float>(normal[0]);
                    vertex.normal.y = static_cast<float>(normal[1]);
                    vertex.normal.z = static_cast<float>(normal[2]);
                }
                if (fbxMesh->GetElementUVCount() > 0)
                {
                    FbxVector2 uv;
                    bool unmappedUv;
                    fbxMesh->GetPolygonVertexUV(polygonIndex, positionInPolygon,
                        uv_names[0], uv, unmappedUv);
                    vertex.texCoord.x = static_cast<float>(uv[0]);
                    vertex.texCoord.y = 1.0f - static_cast<float>(uv[1]);
                }

                //法線ベクトル取得
                if (fbxMesh->GenerateTangentsData(0, false))
                {
                    const FbxGeometryElementTangent* tangent = fbxMesh->GetElementTangent(0);
                    vertex.tangent.x = static_cast<float>(tangent->GetDirectArray().GetAt(vertexIndex)[0]);
                    vertex.tangent.y = static_cast<float>(tangent->GetDirectArray().GetAt(vertexIndex)[1]);
                    vertex.tangent.z = static_cast<float>(tangent->GetDirectArray().GetAt(vertexIndex)[2]);
                    vertex.tangent.w = static_cast<float>(tangent->GetDirectArray().GetAt(vertexIndex)[3]);
                }

                mesh.vertices.at(vertexIndex) = std::move(vertex);
                mesh.indices.at(static_cast<size_t>(offset) + positionInPolygon) = vertexIndex;
                subset.indexCount++;
            }

        }
    }
#else
    // Fetch all meshes from the scene.
    for (const ModelScene::Node& node : sceneView.nodes)
    {
        // Skip if node attribute is not eMesh.
        if (node.attribute != FbxNodeAttribute::EType::eMesh)
        {
            continue;
        }

        FbxNode* fbx_node{ fbx_scene->FindNodeByName(node.name.c_str()) };
        FbxMesh* fbx_mesh{ fbx_node->GetMesh() };

        Mesh& mesh{ meshes.emplace_back() };
        mesh.uniqueId = fbx_mesh->GetNode()->GetUniqueID();
        mesh.name = fbx_mesh->GetNode()->GetName();
        mesh.nodeIndex = sceneView.IndexOf(mesh.uniqueId);

        // UNIT.20
        // Build subsets for each material
        std::vector<Mesh::Subset>& subsets{ mesh.subsets };
        const int material_count{ fbx_mesh->GetNode()->GetMaterialCount() };
        subsets.resize(material_count > 0 ? material_count : 1);
        for (int material_index = 0; material_index < material_count; ++material_index)
        {
            const FbxSurfaceMaterial* fbx_material{ fbx_mesh->GetNode()->GetMaterial(material_index) };
            subsets.at(material_index).materialName = fbx_material->GetName();
            subsets.at(material_index).materialUniqueId = fbx_material->GetUniqueID();
        }
        if (material_count > 0)
        {
            // Count the faces of each material
            const int polygon_count{ fbx_mesh->GetPolygonCount() };
            for (int polygon_index = 0; polygon_index < polygon_count; ++polygon_index)
            {
                const int material_index{ fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index) };
                subsets.at(material_index).indexCount += 3;
            }

            // Record the offset (How many vertex)
            uint32_t offset{ 0 };
            for (Mesh::Subset& subset : subsets)
            {
                subset.startIndexLocation = offset;
                offset += subset.indexCount;
                // This will be used as counter in the following procedures, reset to zero
                subset.indexCount = 0;
            }
        }

        const int polygon_count{ fbx_mesh->GetPolygonCount() };
        mesh.vertices.resize(polygon_count * 3LL);
        mesh.indices.resize(polygon_count * 3LL);

        FbxStringList uv_names;
        fbx_mesh->GetUVSetNames(uv_names);
        const FbxVector4* control_points{ fbx_mesh->GetControlPoints() };
        for (int polygon_index = 0; polygon_index < polygon_count; ++polygon_index)
        {
            // UNIT.20
            const int material_index{ material_count > 0 ? fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index) : 0 };
            Mesh::Subset& subset{ subsets.at(material_index) };
            const uint32_t offset{ subset.startIndexLocation + subset.indexCount };

            for (int position_in_polygon = 0; position_in_polygon < 3; ++position_in_polygon)
            {
                const int vertex_index{ polygon_index * 3 + position_in_polygon };

                Vertex vertex;
                const int polygon_vertex{ fbx_mesh->GetPolygonVertex(polygon_index, position_in_polygon) };
                vertex.position.x = static_cast<float>(control_points[polygon_vertex][0]);
                vertex.position.y = static_cast<float>(control_points[polygon_vertex][1]);
                vertex.position.z = static_cast<float>(control_points[polygon_vertex][2]);

                if (fbx_mesh->GetElementNormalCount() > 0)
                {
                    FbxVector4 normal;
                    fbx_mesh->GetPolygonVertexNormal(polygon_index, position_in_polygon, normal);
                    vertex.normal.x = static_cast<float>(normal[0]);
                    vertex.normal.y = static_cast<float>(normal[1]);
                    vertex.normal.z = static_cast<float>(normal[2]);
                }

                if (fbx_mesh->GetElementUVCount() > 0)
                {
                    FbxVector2 uv;
                    bool unmapped_uv;
                    fbx_mesh->GetPolygonVertexUV(polygon_index, position_in_polygon, uv_names[0], uv, unmapped_uv);
                    vertex.texcoord.x = static_cast<float>(uv[0]);
                    vertex.texcoord.y = 1.0f - static_cast<float>(uv[1]);
                }

                mesh.vertices.at(vertex_index) = std::move(vertex);
                // UNIT.20
#if 0
                mesh.indices.at(vertex_index) = vertex_index;
#else
                mesh.indices.at(static_cast<size_t>(offset) + position_in_polygon) = vertex_index;
                subset.indexCount++;
#endif
            }

        }
    }

#endif

}

void ModelData::FetchMaterials(FbxScene* fbxScene, std::unordered_map<uint64_t, Material>& materials) const
{
    const size_t node_count{ sceneView.nodes.size() };
    for (size_t node_index = 0; node_index < node_count; ++node_index)
    {
        const ModelScene::Node& node{ sceneView.nodes.at(node_index) };
        const FbxNode* fbx_node{ fbxScene->FindNodeByName(node.name.c_str()) };

        const int material_count{ fbx_node->GetMaterialCount() };
        for (int material_index = 0; material_index < material_count; ++material_index)
        {
            const FbxSurfaceMaterial* fbx_material{ fbx_node->GetMaterial(material_index) };

            Material material;
            material.name = fbx_material->GetName();
            material.uniqueId = fbx_material->GetUniqueID();
            FbxProperty fbx_property;
            fbx_property = fbx_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
            //BaseColor
            if (fbx_property.IsValid())
            {
                const FbxDouble3 color{ fbx_property.Get<FbxDouble3>() };
                material.Kd.x = static_cast<float>(color[0]);
                material.Kd.y = static_cast<float>(color[1]);
                material.Kd.z = static_cast<float>(color[2]);
                material.Kd.w = 1.0f;

                const FbxFileTexture* fbx_texture{ fbx_property.GetSrcObject<FbxFileTexture>() };
                material.textureFilenames[static_cast<int>(Texture::Type::BaseColor)] = fbx_texture ? fbx_texture->GetRelativeFileName() : "";
            }
            //NormalMap
            fbx_property = fbx_material->FindProperty(FbxSurfaceMaterial::sNormalMap);
            if (fbx_property.IsValid())
            {
                const FbxFileTexture* file_texture{ fbx_property.GetSrcObject<FbxFileTexture>() };
                material.textureFilenames[static_cast<int>(Texture::Type::Normal)] = file_texture ? file_texture->GetRelativeFileName() : "";
            }

            //Ambient
            fbx_property = fbx_material->FindProperty(FbxSurfaceMaterial::sAmbient);
            if (fbx_property.IsValid())
            {
                const FbxDouble3 color{ fbx_property.Get<FbxDouble3>() };
                material.Ka.x = static_cast<float>(color[0]);
                material.Ka.y = static_cast<float>(color[1]);
                material.Ka.z = static_cast<float>(color[2]);
                material.Ka.w = 1.0f;
            }

            //Specular
            fbx_property = fbx_material->FindProperty(FbxSurfaceMaterial::sSpecular);
            if (fbx_property.IsValid())
            {
                const FbxDouble3 color{ fbx_property.Get<FbxDouble3>() };
                material.Ks.x = static_cast<float>(color[0]);
                material.Ks.y = static_cast<float>(color[1]);
                material.Ks.z = static_cast<float>(color[2]);
                material.Ks.w = 1.0f;
            }

            materials.emplace(material.uniqueId, std::move(material));
        }
        //materialがなかったらダミーを作る
        if (material_count <= 0)
        {
            materials.emplace();
        }
    }
}
void ModelData::FetchPBRMaterials(FbxScene* fbxScene, const char* fbxFilename, std::unordered_map<uint64_t, Material>& materials) const
{
    const size_t nodeCount{ sceneView.nodes.size() };
    for (size_t nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
    {
        const ModelScene::Node& node{ sceneView.nodes.at(nodeIndex) };
        const FbxNode* fbxNode{ fbxScene->FindNodeByName(node.name.c_str()) };

        const int materialCount{ fbxNode->GetMaterialCount() };
        for (int materialIndex = 0; materialIndex < materialCount; ++materialIndex)
        {
            const FbxSurfaceMaterial* fbxMaterial{ fbxNode->GetMaterial(materialIndex) };

            Material material;
            material.name = fbxMaterial->GetName();
            material.uniqueId = fbxMaterial->GetUniqueID();
            FbxProperty fbxProperty;
            fbxProperty = fbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
            //BaseColor
            {
#if 1
                if (fbxProperty.IsValid())
                {
                    const FbxFileTexture* fbxTexture{ fbxProperty.GetSrcObject<FbxFileTexture>() };
                    material.textureFilenames[static_cast<int>(Texture::Type::BaseColor)] = fbxTexture ? fbxTexture->GetRelativeFileName() : "";
                }
#else//強制的にパスを設定
                std::filesystem::path path(fbxFilename);//"./Resources/~~.fbx"
                path.replace_extension("fbm");//"./Resources/~~.fbm"
                path = path.filename();//~~.fbm
                std::string BaseColorName = "_BaseColorTexture.png";//マテリアル名の後ろ
                path += "\\" + BaseColorName;
                material.textureFilenames[static_cast<int>(Texture::Type::BaseColor)] = path.string();
#endif
            }
            //NormalMap
            {
#if 1
                fbxProperty = fbxMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);
                if (fbxProperty.IsValid())
                {
                    const FbxFileTexture* fileTexture{ fbxProperty.GetSrcObject<FbxFileTexture>() };
                    material.textureFilenames[static_cast<int>(Texture::Type::Normal)] = fileTexture ? fileTexture->GetRelativeFileName() : "";
                }
                else
                {
                    std::filesystem::path path(fbxFilename);//"./Resources/~~.fbx"
                    path.replace_extension("fbm");//"./Resources/~~.fbm"
                    std::filesystem::path texturePath = path.filename();//~~.fbm
                    std::string normalName = "_Normal_DirectX.png";//マテリアル名の後ろ
                    path += "\\" + material.name + normalName;//"./Resources/~.fbm/~_Normal_DirectX.png"
                    if (std::filesystem::exists(path.c_str())) //そのファイル名のものがあるか調べる
                    {
                        texturePath += "\\" + material.name + normalName;//~.fbm/~_Normal_DirectX.png"
                        material.textureFilenames[static_cast<int>(Texture::Type::Normal)] = texturePath.string();
                    }
                    else
                        material.textureFilenames[static_cast<int>(Texture::Type::Normal)] = "";
                }
#else//強制的にパスを設定
                std::filesystem::path path(fbxFilename);//"./Resources/~~.fbx"
                path.replace_extension("fbm");//"./Resources/~~.fbm"
                path = path.filename();//~~.fbm
                std::string normalName = "vanguard_normal.png";//マテリアル名の後ろ
                path += "\\" + normalName;
                material.textureFilenames[static_cast<int>(Texture::Type::Normal)] = path.string();
#endif
            }

            //AO
            {
                std::filesystem::path path(fbxFilename);//"./Resources/~~.fbx"
                path.replace_extension("fbm");//"./Resources/~~.fbm"
                std::filesystem::path texturePath = path.filename();//~~.fbm
                std::string ambientName = "_Mixed_AO.png";//マテリアル名の後ろ
                path += "\\" + material.name + ambientName;//"./Resources/~.fbm/~_Mixed_AO.png"
                if (std::filesystem::exists(path.c_str())) //そのファイル名のものがあるか調べる
                {
                    texturePath += "\\" + material.name + ambientName;//~.fbm/~_Mixed_AO.png"
                    material.textureFilenames[static_cast<int>(Texture::Type::Ambient)] = texturePath.string();
                }
                else
                    material.textureFilenames[static_cast<int>(Texture::Type::Ambient)] = "";
            }

            //Emissive
            {
#if 1
                std::filesystem::path path(fbxFilename);//"./Resources/~~.fbx"
                path.replace_extension("fbm");//"./Resources/~~.fbm"
                std::filesystem::path texturePath = path.filename();//~~.fbm
                std::string emissiveName = "_Emissive.png";//マテリアル名の後ろ
                path += "\\" + material.name + emissiveName;//"./Resources/~.fbm/~_Emissive.png"
                if (std::filesystem::exists(path.c_str())) //そのファイル名のものがあるか調べる
                {
                    texturePath += "\\" + material.name + emissiveName;//~.fbm/~_Emissive.png"
                    material.textureFilenames[static_cast<int>(Texture::Type::Emissive)] = texturePath.string();
                }
                else
                    material.textureFilenames[static_cast<int>(Texture::Type::Emissive)] = "";

#else//強制的にパスを設定
                std::filesystem::path path(fbxFilename);//"./Resources/~~.fbx"
                path.replace_extension("fbm");//"./Resources/~~.fbm"
                path = path.filename();//~~.fbm
                std::string normalName = "EmissiveTexture.png";//マテリアル名の後ろ
                path += "\\" + normalName;
                material.textureFilenames[static_cast<int>(Texture::Type::Emissive)] = path.string();
#endif
            }

            //Metallic
            {
#if 1
                std::filesystem::path path(fbxFilename);//"./Resources/~~.fbx"
                path.replace_extension("fbm");//"./Resources/~~.fbm"
                std::filesystem::path texturePath = path.filename();//~~.fbm
                std::string metallicName = "_Metallic.png";//マテリアル名の後ろ
                path += "\\" + material.name + metallicName;//"./Resources/~.fbm/~_Metallic.png"
                if (std::filesystem::exists(path.c_str())) //そのファイル名のものがあるか調べる
                {
                    texturePath += "\\" + material.name + metallicName;//~.fbm/~_Metallic.png"
                    material.textureFilenames[static_cast<int>(Texture::Type::Metallic)] = texturePath.string();
                }
                else
                    material.textureFilenames[static_cast<int>(Texture::Type::Metallic)] = "";
#else//強制的にパスを設定
                std::filesystem::path path(fbxFilename);//"./Resources/~~.fbx"
                path.replace_extension("fbm");//"./Resources/~~.fbm"
                path = path.filename();//~~.fbm
                std::string normalName = "kari_lambert1_Metalness.png";
                path += "\\" + normalName;
                material.textureFilenames[static_cast<int>(Texture::Type::Metallic)] = path.string();
#endif
            }

            //Roughness
            {
#if 1
                std::filesystem::path path(fbxFilename);//"./Resources/~~.fbx"
                path.replace_extension("fbm");//"./Resources/~~.fbm"
                std::filesystem::path texturePath = path.filename();//~~.fbm
                std::string roughnessName = "_Roughness.png";//マテリアル名の後ろ
                path += "\\" + material.name + roughnessName;//"./Resources/~.fbm/~_Roughness.png"
                if (std::filesystem::exists(path.c_str())) //そのファイル名のものがあるか調べる
                {
                    texturePath += "\\" + material.name + roughnessName;//~.fbm/~_Roughness.png"
                    material.textureFilenames[static_cast<int>(Texture::Type::Roughness)] = texturePath.string();
                }
                else
                    material.textureFilenames[static_cast<int>(Texture::Type::Roughness)] = "";
#else//強制的にパスを設定
                std::filesystem::path path(fbxFilename);//"./Resources/~~.fbx"
                path.replace_extension("fbm");//"./Resources/~~.fbm"
                path = path.filename();//~~.fbm
                std::string normalName = "kari_lambert1_Roughness.png";
                path += "\\" + normalName;
                material.textureFilenames[static_cast<int>(Texture::Type::Roughness)] = path.string();
#endif
            }

            materials.emplace(material.uniqueId, std::move(material));
        }

    }
    //materialがなかったらダミーを作る
    if (materials.empty())
    {
        materials.emplace();
    }
}


void ModelData::CreateComObjects(const char* fbxFilename)
{
    ID3D11Device* device = SystemManager::Instance().GetDevice();

    for (Mesh& mesh : meshes)
    {
        HRESULT hr{ S_OK };
        D3D11_BUFFER_DESC bufferDesc{};
        D3D11_SUBRESOURCE_DATA subresourceData{};
        bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;
        subresourceData.pSysMem = mesh.vertices.data();
        subresourceData.SysMemPitch = 0;
        subresourceData.SysMemSlicePitch = 0;
        hr = device->CreateBuffer(&bufferDesc, &subresourceData,
            mesh.vertexBuffer.ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));

        bufferDesc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * mesh.indices.size());
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        subresourceData.pSysMem = mesh.indices.data();
        hr = device->CreateBuffer(&bufferDesc, &subresourceData,
            mesh.indexBuffer.ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HrTrace(hr));


        //バウンディングボックス
        //for (const Vertex& v : mesh.vertices)
        //{
        //    mesh.minPos.x = std::min<float>(mesh.minPos.x, v.position.x);
        //    mesh.minPos.y = std::min<float>(mesh.minPos.y, v.position.y);
        //    mesh.minPos.z = std::min<float>(mesh.minPos.z, v.position.z);

        //    mesh.maxPos.x = std::max<float>(mesh.maxPos.x, v.position.x);
        //    mesh.maxPos.y = std::max<float>(mesh.maxPos.y, v.position.y);
        //    mesh.maxPos.z = std::max<float>(mesh.maxPos.z, v.position.z);
        //}
        //mesh.boundingBox = std::make_unique<BoundingBox>(mesh.minPos, mesh.maxPos);

#if 0
        mesh.vertices.clear();
        mesh.indices.clear();
#endif
    }

    //シェーダーリソースビューオブジェクト生成
    for (std::unordered_map<uint64_t, Material>::iterator iterator = materials.begin(); iterator != materials.end(); ++iterator)
    {
        for (int i = 0; i < PBRMaxTexture; i++)
        {
            if (!iterator->second.textureFilenames[i].empty())
            {
                std::filesystem::path path(fbxFilename);
                path.replace_filename(iterator->second.textureFilenames[i]);
                iterator->second.texture[i].Initialize(path.c_str(), static_cast<Texture::Type>(i), i);
            }
            else
            {
                iterator->second.texture[i].Initialize(nullptr, Texture::Type::None, i);
            }
        }

    }


    D3D11_INPUT_ELEMENT_DESC inputElementDesc[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT },
        { "BONES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT },
    };
    //通常描画時の頂点シェーダーを生成
    CreateVsFromCso(".\\Shaders\\3D_VS.cso", defaultVertexShader.ReleaseAndGetAddressOf(),
        defaultInputLayout.ReleaseAndGetAddressOf(), inputElementDesc, ARRAYSIZE(inputElementDesc));

    D3D11_INPUT_ELEMENT_DESC instanceInputElementDesc[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT },
        { "BONES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT },
    };
    //通常描画時のピクセルシェーダーを生成
	CreatePsFromCso(".\\Shaders\\3D_PS.cso", defaultPixelShader.ReleaseAndGetAddressOf());

    replacedVertexShader = nullptr;
    replacedPixelShader = nullptr;

    defaultConstantBuffer.Initialize(device, &constantDefault);
    instancingConstantBuffer.Initialize(device, &constantInstancing);
}

void ModelData::FetchSkeleton(FbxMesh* fbxMesh, Skeleton& bindPose) const
{
    const int deformerCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
    for (int deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
    {
        FbxSkin* skin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
        const int cluster_count = skin->GetClusterCount();
        bindPose.bones.resize(cluster_count);
        for (int cluster_index = 0; cluster_index < cluster_count; ++cluster_index)
        {
            FbxCluster* cluster = skin->GetCluster(cluster_index);

            Skeleton::Bone& bone{ bindPose.bones.at(cluster_index) };
            bone.name = cluster->GetLink()->GetName();
            bone.uniqueId = cluster->GetLink()->GetUniqueID();
            bone.parentIndex = bindPose.IndexOf(cluster->GetLink()->GetParent()->GetUniqueID());
            bone.nodeIndex = sceneView.IndexOf(bone.uniqueId);

            //'reference_global_init_position' is used to convert from local space of model(mesh) to
            // global space of scene.
            FbxAMatrix referenceGlobalInitPosition;
            cluster->GetTransformMatrix(referenceGlobalInitPosition);

            // 'cluster_global_init_position' is used to convert from local space of bone to
                // global space of scene.
            FbxAMatrix clusterGlobalInitPosition;
            cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);

            // Matrices are defined using the Column Major scheme. When a FbxAMatrix represents a transformation
                // (translation, rotation and scale), the last row of the matrix represents the translation part of
                // the transformation.
                // Compose 'bone.offset_transform' matrix that trnasforms position from mesh space to bone space.
                // This matrix is called the offset matrix.
            bone.offsetTransform
                = ConvertToXMFloat4x4(clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition);
        }
    }
}

void ModelData::FetchAnimations(FbxScene* fbxScene, std::vector<Animation>& animationClips,
    float samplingRate /*If this value is 0, the animation data will be sampled at the default frame rate.*/) const
{
    FbxArray<FbxString*> animationStackNames;
    fbxScene->FillAnimStackNameArray(animationStackNames);
    const int animationStackCount{ animationStackNames.GetCount() };

    for (int animationStackIndex = 0; animationStackIndex < animationStackCount; ++animationStackIndex)
    {
        Animation& animationClip{ animationClips.emplace_back() };
        animationClip.name = animationStackNames[animationStackIndex]->Buffer();

        FbxAnimStack* animationStack{ fbxScene->FindMember<FbxAnimStack>(animationClip.name.c_str()) };
        fbxScene->SetCurrentAnimationStack(animationStack);

        const FbxTime::EMode timeMode{ fbxScene->GetGlobalSettings().GetTimeMode() };
        FbxTime oneSecond;
        oneSecond.SetTime(0, 0, 1, 0, 0, timeMode);
        animationClip.samplingRate = samplingRate > 0 ?
            samplingRate : static_cast<float>(oneSecond.GetFrameRate(timeMode));
        const FbxTime samplingInterval
        { static_cast<FbxLongLong>(oneSecond.Get() / animationClip.samplingRate) };
        const FbxTakeInfo* takeInfo{ fbxScene->GetTakeInfo(animationClip.name.c_str()) };
        const FbxTime startTime{ takeInfo->mLocalTimeSpan.GetStart() };
        const FbxTime stopTtime{ takeInfo->mLocalTimeSpan.GetStop() };
        for (FbxTime time = startTime; time < stopTtime; time += samplingInterval)
        {
            Animation::Keyframe& keyframe{ animationClip.sequence.emplace_back() };

            const size_t nodeCount{ sceneView.nodes.size() };
            keyframe.nodes.resize(nodeCount);
            for (size_t node_index = 0; node_index < nodeCount; ++node_index)
            {
                FbxNode* fbxNode{ fbxScene->FindNodeByName(sceneView.nodes.at(node_index).name.c_str()) };
                if (fbxNode)
                {
                    Animation::Keyframe::Node& node{ keyframe.nodes.at(node_index) };
                    // 'globalTransform' is a transformation matrix of a node with respect to
                     // the scene's global coordinate system.
                    node.globalTransform = ConvertToXMFloat4x4(fbxNode->EvaluateGlobalTransform(time));

                    // 'local_transform' is a transformation matrix of a node with respect to
                    // its parent's local coordinate system.
                    const FbxAMatrix& local_transform{ fbxNode->EvaluateLocalTransform(time) };
                    node.scaling = ConvertToXMFloat3(local_transform.GetS());
                    node.rotation = ConvertToXMFloat4(local_transform.GetQ());
                    node.translation = ConvertToXMFloat3(local_transform.GetT());
                }
            }
        }
    }
    for (int animationStackIndex = 0; animationStackIndex < animationStackCount; ++animationStackIndex)
    {
        delete animationStackNames[animationStackIndex];
    }
}

bool ModelData::AppendAnimation(const char* animationFilename, float samplingRate)
{
    std::filesystem::path cerealFilename(animationFilename);
    cerealFilename.replace_extension("anim");
    if (std::filesystem::exists(cerealFilename.c_str()))
    {
        std::ifstream ifs(cerealFilename.c_str(), std::ios::binary);
        cereal::BinaryInputArchive deserialization(ifs);
        deserialization(animationClips);

    }
    else
    {
        FbxManager* fbxManager{ FbxManager::Create() };
        FbxScene* fbxScene{ FbxScene::Create(fbxManager, "") };

        FbxImporter* fbxImporter{ FbxImporter::Create(fbxManager, "") };
        bool importStatus{ false };
        importStatus = fbxImporter->Initialize(animationFilename);
        _ASSERT_EXPR_A(importStatus, fbxImporter->GetStatus().GetErrorString());
        importStatus = fbxImporter->Import(fbxScene);
        _ASSERT_EXPR_A(importStatus, fbxImporter->GetStatus().GetErrorString());

        FetchAnimations(fbxScene, animationClips, samplingRate);

        fbxManager->Destroy();

        std::ofstream ofs(cerealFilename.c_str(), std::ios::binary);
        cereal::BinaryOutputArchive serialization(ofs);
        serialization(animationClips);
    }
    return true;
}
void ModelData::BlendAnimations(const Animation::Keyframe* keyframes[2], float factor, Animation::Keyframe& keyframe)
{
    size_t nodeCount{ keyframes[0]->nodes.size() };
    keyframe.nodes.resize(nodeCount);

    for (size_t nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
    {
        XMVECTOR S[2]{
            XMLoadFloat3(&(keyframes[0]->nodes.at(nodeIndex).scaling)),
            XMLoadFloat3(&(keyframes[1]->nodes.at(nodeIndex).scaling)) };
        XMStoreFloat3(&keyframe.nodes.at(nodeIndex).scaling, XMVectorLerp(S[0], S[1], factor));

        XMVECTOR R[2]{
            XMLoadFloat4(&(keyframes[0]->nodes.at(nodeIndex).rotation)),
            XMLoadFloat4(&(keyframes[1]->nodes.at(nodeIndex).rotation)) };
        XMStoreFloat4(&keyframe.nodes.at(nodeIndex).rotation, XMQuaternionSlerp(R[0], R[1], factor));

        XMVECTOR T[2]{
            XMLoadFloat3(&(keyframes[0]->nodes.at(nodeIndex).translation)),
            XMLoadFloat3(&(keyframes[1]->nodes.at(nodeIndex).translation)) };
        XMStoreFloat3(&keyframe.nodes.at(nodeIndex).translation, XMVectorLerp(T[0], T[1], factor));
    }
}

DirectX::XMFLOAT4X4 ModelData::SearchBoneMatrix(std::string name)
{
    for (const Mesh& mesh : meshes)
    {
        const size_t boneSize{ mesh.bindPose.bones.size() };
        for (size_t boneIndex = 0; boneIndex < boneSize; ++boneIndex)
        {
            const Skeleton::Bone& bone{ mesh.bindPose.bones.at(boneIndex) };
            if (bone.name == name)
            {
                DirectX::XMFLOAT4X4 mat{};

                const Animation::Keyframe::Node& boneNode{ currentKeyframe->nodes.at(bone.nodeIndex) };
                XMStoreFloat4x4(&mat, XMLoadFloat4x4(&bone.offsetTransform) *
                    XMLoadFloat4x4(&boneNode.globalTransform) *
                    XMMatrixInverse(nullptr, XMLoadFloat4x4(&mesh.defaultGlobalTransform)));

                return mat;
            }
        }
    }

    return { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
}


//----------グローバルスコープ------------//
void FetchBoneInfluences(const FbxMesh* fbxMesh,
    std::vector<BoneInfluencesPerControlPoint>& boneInfluences)
{
    const int controlPointsCount{ fbxMesh->GetControlPointsCount() };
    boneInfluences.resize(controlPointsCount);

    const int skinCount{ fbxMesh->GetDeformerCount(FbxDeformer::eSkin) };
    for (int skinIndex = 0; skinIndex < skinCount; ++skinIndex)
    {
        const FbxSkin* fbxSkin{ static_cast<FbxSkin*>(fbxMesh->GetDeformer(skinIndex, FbxDeformer::eSkin)) };

        const int clusterCount{ fbxSkin->GetClusterCount() };
        for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
        {
            const FbxCluster* fbxCluster{ fbxSkin->GetCluster(clusterIndex) };

            const int controlPointIndicesCount{ fbxCluster->GetControlPointIndicesCount() };
            for (int controlPointIndicesIndex = 0; controlPointIndicesIndex < controlPointIndicesCount;
                ++controlPointIndicesIndex)
            {
                const int controlPointIndex{ fbxCluster->GetControlPointIndices()[controlPointIndicesIndex] };
                const double controlPointWeight
                { fbxCluster->GetControlPointWeights()[controlPointIndicesIndex] };
                BoneInfluence& boneInfluence{ boneInfluences.at(controlPointIndex).emplace_back() };
                boneInfluence.boneIndex = static_cast<uint32_t>(clusterIndex);
                boneInfluence.boneWeight = static_cast<float>(controlPointWeight);
            }
        }
    }
}
