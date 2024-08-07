#include "..\public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CVIBuffer(pDevice,pDeviceContext)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ)
{
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iStride = sizeof(VTXNORTEX);
	m_iNumVertices = iNumVerticesX * iNumVerticesZ;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	m_pVertices = new VTXNORTEX[m_iNumVertices];
	m_pVerticesPos = new _float3[m_iNumVertices];

	for (_uint i = 0; i < iNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < iNumVerticesX; ++j)
		{
			_uint iIndex = i * iNumVerticesX + j;
			_float3 Nomalfloat3;

			m_pVerticesPos[iIndex] = ((VTXNORTEX*)m_pVertices)[iIndex].vPosition = _float3(j, 0.f,i);
			((VTXNORTEX*)m_pVertices)[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			((VTXNORTEX*)m_pVertices)[iIndex].vTexUV = _float2(j / _float(iNumVerticesX - 1), i / _float(iNumVerticesZ - 1));
			_int iA = 0;
		}
	}



	m_iNumPrimitive = (iNumVerticesX -1) * (iNumVerticesZ -1) * 2; // 도형갯수
	m_iNumIndicesPerPrimitive = 3; //도형1개당 쓸 정점의갯수
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
	VTXNORTEX*	pVertices = (VTXNORTEX*)m_pVertices;
	_uint		iNumFace = 0;

	for (_uint i = 0; i < iNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < iNumVerticesX - 1; ++j)
		{
			_uint iIndex = i * iNumVerticesX + j;

			_uint iIndices[4] = {
				iIndex + iNumVerticesX,
				iIndex + iNumVerticesX + 1,
				iIndex + 1,
				iIndex
			};
			_vector		vNormal, vSour, vDest;

			((FACEINDICES32*)m_pIndices)[iNumFace]._1 = iIndices[0];
			((FACEINDICES32*)m_pIndices)[iNumFace]._2 = iIndices[1];
			((FACEINDICES32*)m_pIndices)[iNumFace]._3 = iIndices[2];

			vSour = XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._2].vPosition) - XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._3].vPosition) - XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._2].vPosition);
			vNormal = XMVector3Cross(vSour, vDest);

			XMStoreFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._1].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._1].vNormal) + vNormal));

			XMStoreFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._2].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._2].vNormal) + vNormal));

			XMStoreFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._3].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._3].vNormal) + vNormal));

			++iNumFace;

			((FACEINDICES32*)m_pIndices)[iNumFace]._1 = iIndices[0];
			((FACEINDICES32*)m_pIndices)[iNumFace]._2 = iIndices[2];
			((FACEINDICES32*)m_pIndices)[iNumFace]._3 = iIndices[3];

			vSour = XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._2].vPosition) - XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._3].vPosition) - XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._2].vPosition);
			vNormal = XMVector3Cross(vSour, vDest);

			XMStoreFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._1].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._1].vNormal) + vNormal));

			XMStoreFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._2].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._2].vNormal) + vNormal));

			XMStoreFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._3].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._3].vNormal) + vNormal));
			++iNumFace;
		}
	}

	m_VBSubResourceData.pSysMem = m_pVertices;


	if (FAILED(__super::Create_VetexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(m_pVertices);

	m_IBSubResourceData.pSysMem = m_pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	D3D11_INPUT_ELEMENT_DESC		Elements[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(Complie_Shader(TEXT("../Bin/ShaderFiles/Shader_Terrain.hlsl"), Elements, 3)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(const _tchar * pTerrainPathName)
{

	HANDLE hFile = CreateFile(pTerrainPathName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
		return E_FAIL;

	_ulong dwByte = 0;

	BITMAPFILEHEADER	fh;
	BITMAPINFOHEADER	ih;

	ReadFile(hFile, &fh, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(hFile, &ih, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	_ulong*			pPixel = new _ulong[ih.biWidth * ih.biHeight];
	ZeroMemory(pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight);

	ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr);

	m_iVeticesX = ih.biWidth;
	m_iVeticesZ = ih.biHeight;

	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iStride = sizeof(VTXNORTEX);
	m_iNumVertices = m_iVeticesX * m_iVeticesZ;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	m_pVertices = new VTXNORTEX[m_iNumVertices];
	m_pVerticesPos = new _float3[m_iNumVertices];

	for (_uint i = 0; i < m_iVeticesZ; ++i)
	{
		for (_uint j = 0; j < m_iVeticesX; ++j)
		{
			_uint iIndex = i * m_iVeticesX + j;
			_float3 Nomalfloat3;

			m_pVerticesPos[iIndex] = ((VTXNORTEX*)m_pVertices)[iIndex].vPosition = _float3((_float)j,(_float)(pPixel[iIndex] & 0x000000ff) / 8.f, (_float)i);
			((VTXNORTEX*)m_pVertices)[iIndex].vNormal =_float3(0.f, 0.f, 0.f);
			((VTXNORTEX*)m_pVertices)[iIndex].vTexUV = _float2(j / _float(m_iVeticesX - 1), i / _float(m_iVeticesZ - 1));
		}
	}



	m_iNumPrimitive = (m_iVeticesX - 1) * (m_iVeticesZ - 1) * 2; // 도형갯수
	m_iNumIndicesPerPrimitive = 3; //도형1개당 쓸 정점의갯수
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
	VTXNORTEX*	pVertices = (VTXNORTEX*)m_pVertices;

	_uint		iNumFace = 0;

	for (_uint i = 0; i < m_iVeticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iVeticesX - 1; ++j)
		{
			_uint iIndex = i * m_iVeticesX + j;

			_uint iIndices[4] = {
				iIndex + m_iVeticesX,
				iIndex + m_iVeticesX + 1,
				iIndex + 1,
				iIndex
			};

			_vector		vNormal, vSour, vDest;

			((FACEINDICES32*)m_pIndices)[iNumFace]._1 = iIndices[0];
			((FACEINDICES32*)m_pIndices)[iNumFace]._2 = iIndices[1];
			((FACEINDICES32*)m_pIndices)[iNumFace]._3 = iIndices[2];

			vSour = XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._2].vPosition) - XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._3].vPosition) - XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._2].vPosition);
			vNormal = XMVector3Cross(vSour, vDest);

			XMStoreFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._1].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._1].vNormal) + vNormal));

			XMStoreFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._2].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._2].vNormal) + vNormal));

			XMStoreFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._3].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._3].vNormal) + vNormal));

			++iNumFace;

			((FACEINDICES32*)m_pIndices)[iNumFace]._1 = iIndices[0];
			((FACEINDICES32*)m_pIndices)[iNumFace]._2 = iIndices[2];
			((FACEINDICES32*)m_pIndices)[iNumFace]._3 = iIndices[3];

			vSour = XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._2].vPosition) - XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._1].vPosition);
			vDest = XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._3].vPosition) - XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._2].vPosition);
			vNormal = XMVector3Cross(vSour, vDest);
			 
			XMStoreFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._1].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._1].vNormal) + vNormal));

			XMStoreFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._2].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._2].vNormal) + vNormal));

			XMStoreFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._3].vNormal,
				XMVector3Normalize(XMLoadFloat3(&pVertices[((FACEINDICES32*)m_pIndices)[iNumFace]._3].vNormal) + vNormal));
			 
			++iNumFace;
		}
	}


	m_VBSubResourceData.pSysMem = m_pVertices;

	if (FAILED(__super::Create_VetexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(m_pVertices);

	m_IBSubResourceData.pSysMem = m_pIndices;

	if (FAILED(Create_IndexBuffer()))
		return E_FAIL;

	D3D11_INPUT_ELEMENT_DESC		Elements[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(Complie_Shader(TEXT("../Bin/ShaderFiles/Shader_Terrain.hlsl"), Elements, 3)))
		return E_FAIL;

	Safe_Delete_Array(pPixel);
	CloseHandle(hFile);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct(void * pArg)
{
	return S_OK;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pTerrainPathName)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pTerrainPathName)))
	{
		MSGBOX("Failed to Create TerrainBuffer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iNumVerticesX, _uint iNumVerticesZ)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(iNumVerticesX, iNumVerticesZ)))
	{
		MSGBOX("Failed to Create TerrainBuffer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Create TerrainBuffer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();
}
