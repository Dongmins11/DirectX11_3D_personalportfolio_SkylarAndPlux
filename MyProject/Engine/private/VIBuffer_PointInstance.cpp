#include "..\public\VIBuffer_PointInstance.h"
#include "GameInstance.h"
#include <random>
#include <functional>
#include "Transform.h"

struct stlListSort
{
	bool operator()(const CVIBuffer_PointInstance::VIEWDESC vOne, const CVIBuffer_PointInstance::VIEWDESC vTwo) const
	{
		return vOne.fViewZ > vTwo.fViewZ;
	}
};

CVIBuffer_PointInstance::CVIBuffer_PointInstance(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CVIBuffer_Instance(pDevice, pDeviceContext)
{
}

CVIBuffer_PointInstance::CVIBuffer_PointInstance(const CVIBuffer_PointInstance & rhs)
	: CVIBuffer_Instance(rhs)
	, m_pSpeed(rhs.m_pSpeed)
	, m_pPointInst(rhs.m_pPointInst)
	, m_iOriginNumInstance(rhs.m_iOriginNumInstance)
{
}

HRESULT CVIBuffer_PointInstance::NativeConstruct_Prototype(_uint iNumInstance, _float fPSize)
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

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
	((VTXPOINT*)m_pVertices)[0].fPSize = fPSize;

	m_VBSubResourceData.pSysMem = m_pVertices;


	if (FAILED(CVIBuffer::Create_VetexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(m_pVertices);


	m_iNumInstance = iNumInstance;
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

	m_pPointInst = new POINTINST;
	ZeroMemory(m_pPointInst, sizeof(POINTINST));

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		m_pVertexMatrices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		m_pVertexMatrices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		m_pVertexMatrices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);

		mt19937 engine((unsigned int)time(NULL));
		uniform_real_distribution<> distribution(-0.01f, 0.01f);
		auto generator = bind(distribution, engine);

		m_pVertexMatrices[i].vPosition = _float4(generator(), generator(), generator(), 1.f);

		m_pSpeed[i] = (rand() % 10) + 5.f;
	}

	ZeroMemory(&m_VBInstSubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBInstSubResourceData.pSysMem = m_pVertexMatrices;

	if (FAILED(CVIBuffer_Instance::Create_InstanceBuffer()))
		return E_FAIL;

	Safe_Delete_Array(m_pVertexMatrices);

	m_iOriginNumInstance = m_iNumInstance;
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

	if (FAILED(Complie_Shader(TEXT("../Bin/ShaderFiles/Shader_PointInstance.hlsl"), Elements, 6)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_PointInstance::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
	{
		memcpy(m_pPointInst, pArg, sizeof(POINTINST));

		if (m_pPointInst->m_iType == 0)
		{
			D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
			ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

			m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);

			mt19937 engine((unsigned int)time(NULL));
			uniform_real_distribution<> distribution(-0.01f, 0.01f);
			auto generator = bind(distribution, engine);

			for (_uint i = 0; i < m_iNumInstance; ++i)
			{
			
				((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.x = (m_pPointInst->m_vPosition.x + generator());
				((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y = (m_pPointInst->m_vPosition.y + generator());
				((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.z = (m_pPointInst->m_vPosition.z + generator());
			}

			m_pDeviceContext->Unmap(m_pVBInstance, 0);
		}
		else if (m_pPointInst->m_iType == 1)
		{
			D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
			ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

			m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);

			mt19937 engine((unsigned int)time(NULL));
			uniform_real_distribution<> distribution(-0.01f, 0.01f);
			auto generator = bind(distribution, engine);

			for (_uint i = 0; i < m_iNumInstance; ++i)
			{

				((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.x = (m_pPointInst->m_vPosition.x + generator());
				((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y = (m_pPointInst->m_vPosition.y + generator());
				((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.z = (m_pPointInst->m_vPosition.z + generator());
			}

			m_pDeviceContext->Unmap(m_pVBInstance, 0);
		}
		else if (m_pPointInst->m_iType == 2)
		{
			D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
			ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

			m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);

			mt19937 engine((unsigned int)time(NULL));
			uniform_real_distribution<> distribution(-0.01f, 0.01f);
			auto generator = bind(distribution, engine);

			for (_uint i = 0; i < m_iNumInstance; ++i)
			{

				((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.x = (m_pPointInst->m_vPosition.x + generator());
				((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y = (m_pPointInst->m_vPosition.y + generator());
				((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.z = (m_pPointInst->m_vPosition.z + generator());
			}

			m_pDeviceContext->Unmap(m_pVBInstance, 0);
		}
		else if (m_pPointInst->m_iType == 3 || m_pPointInst->m_iType == 4)
		{
			//if (m_iNumInstance < 360)
			//PerDegree = 360.f / m_iNumInstance;
			//else
			//PerDegree = 1.f;

			D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
			ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

			m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);

			//for (_uint i = 0; i < m_iNumInstance; ++i)
			//{
			//m_vDir[i] = _float4(0.f, 0.f, 0.f, 0.f);
			//((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition = m_pPointInst->m_vPosition;
			//}


			for (_uint i = 0; i < m_iNumInstance; ++i)
			{
			//_float4 Dir;
			//Dir = _float4(cosf(XMConvertToDegrees(PerDegree*i) * 2.f), sinf(XMConvertToDegrees(PerDegree*i)), 0.f, 1.f);
			//XMStoreFloat4(&m_vDir[i], XMVector4Normalize(XMLoadFloat4(&Dir) - XMLoadFloat4(&((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition)));
				((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition = m_pPointInst->m_vPosition;
			}


			m_pDeviceContext->Unmap(m_pVBInstance, 0);
		}
		else if (m_pPointInst->m_iType == 5)
		{
			D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
			ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

			m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);

			for (_uint i = 0; i < m_iNumInstance; ++i)
			{
				((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition = m_pPointInst->m_vPosition;
			}
			m_pDeviceContext->Unmap(m_pVBInstance, 0);

		}
		else if (m_pPointInst->m_iType == 6)
		{
			D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
			ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

			m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);

			for (_uint i = 0; i < m_iNumInstance; ++i)
			{
				((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition = m_pPointInst->m_vPosition;
			}
			m_pDeviceContext->Unmap(m_pVBInstance, 0);

		}
		else if (m_pPointInst->m_iType == 7)
		{
			D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
			ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

			m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);

			for (_uint i = 0; i < m_iNumInstance; ++i)
			{
				((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition = m_pPointInst->m_vPosition;
			}
			m_pDeviceContext->Unmap(m_pVBInstance, 0);

		}
			
	}

	return S_OK;
}

void CVIBuffer_PointInstance::Update(_double TimeDelta,CTransform* pTransform, CTransform* pTargetTransform)
{
	if (nullptr == m_pDeviceContext)
		return;

	if (m_pPointInst->m_iType == 0)
	{
		if (FAILED(Type_One(TimeDelta, pTransform)))
			return;
	}
	else if (m_pPointInst->m_iType == 1)
	{
		if (FAILED(Type_Two(TimeDelta, pTransform)))
			return;
	}
	else if (m_pPointInst->m_iType == 2)
	{
		if (FAILED(Type_Two(TimeDelta, pTransform)))
			return;
	}
	else if (m_pPointInst->m_iType == 3 || m_pPointInst->m_iType == 4)
	{
		if (FAILED(Type_Three(TimeDelta, pTransform)))
			return;
	}
	else if (m_pPointInst->m_iType == 5)
	{
		if (FAILED(Type_For(TimeDelta, pTransform,pTargetTransform)))
			return;
	}
	else if (m_pPointInst->m_iType == 6)
	{
		if (FAILED(Type_Five(TimeDelta, pTransform)))
			return;
	}
	else if (m_pPointInst->m_iType == 7)
	{
		if (FAILED(Type_Six(TimeDelta, pTransform)))
			return;
	}
}



HRESULT CVIBuffer_PointInstance::Type_One(_double TimeDelta, CTransform* pTransform)
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
		{
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.x += (TimeDelta *2)* generator();
		}
		else
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.x += (TimeDelta * 2) * generator2();

		if (0.f <= ((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y)
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y += (TimeDelta * 2) * generator();
		else
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y += (TimeDelta * 2) * generator2();

		if (0.f <= ((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.z)
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.z += (TimeDelta * 2) * generator();
		else
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.z += (TimeDelta * 2) * generator2();

	}

	m_pDeviceContext->Unmap(m_pVBInstance, 0);

	return S_OK;
}

HRESULT CVIBuffer_PointInstance::Type_Two(_double TimeDelta,CTransform* pTransform)
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

HRESULT CVIBuffer_PointInstance::Type_Three(_double TimeDelta, CTransform* pTransform)
{
	D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
	ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);
	_float PerDegree;
	_bool bCheck = m_bChangeCheck;
	//list<VIEWDESC> ListViewZ;
	if (false == bCheck)
	{
		for (_uint i = 0; i < m_iNumInstance; ++i)
		{

			PerDegree = i * (360 / m_iNumInstance);

			if (i > 15)
				PerDegree -= 5.f;

			_matrix Mat= XMMatrixRotationAxis(pTransform->Get_State(CTransform::STATE_LOOK),PerDegree);

			XMStoreFloat4(&m_vDir[i],XMVector3TransformNormal(pTransform->Get_State(CTransform::STATE_RIGHT), Mat));

			m_bChangeCheck = false;
		}
	}
	


	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.x += m_vDir[i].x * (TimeDelta * 100);
		((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y += m_vDir[i].y * (TimeDelta * 100);
		((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.z += m_vDir[i].z * (TimeDelta * 100);
	}
	//for (_uint i = 0; i < m_iNumInstance; ++i)
	//{
	//	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	//	Safe_AddRef(pGameInstance);
	//	VIEWDESC ViewZ;
	//	ViewZ.fViewZ = XMVectorGetX(XMVector4Length(XMLoadFloat4(&((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition) - pGameInstance->Get_CamPosition()));
	//	ViewZ.iIndex = i;

	//	ListViewZ.emplace_back(ViewZ);
	//	Safe_Release(pGameInstance);
	//}

	//ListViewZ.sort(stlListSort());
	//auto iter = ListViewZ.begin();
	//for (_uint i = 0; i < m_iNumInstance; ++i)
	//{
	//	((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition = ((VTXMATRIX*)Mapped_SubResource.pData)[(*iter).iIndex].vPosition;
	//	++iter;
	//}

	m_pDeviceContext->Unmap(m_pVBInstance, 0);

	return S_OK;
}

HRESULT CVIBuffer_PointInstance::Type_Five(_double TimeDelta, CTransform* pTransform)
{

	D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
	ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);
	_float PerDegree;
	_bool bCheck = m_bChangeCheck;
	if (false == bCheck)
	{
		for (_uint i = 0; i < m_iNumInstance; ++i)
		{

			PerDegree = i * (360 / m_iNumInstance);

			if (i > 15)
				PerDegree -= 5.f;

			_matrix Mat = XMMatrixRotationAxis(XMVectorSet(0.f,1.f,0.f,0.f), PerDegree);
			XMStoreFloat4(&m_vDir[i], XMVector3TransformNormal(pTransform->Get_State(CTransform::STATE_RIGHT), Mat));
			m_bChangeCheck = false;
		}
	}



	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.x += m_vDir[i].x * (TimeDelta);
		((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y += m_vDir[i].y * (TimeDelta);
		((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.z += m_vDir[i].z * (TimeDelta);
	}

	m_pDeviceContext->Unmap(m_pVBInstance, 0);

	return S_OK;
}

HRESULT CVIBuffer_PointInstance::Type_For(_double TimeDelta, CTransform* pTransform, CTransform* pTargetTransform)
{

	D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
	ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);

	m_BackTimeAcc += TimeDelta;

	m_iNumInstance = (_uint)m_BackTimeAcc;
	if (m_BackTimeAcc > m_iOriginNumInstance)
	{
		m_BackTimeAcc = 0.0;
		m_iNumInstance = 0;
	}
	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.x -= /*m_pSpeed[i] * */0.0001f * (rand() % 10 - 3);
		((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y -=/* m_pSpeed[i] * */0.0001f * (rand() % 10);
		((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.z -= /*m_pSpeed[i] * */0.0001f * (rand() % 10 - 8);

		if (m_iNumInstance - i > 15)
			continue;

		if ((i == m_iNumInstance - 1) && m_iNumInstance != m_iNumInstanceBackUp)
		{
			_float3 vLocalMyPos;
			XMStoreFloat3(&vLocalMyPos, XMVector3TransformCoord(pTargetTransform->Get_State(CTransform::STATE_POSITION),
				pTransform->Get_WorldMatrixInverse()));

			vLocalMyPos.x += (rand() % 10 - 5) * 0.03;
			vLocalMyPos.y += (rand() % 10 - 5) * 0.03;
			vLocalMyPos.z += (rand() % 10 - 5) * 0.03;
			XMStoreFloat4(&((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition, XMLoadFloat3(&vLocalMyPos));
		}
	}

	m_pDeviceContext->Unmap(m_pVBInstance, 0);


	m_iNumInstanceBackUp = m_iNumInstance;

	return S_OK;
}

HRESULT CVIBuffer_PointInstance::Type_Six(_double TimeDelta, CTransform* pTransform)
{
	D3D11_MAPPED_SUBRESOURCE		Mapped_SubResource;
	ZeroMemory(&Mapped_SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &Mapped_SubResource);
	_bool bCheck = m_bChangeCheck;
	if (false == bCheck)
	{
		mt19937 engine((unsigned int)time(NULL));
		uniform_real_distribution<> distribution(-1.f, 1.f);
		auto generator = bind(distribution, engine);

		mt19937 engineTwo((unsigned int)time(NULL));
		uniform_real_distribution<> distributionTwo(0.f, 0.5f);
		auto generatorTwo = bind(distributionTwo, engineTwo);

		for (_uint i = 0; i < m_iNumInstance; ++i)
		{

			((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.x = generator();
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y = generatorTwo();
			((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.z = generator();

			m_bChangeCheck = true;
		}
	}



	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		((VTXMATRIX*)Mapped_SubResource.pData)[i].vPosition.y += TimeDelta;
	}

	m_pDeviceContext->Unmap(m_pVBInstance, 0);

	return S_OK;
}

HRESULT CVIBuffer_PointInstance::Type_Seven(_double TimeDelta, CTransform* pTransform)
{
	return S_OK;
}

HRESULT CVIBuffer_PointInstance::Type_Eight(_double TimeDelta, CTransform* pTransform)
{
	return S_OK;
}

HRESULT CVIBuffer_PointInstance::Type_Nine(_double TimeDelta, CTransform* pTransform)
{
	return S_OK;
}


CVIBuffer_PointInstance * CVIBuffer_PointInstance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iNumInstance, _float fPSize)
{
	CVIBuffer_PointInstance* pInstance = new CVIBuffer_PointInstance(pDevice, pDeviceContext);
	if (FAILED(pInstance->NativeConstruct_Prototype(iNumInstance, fPSize)))
	{
		MSGBOX("Failed to Create CVIBuffer_PointInstance");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_PointInstance::Clone(void * pArg)
{
	CVIBuffer_PointInstance* pInstance = new CVIBuffer_PointInstance(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CVIBuffer_PointInstance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_PointInstance::Free()
{
	__super::Free();

	if (false == m_bIsClone)
	{
		Safe_Delete_Array(m_pSpeed);
		Safe_Delete_Array(m_pPointInst);
	}
}
