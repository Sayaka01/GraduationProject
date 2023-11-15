#pragma once
#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include<vector>
#include <string>
#include <fbxsdk.h>
#include <unordered_map>

#include <cereal/archives/binary.hpp>

#include "System/ConstantBuffer.h"

#include "System/Texture.h"


//シリアライズ
namespace DirectX
{
	template<class T>
	void serialize(T& archive, DirectX::XMFLOAT2& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y)
		);
	}
	template<class T>
	void serialize(T& archive, DirectX::XMFLOAT3& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z)
		);
	}
	template<class T>
	void serialize(T& archive, DirectX::XMFLOAT4& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z),
			cereal::make_nvp("w", v.w)
		);
	}
	template<class T>
	void serialize(T& archive, DirectX::XMFLOAT4X4& m)
	{
		archive(
			cereal::make_nvp("_11", m._11), cereal::make_nvp("_12", m._12),
			cereal::make_nvp("_13", m._13), cereal::make_nvp("_14", m._14),
			cereal::make_nvp("_21", m._21), cereal::make_nvp("_22", m._22),
			cereal::make_nvp("_23", m._23), cereal::make_nvp("_24", m._24),
			cereal::make_nvp("_31", m._31), cereal::make_nvp("_32", m._32),
			cereal::make_nvp("_33", m._33), cereal::make_nvp("_34", m._34),
			cereal::make_nvp("_41", m._41), cereal::make_nvp("_42", m._42),
			cereal::make_nvp("_43", m._43), cereal::make_nvp("_44", m._44)
		);
	}
}


struct ModelScene
{
	struct Node
	{
		uint64_t uniqueId{ 0 };
		std::string name;
		FbxNodeAttribute::EType attribute{ FbxNodeAttribute::EType::eUnknown };
		int64_t parentIndex{ -1 };

		//シリアライズ
		template<class T>
		void serialize(T& archive)
		{
			archive(uniqueId, name, attribute, parentIndex);
		}

	};
	std::vector<Node> nodes;

	[[nodiscard]] int64_t IndexOf(uint64_t uniqueId)const
	{
		int64_t index{ 0 };
		for (const Node& node : nodes)
		{
			if (node.uniqueId == uniqueId)
			{
				return index;
			}
			++index;
		}
		return -1;
	}

	//シリアライズ
	template<class T>
	void serialize(T& archive)
	{
		archive(nodes);
	}

};

struct Animation
{
	std::string name;
	float samplingRate{ 0 };

	struct Keyframe
	{
		struct Node
		{
			// 'globalTransform' is used to convert from local space of node to global space of scene.
			DirectX::XMFLOAT4X4 globalTransform{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

			// The transformation data of a node includes its translation, rotation and scaling vectors
						 // with respect to its parent. 
			DirectX::XMFLOAT3 scaling{ 1, 1, 1 };
			DirectX::XMFLOAT4 rotation{ 0, 0, 0, 1 }; // Rotation quaternion
			DirectX::XMFLOAT3 translation{ 0, 0, 0 };
			std::string name{};

			//シリアライズ
			template<class T>
			void serialize(T& archive)
			{
				archive(globalTransform, scaling, rotation, translation,name);
			}
		};
		std::vector<Node> nodes;

		//シリアライズ
		template<class T>
		void serialize(T& archive)
		{
			archive(nodes);
		}
	};
	std::vector<Keyframe> sequence;

	template < class T>
	void serialize(T& archive)
	{
		archive(name, samplingRate, sequence);
	}
};

class ModelData final
{
public:
	static constexpr int MAX_BONE_INFLUENCES{ 4 };
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 tangent;
		DirectX::XMFLOAT2 texCoord;
		float boneWeights[MAX_BONE_INFLUENCES]{ 1, 0, 0, 0 };
		uint32_t boneIndices[MAX_BONE_INFLUENCES]{};

		//シリアライズ
		template<class T>
		void serialize(T& archive)
		{
			archive(position, normal, tangent, texCoord, boneWeights, boneIndices);
		}

	};
	static constexpr int MAX_BONES{ 256 };

	struct ConstantsDefault
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4 materialColor;
		DirectX::XMFLOAT4X4 boneTransforms[MAX_BONES]{ { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } };
	}constantDefault{};

	struct InstancingData
	{
		DirectX::XMFLOAT4X4 world;
	};
	static constexpr int MaxInstanceCount = 512;
	struct ConstantsInstancing
	{
		InstancingData instancingData[MaxInstanceCount];
		DirectX::XMFLOAT4 materialColor;
		DirectX::XMFLOAT4X4 boneTransforms[MAX_BONES];
	}constantInstancing;


private:

	struct Skeleton
	{
		struct Bone
		{
			uint64_t uniqueId{ 0 };
			std::string name;
			// 'parent_index' is index that refers to the parent bone's position in the array that contains itself.
			int64_t parentIndex{ -1 }; // -1 : the bone is orphan
				// 'node_index' is an index that refers to the node array of the scene.
			int64_t nodeIndex{ 0 };

			// 'offset_transform' is used to convert from modelData(mesh) space to bone(node) scene.
			DirectX::XMFLOAT4X4 offsetTransform{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

			[[nodiscard]] bool IsOrphan() const { return parentIndex < 0; };

			//シリアライズ
			template < class T>
			void serialize(T& archive)
			{
				archive(uniqueId, name, parentIndex, nodeIndex, offsetTransform);
			}

		};
		std::vector<Bone> bones;

		[[nodiscard]] int64_t IndexOf(uint64_t uniqueId) const
		{
			int64_t index{ 0 };
			for (const Bone& bone : bones)
			{
				if (bone.uniqueId == uniqueId)
				{
					return index;
				}
				++index;
			}
			return -1;
		}

		template<class T>
		void serialize(T& archive)
		{
			archive(bones);
		}
	};


	struct Mesh
	{
		uint64_t uniqueId{ 0 };
		std::string name;
		// 'node_index' is an index that refers to the node array of the scene.
		int64_t nodeIndex{ 0 };

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
		//friend class SkinnedMesh;

		DirectX::XMFLOAT4X4 defaultGlobalTransform{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

		struct Subset
		{
			uint64_t materialUniqueId{ 0 };
			std::string materialName;

			uint32_t startIndexLocation{ 0 };
			uint32_t indexCount{ 0 };

			//シリアライズ
			template<class T>
			void serialize(T& archive)
			{
				archive(materialUniqueId, materialName, startIndexLocation, indexCount);
			}

		};
		std::vector<Subset> subsets;

		Skeleton bindPose;

		template < class T>
		void serialize(T& archive)
		{
			archive(uniqueId, name, nodeIndex, vertices, indices, defaultGlobalTransform, subsets,
				bindPose);
		}


		//バウンディングボックス
		//std::unique_ptr<BoundingBox> boundingBox = nullptr;
		//DirectX::XMFLOAT3 minPos{ +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX };
		//DirectX::XMFLOAT3 maxPos{ -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX };

	};
	std::vector<Mesh> meshes;

	struct Material
	{
		uint64_t uniqueId{ 0 };
		std::string name;

		DirectX::XMFLOAT4 Ka{ 0.2f, 0.2f, 0.2f, 1.0f };
		DirectX::XMFLOAT4 Kd{ 0.8f, 0.8f, 0.8f, 1.0f };
		DirectX::XMFLOAT4 Ks{ 1.0f, 1.0f, 1.0f, 1.0f };

		std::string textureFilenames[PBRMaxTexture];
		Texture texture[PBRMaxTexture];

		template<class T>
		void serialize(T& archive)
		{
			archive(uniqueId, name, Ka, Kd, Ks, textureFilenames);
		}
	};
	std::unordered_map<uint64_t, Material> materials;

private:
	//通常描画のシェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> defaultVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> defaultPixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> defaultInputLayout;
	//置き換え用シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> replacedVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> replacedPixelShader;
	ConstantBuffer defaultConstantBuffer;
	ConstantBuffer instancingConstantBuffer;
public:
	ModelData(const char* fbxFileParh, float samplingRate = 0.0f, bool trianglate = true);
	virtual ~ModelData() = default;

	void Draw(const DirectX::XMFLOAT4X4& world, Animation::Keyframe* keyframe, /*Frustum* frustum, */DirectX::XMFLOAT4 materialColor = { 1.0f,1.0f,1.0f,1.0f });
	//void DrawInstance(ID3D11DeviceContext* dc, DirectX::XMFLOAT4X4 worlds[], int instanceCount, Animation::Keyframe* keyframe, Frustum* frustum, DirectX::XMFLOAT4 materialColor = { 1.0f,1.0f,1.0f,1.0f });

	void UpdateGlobalTransform(Animation::Keyframe& keyframe) const;

	static void BlendAnimations(const Animation::Keyframe* keyframes[2], float factor, Animation::Keyframe& keyframe);
	bool AppendAnimation(const char* animationFilename, float samplingRate = 0.0f);


	//Getter
	[[nodiscard]] int GetVerticesSize(int meshIndex) const { return static_cast<int>(meshes.at(meshIndex).vertices.capacity()); }
	[[nodiscard]] int GetMeshesSize() const { return static_cast<int>(meshes.size()); }
	Mesh& GetMesh(int meshIndex) { return meshes.at(meshIndex); }
	Vertex& GetVertex(int meshIndex, int vertexIndex) { return meshes.at(meshIndex).vertices.at(vertexIndex); }
	DirectX::XMFLOAT4X4 GetMeshGlobalTransform(int meshIndex) const { return meshes.at(meshIndex).defaultGlobalTransform; }
	//BoundingBox* GetMeshBoundingBox(int meshIndex) { return meshes.at(meshIndex).boundingBox.get(); }

	ModelData::Skeleton::Bone& GetBone(int meshIndex, int boneIndex) { return GetMesh(meshIndex).bindPose.bones.at(boneIndex); }


	//シェーダーの差し替え
	void SetPixelShader(ID3D11PixelShader* newPixelShader) { replacedPixelShader = newPixelShader; }
	void SetVertexShader(ID3D11VertexShader* newVertexShader) { replacedVertexShader = newVertexShader; }

	//テクスチャの張替え
	void SetTexture(const Texture newTexture)
	{
		for (std::unordered_map<uint64_t, Material>::iterator iterator = materials.begin(); iterator != materials.end(); ++iterator)
		{
			iterator->second.texture[static_cast<int>(newTexture.textureType)] = newTexture;
			iterator->second.textureFilenames[static_cast<int>(newTexture.textureType)] = newTexture.GetTexturePath();
		}
	}

	//ボーンの名前検索
	DirectX::XMFLOAT4X4 SearchBoneMatrix(std::string name);

private:
	void FetchMeshes(FbxScene* fbxScene, std::vector<Mesh>& meshes) const;
	void FetchMaterials(FbxScene* fbxScene, std::unordered_map<uint64_t, Material>& materials) const;
	void FetchPBRMaterials(FbxScene* fbxScene, const char* fbxFilename, std::unordered_map<uint64_t, Material>& materials) const;
	void CreateComObjects(const char* fbxFilename);

	void FetchSkeleton(FbxMesh* fbxMesh, Skeleton& bindPose) const;
	void FetchAnimations(FbxScene* fbxScene, std::vector<Animation>& animationClips, float samplingRate) const;


protected:
	ModelScene sceneView;

public:
	std::vector<Animation> animationClips;

private:
	Animation::Keyframe* currentKeyframe;

};