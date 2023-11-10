#pragma once

#include <DirectXMath.h>

#include "PrimitiveMeshRenderer.h"

class CapsuleCollider;

class CapsuleMeshRenderer : public PrimitiveMeshRenderer
{
public:
	CapsuleMeshRenderer();
	~CapsuleMeshRenderer() override = default;

	//-----< �֐� >-----//

	//������
	void Initialize()override;
	//�`�揀��
	void DrawPrepare();
	//�`��
	void Draw(CapsuleCollider* capsuleCollider);

private:
	//-----< �ϐ� >-----//
	Microsoft::WRL::ComPtr <ID3D11Buffer> cylinderVertexBuffer;//�~���̒��_�o�b�t�@
	Microsoft::WRL::ComPtr <ID3D11Buffer> halfSphereUpVertexBuffer;//���i��j�̒��_�o�b�t�@
	Microsoft::WRL::ComPtr <ID3D11Buffer> halfSphereDownVertexBuffer;//���i���j�̒��_�o�b�t�@

	UINT cylinderVertexCount = 0;//�~���̒��_��
	UINT halfSphereVertexCount = 0;//���̒��_���i�����j

};