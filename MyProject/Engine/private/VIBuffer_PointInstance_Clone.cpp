#include "..\public\VIBuffer_PointInstance_Clone.h"
#include <random>
#include <functional>



CVIBuffer_PointInstance_Clone::CVIBuffer_PointInstance_Clone(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CVIBuffer_Instance(pDevice, pDeviceContext)
{
}

CVIBuffer_PointInstance_Clone::CVIBuffer_PointInstance_Clone(const CVIBuffer_PointInstance_Clone & rhs)
	: CVIBuffer_Instance(rhs)
	, m_fPSize(rhs.m_fPSize)
{
}

HRESULT CVIBuffer_PointInstance_Clone::NativeConstruct_Prototype(_uint iNumInstance, _float fPSize)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	m_iNumInstance = iNumInstance;
	m_fPSize = fPSize;

	return S_OK;
}

HRESULT CVIBuffer_PointInstance_Clone::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
	{
		m_pPointInst = new POINTINST;
		ZeroMemory(m_pPointInst, sizeof(POINTINST));

		memcpy(m_pPointInst, pArg, sizeof(POINTINST));


		m_eTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
		m_iStride = sizeof(VTXPOINT);
		m_iNumVertices = 1;

		ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

		m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
		m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
		m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_VBDesc.CPUAccessFlags = 0;
		m_VBDesc.MiscFlags = 0;
		m_VBDesc.StructureByteStride = m_iStride;

		m_pVertices = new VTXPOINT[m_iNumVertices];
		m_pVerticesPos = new _float3[m_iNumVertices];

		m_pVerticesPos[0] = ((VTXPOINT*)m_pVertices)[0].vPosition = _float3(0.f, 0.f, 0.f);
		((VTXPOINT*)m_pVertices)[0].fPSize = m_fPSize;

		m_VBSubResourceData.pSysMem = m_pVertices;


		if (FAILED(CVIBuffer::Create_VetexBuffer()))
			return E_FAIL;

		Safe_Delete_Array(m_pVertices);

		m_iInstStride = sizeof(VTXMATRIX);
		m_iIndexCountPerInstance = 1; //정점 갯수 삼각형2개 -> 정점6개

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

		mt19937 engine((unsigned int)time(NULL));
		uniform_real_distribution<> distribution(-0.01f, 0.01f);
		auto generator = bind(distribution, engine);

		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			m_pVertexMatrices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
			m_pVertexMatrices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
			m_pVertexMatrices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);

			m_pVertexMatrices[i].vPosition = _float4(m_pPointInst->m_vPosition.x + generator()
				, m_pPointInst->m_vPosition.y + generator()
				, m_pPointInst->m_vPosition.z + generator()
				, 1.f);

			m_pSpeed[i] = (rand() % 10) + 5.f;
		}

		ZeroMemory(&m_VBInstSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		m_VBInstSubResourceData.pSysMem = m_pVertexMatrices;

		if (FAILED(CVIBuffer_Instance::Create_InstanceBuffer()))
			return E_FAIL;

		Safe_Delete_Array(m_pVertexMatrices);


		m_iNumPrimitive = m_iNumInstance; // 도형갯수
		m_iNumIndicesPerPrimitive = 1; //도형1개당 쓸 정점의갯수
		m_iPrimitiveIndicesSize = sizeof(_ushort);
		m_eFormat = DXGI_FORMAT_R16_UINT;

		ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
		m_IBDesc.ByteWidth = m_iPrimitiveIndicesSize * m_iNumPrimitive;
		m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
		m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		m_IBDesc.CPUAccessFlags = 0;
		m_IBDesc.MiscFlags = 0;
		m_IBDesc.StructureByteStride = 0;

		m_pIndices = new _ushort[m_iNumPrimitive];

		for (_uint i = 0; i < m_iNumInstance; ++i)
		{
			((_ushort*)m_pIndices)[i] = 0;
		}


		m_IBSubResourceData.pSysMem = m_pIndices;

		if (FAILED(CVIBuffer::Create_IndexBuffer()))
			return E_FAIL;

		D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "PSIZE", 0, DXGI_FORMAT_R32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
		};

		if (FAILED(Complie_Shader(TEXT("../Bin/ShaderFiles/Shader_PointInstance_Clone.hlsl"), Elements, 6)))
			return E_FAIL;
	}
	return S_OK;
}

void CVIBuffer_PointInstance_Clone::Update(_double TimeDelta)
{
	if (nullptr == m_pDeviceContext)
		return;

	if (m_pPointInst->m_iType == 3)
	{
		if (FAILED(Type_One(TimeDelta)))
			return;
	}

}



HRESULT CVIBuffer_PointInstance_Clone::Type_One(_double TimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
	ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);

	mt19937 engine((unsigned int)time(NULL));
	uniform_real_distribution<> distribution(0.4f, 1.3f);
	auto generator = bind(distribution, engine);

	mt19937 engine2((unsigned int)time(NULL));
	uniform_real_distribution<> distribution2(-1.3f, -0.4f);
	auto generator2 = bind(distribution2, engine2);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		if (0.f <= ((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.x)
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.x += TimeDelta * generator();
		else
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.x += TimeDelta * generator2();

		if (0.f <= ((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y)
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y += TimeDelta * generator();
		else
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y += TimeDelta * generator2();

		if (0.f <= ((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.z)
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.z += TimeDelta * generator();
		else
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.z += TimeDelta * generator2();

	}

	m_pDeviceContext->Unmap(m_pVBInstance, 0);

	return S_OK;
}

HRESULT CVIBuffer_PointInstance_Clone::Type_Two(_double TimeDelta)
{

	return S_OK;
}

HRESULT CVIBuffer_PointInstance_Clone::Type_Three(_double TimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
	ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);


	m_pDeviceContext->Unmap(m_pVBInstance, 0);

	return S_OK;
}

HRESULT CVIBuffer_PointInstance_Clone::Type_For(_double TimeDelata)
{

	D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
	ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);


	m_pDeviceContext->Unmap(m_pVBInstance, 0);

	return S_OK;
}

HRESULT CVIBuffer_PointInstance_Clone::Type_Five(_double TimeDelta)
{

	D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
	ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);


	m_pDeviceContext->Unmap(m_pVBInstance, 0);

	return S_OK;
}

HRESULT CVIBuffer_PointInstance_Clone::Type_Six(_double TimeDelta)
{
	return E_NOTIMPL;
}

HRESULT CVIBuffer_PointInstance_Clone::Type_Seven(_double TimeDelta)
{
	return E_NOTIMPL;
}

HRESULT CVIBuffer_PointInstance_Clone::Type_Eight(_double TimeDelta)
{
	return E_NOTIMPL;
}

HRESULT CVIBuffer_PointInstance_Clone::Type_Nine(_double TimeDelta)
{
	return E_NOTIMPL;
}


CVIBuffer_PointInstance_Clone * CVIBuffer_PointInstance_Clone::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iNumInstance, _float fPSize)
{
	CVIBuffer_PointInstance_Clone* pInstance = new CVIBuffer_PointInstance_Clone(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype(iNumInstance, fPSize)))
	{
		MSGBOX("Failed to Create CVIBuffer_PointInstance_Clone");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_PointInstance_Clone::Clone(void * pArg)
{
	CVIBuffer_PointInstance_Clone* pInstance = new CVIBuffer_PointInstance_Clone(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CVIBuffer_PointInstance_Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_PointInstance_Clone::Free()
{
	__super::Free();
	Safe_Delete_Array(m_pSpeed);
	Safe_Delete_Array(m_pPointInst);
}
