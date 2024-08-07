#include "..\public\VIBuffer_RectInstance.h"

CVIBuffer_RectInstance::CVIBuffer_RectInstance(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CVIBuffer_Instance(pDevice,pDeviceContext)
{
}

CVIBuffer_RectInstance::CVIBuffer_RectInstance(const CVIBuffer_RectInstance & rhs)
	:CVIBuffer_Instance(rhs)
	, m_pSpeed(rhs.m_pSpeed)
	, m_pRectInst(rhs.m_pRectInst)
{
}

HRESULT CVIBuffer_RectInstance::NativeConstruct_Prototype(_uint iNumInstance)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	m_pVertices = new VTXTEX[m_iNumVertices];
	m_pVerticesPos = new _float3[m_iNumVertices];

	m_pVerticesPos[0] = ((VTXTEX*)m_pVertices)[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	((VTXTEX*)m_pVertices)[0].vTexUV = _float2(0.f, 0.f);

	m_pVerticesPos[1] = ((VTXTEX*)m_pVertices)[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	((VTXTEX*)m_pVertices)[1].vTexUV = _float2(1.f, 0.f);

	m_pVerticesPos[2] = ((VTXTEX*)m_pVertices)[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	((VTXTEX*)m_pVertices)[2].vTexUV = _float2(1.f, 1.f);

	m_pVerticesPos[3] = ((VTXTEX*)m_pVertices)[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	((VTXTEX*)m_pVertices)[3].vTexUV = _float2(0.f, 1.f);

	m_VBSubResourceData.pSysMem = m_pVertices;


	if (FAILED(CVIBuffer::Create_VetexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(m_pVertices);


	m_iNumInstance = iNumInstance;
	m_iInstStride = sizeof(VTXMATRIX);
	m_iIndexCountPerInstance = 6; //정점 갯수 삼각형2개 -> 정점6개

	ZeroMemory(&m_VBInstDesc, sizeof(D3D11_BUFFER_DESC));

	m_VBInstDesc.ByteWidth = m_iInstStride * m_iNumInstance;
	m_VBInstDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBInstDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBInstDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBInstDesc.MiscFlags = 0;
	m_VBInstDesc.StructureByteStride = m_iInstStride;

	m_pVertexMatrices = new VTXMATRIX[m_iNumInstance];
	ZeroMemory(m_pVertexMatrices, sizeof(VTXMATRIX) * m_iNumInstance);

	m_pSpeed = new _float[m_iNumInstance];
	ZeroMemory(m_pSpeed, sizeof(_float) * m_iNumInstance);

	m_pRectInst = new RECTINST;
	ZeroMemory(m_pRectInst, sizeof(RECTINST));

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		m_pVertexMatrices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		m_pVertexMatrices[i].vUp	= _float4(0.f, 1.f, 0.f, 0.f);
		m_pVertexMatrices[i].vLook	= _float4(0.f, 0.f, 1.f, 0.f);
		m_pVertexMatrices[i].vPosition = _float4(0.f, 0.f, 0.f, 1.f);

		m_pSpeed[i] = (rand() % 10 )+ 5.f;
	}

	ZeroMemory(&m_VBInstSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBInstSubResourceData.pSysMem = m_pVertexMatrices;

	if (FAILED(CVIBuffer_Instance::Create_InstanceBuffer()))
		return E_FAIL;

	Safe_Delete_Array(m_pVertexMatrices);


	m_iNumPrimitive = 2; // 도형갯수
	m_iNumIndicesPerPrimitive = 3; //도형1개당 쓸 정점의갯수
	m_iPrimitiveIndicesSize = sizeof(FACEINDICES16);
	m_eFormat = DXGI_FORMAT_R16_UINT;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	m_IBDesc.ByteWidth = m_iPrimitiveIndicesSize * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;

	m_pIndices = new FACEINDICES16[m_iNumPrimitive];

	((FACEINDICES16*)m_pIndices)[0]._1 = 0;
	((FACEINDICES16*)m_pIndices)[0]._2 = 1;
	((FACEINDICES16*)m_pIndices)[0]._3 = 2;

	((FACEINDICES16*)m_pIndices)[1]._1 = 0;
	((FACEINDICES16*)m_pIndices)[1]._2 = 2;
	((FACEINDICES16*)m_pIndices)[1]._3 = 3;

	m_IBSubResourceData.pSysMem = m_pIndices;

	if (FAILED(CVIBuffer::Create_IndexBuffer()))
		return E_FAIL;

	D3D11_INPUT_ELEMENT_DESC		Elements[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};

	if (FAILED(Complie_Shader(TEXT("../Bin/ShaderFiles/Shader_RectInstance.hlsl"), Elements, 6)))
		return E_FAIL;

	return S_OK;


	return S_OK;
}

HRESULT CVIBuffer_RectInstance::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
	{
		memcpy(m_pRectInst, pArg, sizeof(RECTINST));


		if (0 == m_pRectInst->m_iType)
		{
			D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
			ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

			m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);

			for (_uint i = 0; i < m_iNumInstance; ++i)
			{
				if (0 == i)
				{
					((VTXMATRIX*)Mapped_SubResource.pData)[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
					((VTXMATRIX*)Mapped_SubResource.pData)[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
					((VTXMATRIX*)Mapped_SubResource.pData)[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
				}
				else if (1 == i)
				{
					((VTXMATRIX*)Mapped_SubResource.pData)[i].vRight = _float4(2.0f, 0.f, 0.f, 0.f);
					((VTXMATRIX*)Mapped_SubResource.pData)[i].vUp = _float4(0.f, 2.0f, 0.f, 0.f);
					((VTXMATRIX*)Mapped_SubResource.pData)[i].vLook = _float4(0.f, 0.f, 2.0f, 0.f);
				}
				else if (2 == i)
				{
					((VTXMATRIX*)Mapped_SubResource.pData)[i].vRight = _float4(3.f, 0.f, 0.f, 0.f);
					((VTXMATRIX*)Mapped_SubResource.pData)[i].vUp = _float4(0.f, 3.f, 0.f, 0.f);
					((VTXMATRIX*)Mapped_SubResource.pData)[i].vLook = _float4(0.f, 0.f, 3.f, 0.f);
				}

				((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition = m_pRectInst->m_vPosition;
			}

			m_pDeviceContext->Unmap(m_pVBInstance, 0);
		}
	}

	return S_OK;
}

void CVIBuffer_RectInstance::Update(_double TimeDelta)
{
	if (nullptr == m_pDeviceContext)
		return;

	D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
	ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);
	if (0 == m_pRectInst->m_iType)
	{
		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vRight.x += 0.05;
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vUp.y += 0.05;
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vLook.z += 0.05;

			if (3.0f <= ((VTXMATRIX*)Mapped_SubResource.pData)[i].vRight.x)
			{
				((VTXMATRIX*)Mapped_SubResource.pData)[i].vRight = _float4(0.1f, 0.f, 0.f, 0.f);
				((VTXMATRIX*)Mapped_SubResource.pData)[i].vUp = _float4(0.f, 0.1f, 0.f, 0.f);
				((VTXMATRIX*)Mapped_SubResource.pData)[i].vLook = _float4(0.f, 0.f, 0.1f, 0.f);
			}
		}
	}
	else if (1 == m_pRectInst->m_iType)
	{
		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y -= m_pSpeed[i] * TimeDelta;

			if (((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y <= 0.0f)
				((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y = 15.0f;
		}
	}
	m_pDeviceContext->Unmap(m_pVBInstance, 0);
}

CVIBuffer_RectInstance * CVIBuffer_RectInstance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iNumInstance)
{
	CVIBuffer_RectInstance* pInstance = new CVIBuffer_RectInstance(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype(iNumInstance)))
	{
		MSGBOX("Failed to Create CVIBuffer_RectInstance");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_RectInstance::Clone(void * pArg)
{
	CVIBuffer_RectInstance* pInstance = new CVIBuffer_RectInstance(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CVIBuffer_RectInstance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_RectInstance::Free()
{
	__super::Free();

	if (false == m_bIsClone)
	{
		Safe_Delete_Array(m_pSpeed);
		Safe_Delete_Array(m_pRectInst);
	}
}
