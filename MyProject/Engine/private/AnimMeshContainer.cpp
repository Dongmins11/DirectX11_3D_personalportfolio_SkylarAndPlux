#include "..\public\AnimMeshContainer.h"
#include "Hierachy_Node.h"


CAnimMeshContainer::CAnimMeshContainer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CVIBuffer(pDevice,pDeviceContext)
{
}

CAnimMeshContainer::CAnimMeshContainer(const CAnimMeshContainer & rhs)
	:CVIBuffer(rhs)
	,m_iMaterialIndex(rhs.m_iMaterialIndex)
	,m_iNumBone(rhs.m_iNumBone)
	,m_BoneNode(rhs.m_BoneNode)
{
	strcpy_s(m_szName, rhs.m_szName);
	
	for (auto iter : m_BoneNode)
		Safe_AddRef(iter);
}

HRESULT CAnimMeshContainer::NativeConstruct_Prototype(aiMesh * pAIMesh)
{
	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	strcpy_s(m_szName, pAIMesh->mName.data);

	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iStride = sizeof(VTXANIMMODEL);
	m_iNumVertices = pAIMesh->mNumVertices;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	m_pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	m_iNumBone = pAIMesh->mNumBones;

	VTXANIMMODEL*		pVertices = (VTXANIMMODEL*)m_pVertices;

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexUV, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}
	//weight���氨���� ������ �������� ���̴����� ��������� ������������������ؼ����������ؼ� �����ִ°Ͱ���

	for (_uint i = 0; i < m_iNumBone; ++i)
	{
		aiBone*		pAIBone = pAIMesh->mBones[i];

		//NumWeight�� ID���� weight�ǰ��� ����־ ������ �ε��������� ����ü�� �з����س����Ͱ���
		//�׷��� �׸�ŭ ��ȸ�����鼭 id����ã�� id�������� weight���� ã�Ƽ� ������ �־��ٷ����س����Ͱ���??

		/* pAIBone->mNumWeights : ���� �� ���� �� �޽������̳ʸ� �����ϴ� ������ ��, ��� ������ ������ �ִ���?! */
		//�����ǻ��� ��� ������ �������ְ��ִ���
		/*�����ȿ������� pAibone���ִ� ����Ʈ���߿� j��° ����Ʈ���ٷ� ���°������ �������ִ°��̴� vetexid��ǥ���ǰ��ִ�
		  ��ƽ��� �겨�̴�
		  ���ؽ����̵� unsingint���̴� ��ƽ���������Ǹ��°��ȣ���������ϴ���*/
		
		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
		{
			_uint	iVertexIndex = pAIBone->mWeights[j].mVertexId;

			if (0.f == pVertices[iVertexIndex].vBlendIndex.x)
			{
				pVertices[iVertexIndex].vBlendIndex.x = i;
				pVertices[iVertexIndex].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
			}
			else if (0.f == pVertices[iVertexIndex].vBlendIndex.y)
			{
				pVertices[iVertexIndex].vBlendIndex.y = i;
				pVertices[iVertexIndex].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
			}
			else if (0.f == pVertices[iVertexIndex].vBlendIndex.z)
			{
				pVertices[iVertexIndex].vBlendIndex.z = i;
				pVertices[iVertexIndex].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
			}
			else if (0.f == pVertices[iVertexIndex].vBlendIndex.w)
			{
				pVertices[iVertexIndex].vBlendIndex.w = i;
				pVertices[iVertexIndex].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
			}
		}
	}

	m_VBSubResourceData.pSysMem = m_pVertices;

	if (__super::Create_VetexBuffer())
		return E_FAIL;

	Safe_Delete_Array(m_pVertices);

	m_iNumPrimitive = pAIMesh->mNumFaces;
	m_iNumIndicesPerPrimitive = 3;
	m_iPrimitiveIndicesSize = sizeof(FACEINDICES32);
	m_eFormat = DXGI_FORMAT_R32_UINT;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = m_iPrimitiveIndicesSize * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		((FACEINDICES32*)m_pIndices)[i]._1 = pAIMesh->mFaces[i].mIndices[0];
		((FACEINDICES32*)m_pIndices)[i]._2 = pAIMesh->mFaces[i].mIndices[1];
		((FACEINDICES32*)m_pIndices)[i]._3 = pAIMesh->mFaces[i].mIndices[2];
	}

	m_IBSubResourceData.pSysMem = m_pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAnimMeshContainer::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CAnimMeshContainer::Render(_uint iPassIndex)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	ID3D11Buffer*		pBuffers[] = { m_pVB };
	_uint				iStrides[] = { m_iStride };
	_uint				iOffsets[] = { 0 };

	m_pDeviceContext->IASetVertexBuffers(0, 1, pBuffers, iStrides, iOffsets);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, m_eFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_eTopology);

	m_pDeviceContext->DrawIndexed(m_iNumIndicesPerPrimitive * m_iNumPrimitive, 0, 0);

	return S_OK;
}

void CAnimMeshContainer::Get_BoneMatrices(_float4x4 * pBoneMatrices, _fmatrix fPivotMatrix)
{
	_uint		iIndex = 0;

	for (auto& iterNode : m_BoneNode)
	{
		_matrix		OffsetMatrix = iterNode->Get_OffsetMatrix();
		_matrix		CombinedTransformationMatrix = iterNode->Get_CombinedTransformationMatrix();

		XMStoreFloat4x4(&pBoneMatrices[iIndex++],XMMatrixTranspose(OffsetMatrix * CombinedTransformationMatrix * fPivotMatrix));
	}
}

HRESULT CAnimMeshContainer::SetUp_MeshBones(CModel * pModel, aiMesh * pAiMesh)
{
	if (0 == m_iNumBone)
	{
		CHierachy_Node*	pNode = pModel->Finder_HierachyNode(m_szName);
		if (nullptr == pNode)
			return E_FAIL;

		m_BoneNode.push_back(pNode);

		m_iNumBone = 1;

		return S_OK;
	}

	for (_uint i = 0; i < m_iNumBone; ++i)
	{
		CHierachy_Node* pNode = pModel->Finder_HierachyNode(pAiMesh->mBones[i]->mName.data);
		if (nullptr == pNode)
			return E_FAIL;

		_matrix	OffestMatrix;
		memcpy(&OffestMatrix, &pAiMesh->mBones[i]->mOffsetMatrix, sizeof(_matrix));

		pNode->SetUp_OffsetMatrix(XMMatrixTranspose(OffestMatrix));

		m_BoneNode.push_back(pNode);
		Safe_AddRef(pNode);
	}


	return S_OK;
}

CAnimMeshContainer * CAnimMeshContainer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, aiMesh * pAIMesh)
{
	CAnimMeshContainer* pInstance = new CAnimMeshContainer(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pAIMesh)))
	{
		MSGBOX("Failed to Create CAnimMeshContainer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CAnimMeshContainer::Clone(void * pArg)
{
	CAnimMeshContainer* pInstance = new CAnimMeshContainer(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CAnimMeshContainer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimMeshContainer::Free()
{
	__super::Free();

	for (auto& iter : m_BoneNode)
	{
		Safe_Release(iter);
	}
	m_BoneNode.clear();
	vector<CHierachy_Node*>().swap(m_BoneNode);

}
