#include "..\public\VIBuffer_Navigation.h"


CVIBuffer_Navigation::CVIBuffer_Navigation(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CVIBuffer(pDevice,pDeviceContext)
{
}

CVIBuffer_Navigation::CVIBuffer_Navigation(const CVIBuffer_Navigation & rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Navigation::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Navigation::NativeConstruct(void * pArg)
{
	memcpy(m_vPostion, pArg, sizeof(_float3) * 3);

	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iStride = sizeof(VTXTEXNAVI);
	m_iNumVertices = 3;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	m_pVertices = new VTXTEXNAVI[m_iNumVertices];
	m_pVerticesPos = new _float3[m_iNumVertices];

	m_pVerticesPos[0] = ((VTXTEXNAVI*)m_pVertices)[0].vPosition = m_vPostion[0];

	m_pVerticesPos[1] = ((VTXTEXNAVI*)m_pVertices)[1].vPosition = m_vPostion[1];

	m_pVerticesPos[2] = ((VTXTEXNAVI*)m_pVertices)[2].vPosition = m_vPostion[2];


	m_VBSubResourceData.pSysMem = m_pVertices;


	if (FAILED(__super::Create_VetexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(m_pVertices);

	m_iNumPrimitive = 1; // 도형갯수
	m_iNumIndicesPerPrimitive = 3; //도형1개당 쓸 정점의갯수
	m_iPrimitiveIndicesSize = sizeof(FACEINDICES32);
	m_eFormat = DXGI_FORMAT_R32_UINT;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = m_iPrimitiveIndicesSize * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];

	((FACEINDICES32*)m_pIndices)[0]._1 = 0;
	((FACEINDICES32*)m_pIndices)[0]._2 = 1;
	((FACEINDICES32*)m_pIndices)[0]._3 = 2;

	m_IBSubResourceData.pSysMem = m_pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	D3D11_INPUT_ELEMENT_DESC		Elements[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	if (FAILED(Complie_Shader(TEXT("../Bin/ShaderFiles/Shader_Navigation.hlsl"), Elements, 1)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Navigation::Set_PointPosition(_float3 Position, _uint iIdex)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	((VTXTEXNAVI*)m_pVertices)[0].vPosition = Position;
	((VTXTEXNAVI*)m_pVertices)[1].vPosition = ((VTXTEXNAVI*)m_pVertices)[1].vPosition;
	((VTXTEXNAVI*)m_pVertices)[2].vPosition = ((VTXTEXNAVI*)m_pVertices)[2].vPosition;

	if (FAILED(m_pDeviceContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return E_FAIL;

	memcpy(mappedResource.pData,(VTXTEXNAVI*)m_pVertices, sizeof(VTXTEXNAVI));

	m_pDeviceContext->Unmap(m_pVB, 0);

	//m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pVB);

	return S_OK;
}

CVIBuffer_Navigation * CVIBuffer_Navigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	CVIBuffer_Navigation* pInstance = new CVIBuffer_Navigation(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSGBOX("Failed to Create CVIBuffer_Navigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Navigation::Clone(void * pArg)
{
	CVIBuffer_Navigation* pInstance = new CVIBuffer_Navigation(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CVIBuffer_Navigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Navigation::Free()
{
	__super::Free();

	if (true == m_bIsClone)
		Safe_Delete_Array(m_pVertices);


}
