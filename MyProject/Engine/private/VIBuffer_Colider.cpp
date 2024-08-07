#include "..\public\VIBuffer_Colider.h"
#include "Transform.h"

CVIBuffer_Colider::CVIBuffer_Colider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CVIBuffer(pDevice,pDeviceContext)
{
}

CVIBuffer_Colider::CVIBuffer_Colider(const CVIBuffer_Colider & rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Colider::NativeConstruct_Prototype()
{

	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iStride = sizeof(VTXCOLIDERCUBETEX);
	m_iNumVertices = 8;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	m_pVertices = new VTXCOLIDERCUBETEX[m_iNumVertices];
	m_pVerticesPos = new _float3[m_iNumVertices];

	m_pVerticesPos[0] = ((VTXCOLIDERCUBETEX*)m_pVertices)[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	m_pVerticesPos[1] = ((VTXCOLIDERCUBETEX*)m_pVertices)[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
	m_pVerticesPos[2] = ((VTXCOLIDERCUBETEX*)m_pVertices)[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	m_pVerticesPos[3] = ((VTXCOLIDERCUBETEX*)m_pVertices)[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	m_pVerticesPos[4] = ((VTXCOLIDERCUBETEX*)m_pVertices)[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	m_pVerticesPos[5] = ((VTXCOLIDERCUBETEX*)m_pVertices)[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
	m_pVerticesPos[6] = ((VTXCOLIDERCUBETEX*)m_pVertices)[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
	m_pVerticesPos[7] = ((VTXCOLIDERCUBETEX*)m_pVertices)[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);

	m_VBSubResourceData.pSysMem = m_pVertices;

	if (FAILED(__super::Create_VetexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(m_pVertices);


	m_iNumPrimitive = 12; // µµÇü°¹¼ö
	m_iNumIndicesPerPrimitive = 3; //µµÇü1°³´ç ¾µ Á¤Á¡ÀÇ°¹¼ö
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


	((FACEINDICES32*)m_pIndices)[0]._1 = 1;
	((FACEINDICES32*)m_pIndices)[0]._2 = 5;
	((FACEINDICES32*)m_pIndices)[0]._3 = 6;


	((FACEINDICES32*)m_pIndices)[1]._1 = 1;
	((FACEINDICES32*)m_pIndices)[1]._2 = 6;
	((FACEINDICES32*)m_pIndices)[1]._3 = 2;


	/*¿ÞÂÊ ¿·¸é*/

	((FACEINDICES32*)m_pIndices)[2]._1 = 4;
	((FACEINDICES32*)m_pIndices)[2]._2 = 0;
	((FACEINDICES32*)m_pIndices)[2]._3 = 3;


	((FACEINDICES32*)m_pIndices)[3]._1 = 4;
	((FACEINDICES32*)m_pIndices)[3]._2 = 3;
	((FACEINDICES32*)m_pIndices)[3]._3 = 7;


	/*À­¸é*/

	((FACEINDICES32*)m_pIndices)[4]._1 = 4;
	((FACEINDICES32*)m_pIndices)[4]._2 = 5;
	((FACEINDICES32*)m_pIndices)[4]._3 = 1;


	((FACEINDICES32*)m_pIndices)[5]._1 = 4;
	((FACEINDICES32*)m_pIndices)[5]._2 = 1;
	((FACEINDICES32*)m_pIndices)[5]._3 = 0;


	/*¾Æ·§¸é*/

	((FACEINDICES32*)m_pIndices)[6]._1 = 3;
	((FACEINDICES32*)m_pIndices)[6]._2 = 2;
	((FACEINDICES32*)m_pIndices)[6]._3 = 6;


	((FACEINDICES32*)m_pIndices)[7]._1 = 3;
	((FACEINDICES32*)m_pIndices)[7]._2 = 6;
	((FACEINDICES32*)m_pIndices)[7]._3 = 7;


	((FACEINDICES32*)m_pIndices)[8]._1 = 5;
	((FACEINDICES32*)m_pIndices)[8]._2 = 4;
	((FACEINDICES32*)m_pIndices)[8]._3 = 7;


	((FACEINDICES32*)m_pIndices)[9]._1 = 5;
	((FACEINDICES32*)m_pIndices)[9]._2 = 7;
	((FACEINDICES32*)m_pIndices)[9]._3 = 6;


	/*µÞ¸é*/

	((FACEINDICES32*)m_pIndices)[10]._1 = 0;
	((FACEINDICES32*)m_pIndices)[10]._2 = 1;
	((FACEINDICES32*)m_pIndices)[10]._3 = 2;


	((FACEINDICES32*)m_pIndices)[11]._1 = 0;
	((FACEINDICES32*)m_pIndices)[11]._2 = 2;
	((FACEINDICES32*)m_pIndices)[11]._3 = 3;


	m_IBSubResourceData.pSysMem = m_pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	D3D11_INPUT_ELEMENT_DESC		Elements[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	if (FAILED(Complie_Shader(TEXT("../Bin/ShaderFiles/Shader_Colider.hlsl"), Elements, 1)))
		return E_FAIL;

	return S_OK;

}

HRESULT CVIBuffer_Colider::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Colider::BigAndSmallVertex(CTransform * pTransform, _float3 * MinPointOut, _float3 * MaxPointOut)
{
	_float3 vMinVTX = m_pVerticesPos[0];
	_float3 vMaxVTX = m_pVerticesPos[7];

	_matrix WorldMat = pTransform->Get_WorldMatrix();
	_vector vSacle;

	vSacle = pTransform->Get_Scale();

	_vector vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVectorGetX(vSacle);
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVectorGetY(vSacle);
	_vector vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVectorGetZ(vSacle);

	memcpy(&WorldMat.r[0], &vRight, sizeof(_vector));
	memcpy(&WorldMat.r[1], &vUp, sizeof(_vector));
	memcpy(&WorldMat.r[2], &vLook, sizeof(_vector));

	XMVector3TransformCoord(XMLoadFloat3(&vMinVTX), WorldMat);
	XMVector3TransformCoord(XMLoadFloat3(&vMaxVTX), WorldMat);

	*MinPointOut = vMinVTX;
	*MaxPointOut = vMaxVTX;

	return S_OK;
}

CVIBuffer_Colider * CVIBuffer_Colider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CVIBuffer_Colider* pInstance = new CVIBuffer_Colider(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CVIBuffer_Colider");
		Safe_Release(pInstance);
	}
	

	return pInstance;
}

CComponent * CVIBuffer_Colider::Clone(void * pArg)
{
	CVIBuffer_Colider* pInstance = new CVIBuffer_Colider(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CVIBuffer_Colider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Colider::Free()
{
	__super::Free();


}
