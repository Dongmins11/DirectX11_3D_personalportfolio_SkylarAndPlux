#include "..\Public\Navigation.h"
#include "Cell.h"

CNavigation::CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_Cells(rhs.m_Cells)
	, m_NaviDesc(rhs.m_NaviDesc)
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);
}

HRESULT CNavigation::NativeConstruct_Prototype(const _tchar * pNaviDataFilePath)
{
	HANDLE			hFile = 0;

	_ulong	dwByte = 0;
	_uint   iProperty = 0;
	hFile = CreateFile(pNaviDataFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3			vPoints[CCell::POINT_END];

	while (true)
	{
		ZeroMemory(vPoints, sizeof(_float3) * CCell::POINT_END);

		ReadFile(hFile, vPoints, sizeof(_float3) * CCell::POINT_END, &dwByte, nullptr);
		ReadFile(hFile, &iProperty, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CCell*			pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	CloseHandle(hFile);

	if (FAILED(SetUp_Neighbor()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigation::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_NaviDesc, pArg, sizeof(NAVIDESC));

	return S_OK;
}

void CNavigation::Update(_fmatrix TransformMatrix)
{
	for (auto& pCell : m_Cells)
		pCell->Update(TransformMatrix);
}

_bool CNavigation::Move_OnNavigation(_fvector vPosition)
{
	if (m_NaviDesc.iCurrentIndex >= m_Cells.size())
		return false;

	CCell*			pNeighbor = nullptr;

	if (true == m_Cells[m_NaviDesc.iCurrentIndex]->isIn(vPosition, &pNeighbor))
	{
		return true;
	}
	else
	{
		if (nullptr != pNeighbor)
		{
			while (true)
			{
				if (nullptr == pNeighbor)
					return false;
				if (true == pNeighbor->isIn(vPosition, &pNeighbor))
				{
					m_NaviDesc.iCurrentIndex = pNeighbor->Get_Index();
					break;
				}
			}
			return true;
		}
		else
		{
			/* 슬라이딩.*/

			return false;
		}
	}

}

#ifdef _DEBUG
HRESULT CNavigation::Render(_uint iPassIndex)
{
	for (auto& pCell : m_Cells)
		pCell->Render(iPassIndex);

	return S_OK;
}
#endif // _DEBUG

HRESULT CNavigation::SetUp_Neighbor()
{
	for (auto& pSour : m_Cells)
	{
		for (auto& pDest : m_Cells)
		{
			if (pSour == pDest)
				continue;

			if (pDest->Compare_Points(pSour->Get_Point(CCell::POINT_A), pSour->Get_Point(CCell::POINT_B)))
				pSour->Set_Neighbor(CCell::LINE_AB, pDest);
			if (pDest->Compare_Points(pSour->Get_Point(CCell::POINT_B), pSour->Get_Point(CCell::POINT_C)))
				pSour->Set_Neighbor(CCell::LINE_BC, pDest);
			if (pDest->Compare_Points(pSour->Get_Point(CCell::POINT_C), pSour->Get_Point(CCell::POINT_A)))
				pSour->Set_Neighbor(CCell::LINE_CA, pDest);
		}
	}
	return S_OK;
}


CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pNaviDataFilePath)
{
	CNavigation*	pInstance = new CNavigation(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(pNaviDataFilePath)))
	{
		MSGBOX("Failed to Created CNavigation");

		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*	pInstance = new CNavigation(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CNavigation");

		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();

}
