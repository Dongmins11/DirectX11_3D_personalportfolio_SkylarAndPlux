#include "..\public\VIBuffer_Sphere.h"



CVIBuffer_Sphere::CVIBuffer_Sphere(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CVIBuffer(pDevice,pDeviceContext)
{
}

CVIBuffer_Sphere::CVIBuffer_Sphere(const CVIBuffer_Sphere & rhs)
	:CVIBuffer(rhs)
	,m_fRadius(rhs.m_fRadius)
	,m_iSliceCount(rhs.m_iSliceCount)
	,m_iStackCount(rhs.m_iStackCount)
{
}

HRESULT CVIBuffer_Sphere::NativeConstruct_Prototype(_float radius, _uint StackCount, _uint SliceCount)
{
	m_fRadius = radius; // 원점으로부터 p까지의 거리 즉 반지름
	m_iStackCount = StackCount; //가로
	m_iSliceCount = SliceCount; // 세로

	return S_OK;
}

HRESULT CVIBuffer_Sphere::NativeConstruct(void * pArg)
{
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//MatrixBufferType* dataPtr;
	//m_pDeviceContext->Map(m_pVB,0,D3D11_MAP_WRITE_DISCARD,0, &mappedResource)

	vector<_int> v;
	m_pVertices = new SPHEREVTXTEXNOMALUV[421];

	_uint	iIndex = 0;

	_float PhiStep = XM_PI / m_iStackCount;

	_int	NumRigs = m_iStackCount;

	for (_int i = 0; i < NumRigs; ++i)
	{
		_float fPhi = i*PhiStep;

		_float fThetaStep = 2.0f / XM_PI / m_iSliceCount;


		for (_int j = 0; j <= m_iSliceCount; ++j)
		{
			_float fTheta = j*fThetaStep;

			((SPHEREVTXTEXNOMALUV*)m_pVertices)[j].vPosition.x = m_fRadius*sinf(fPhi)*cosf(fTheta);
			((SPHEREVTXTEXNOMALUV*)m_pVertices)[j].vPosition.y = m_fRadius*cosf(fPhi);
			((SPHEREVTXTEXNOMALUV*)m_pVertices)[j].vPosition.z = m_fRadius*sinf(fPhi)*sinf(fTheta);
			++iIndex;
			v.push_back(iIndex);
		}

	}
	++iIndex;
	v.push_back(iIndex);
	((SPHEREVTXTEXNOMALUV*)m_pVertices)[iIndex].vPosition = _float3(0.f, -m_fRadius, 0.f);

	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iStride = sizeof(SPHEREVTXTEXNOMALUV);
	m_iNumVertices = v.size();

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;


	m_VBSubResourceData.pSysMem = m_pVertices;
	
	if (FAILED(__super::Create_VetexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(m_pVertices);


	m_iPrimitiveIndicesSize = sizeof(FACEINDICES32);
	m_eFormat = DXGI_FORMAT_R16_UINT;

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));

	m_IBDesc.ByteWidth = m_iPrimitiveIndicesSize * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];
	
	_int	NorthPoleIndex = (_int)iIndex - 1;
	_int	SouthPoleIndex = (_int)iIndex - 2;

	_int	NumRingVertices = m_iSliceCount + 1;

	_uint		iNumFace = 0;

	for (_int i = 0; i < m_iStackCount - 2; ++i)
	{
		for (_int j = 0; j < m_iSliceCount; ++j)
		{
			((FACEINDICES32*)m_pIndices)[iNumFace]._1 = (i*NumRingVertices + j);
			((FACEINDICES32*)m_pIndices)[iNumFace]._2 = (i*NumRingVertices + j +1);
			((FACEINDICES32*)m_pIndices)[iNumFace]._3 = ((i+1)*NumRingVertices + j);
			++iNumFace;

			((FACEINDICES32*)m_pIndices)[iNumFace]._1 = ((i + 1)*NumRingVertices + j);
			((FACEINDICES32*)m_pIndices)[iNumFace]._2 = (i*NumRingVertices + j + 1);
			((FACEINDICES32*)m_pIndices)[iNumFace]._3 = ((j + 1)*NumRingVertices + j + 1);
			++iNumFace;
		}
	}

	for (_int i = 0; i < m_iSliceCount; ++i)
	{
		((FACEINDICES32*)m_pIndices)[iNumFace]._1 = NorthPoleIndex;
		((FACEINDICES32*)m_pIndices)[iNumFace]._2 = (i + 1);
		((FACEINDICES32*)m_pIndices)[iNumFace]._3 = i;
		++iNumFace;
	}


	_int BaseIndex = (NumRigs - 1) * NumRingVertices;

	for(_int i =0; i <m_iSliceCount; ++i)
	{
		((FACEINDICES32*)m_pIndices)[iNumFace]._1 = SouthPoleIndex;
		((FACEINDICES32*)m_pIndices)[iNumFace]._2 = BaseIndex + i;
		((FACEINDICES32*)m_pIndices)[iNumFace]._3 = BaseIndex + i + 1;
		++iNumFace;
	}

	m_iNumPrimitive = iNumFace;
	m_iNumIndicesPerPrimitive = iNumFace *3;

	m_IBSubResourceData.pSysMem = m_pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	D3D11_INPUT_ELEMENT_DESC		Elements[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	if (FAILED(Complie_Shader(TEXT("../Bin/ShaderFiles/Shader_Sphere.hlsl"), Elements, 1)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Sphere * CVIBuffer_Sphere::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _float radius, _uint StackCount, _uint SliceCount)
{
	CVIBuffer_Sphere* pInstance = new CVIBuffer_Sphere(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(radius, StackCount, SliceCount)))
	{
		MSGBOX("Failed to Create CVIBuffer_Sphere");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Sphere::Clone(void * pArg)
{
	CVIBuffer_Sphere* pInstance = new CVIBuffer_Sphere(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create CVIBuffer_Sphere");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Sphere::Free()
{
	__super::Free();
}
