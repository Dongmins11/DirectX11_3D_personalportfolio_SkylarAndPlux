// NaviTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "NaviTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MyForm2.h"
#include "GameInstance.h"
#include "NavigationTool.h"
#include "NavigationCube.h"
#include "Floor_Tool.h"
#include "Tool_Defines.h"
#include "MapTool2.h"
#include "Floor_Tool.h"


// CNaviTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNaviTool, CDialogEx)

CNaviTool::CNaviTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NAVITOOL, pParent)
{

}

CNaviTool::~CNaviTool()
{
	Safe_Release(m_pNaviMeshTool);
	Safe_Release(m_pTerrainBuffer);
	Safe_Release(m_pTransform);
}

void CNaviTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_CheckPicking);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBoxIndex);
	DDX_Control(pDX, IDC_LIST1, m_ListBoxPoint);
	DDX_Control(pDX, IDC_CHECK7, m_ButtonDelete);
}


BEGIN_MESSAGE_MAP(CNaviTool, CDialogEx)

	ON_CBN_SELCHANGE(IDC_COMBO1, &CNaviTool::OnCbnSelchangeIndexPoint)
	ON_LBN_SELCHANGE(IDC_LIST1, &CNaviTool::OnLbnSelchangePointListBox)
	ON_BN_CLICKED(IDC_BUTTON1, &CNaviTool::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_BUTTON2, &CNaviTool::OnBnClickedSaveButton)
	ON_BN_CLICKED(IDC_BUTTON6, &CNaviTool::OnBnClickedLoadButton)
END_MESSAGE_MAP()


// CNaviTool 메시지 처리기입니다.



BOOL CNaviTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Navigation"), LEVEL_STATIC, L"Clone_Navi", nullptr, (CGameObject**)&m_pNaviMeshTool)))
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	m_pTransform = (CTransform*)pGameInstance->Get_Components(LEVEL_STATIC, TEXT("Clone_Terrain"), TEXT("Com_Transform"), 0);
	if (nullptr == m_pTransform)
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	m_pTerrainBuffer = (CVIBuffer_Terrain*)pGameInstance->Get_Components(LEVEL_STATIC, TEXT("Clone_Terrain"), TEXT("Com_VIBuffer"), 0);
	if (nullptr == m_pTerrainBuffer)
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	Safe_Release(pGameInstance);

	m_bIsNaviCheck[0] = false;
	m_bIsNaviCheck[1] = true;
	m_bIsNaviCheck[2] = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

HRESULT CNaviTool::Navi_Picking()
{
	if (m_CheckPicking.GetCheck() == BST_CHECKED)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (pGameInstance->Key_Down(VK_LBUTTON))
		{
			map<wstring, vector<CNavigationCube*>>	m_mapCube = dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->Get_MapCube();

				_float	vDist;
				_float	fDistSave = 0.f;
				_float3 fDist;
				_float3 vPickPos;
				_bool	bIsCheck = false;
				_bool	bCheck = false;
				TCHAR	tLayerTag2[MAX_PATH] = L"";
				wstring	tLayerTag = L"Clone_Navi%d";

				TCHAR	szMapTag[MAX_PATH] = L"";
				wstring strMapTag = L"%d";

				TCHAR	szObejctTag2[MAX_PATH] = L"Clone_Cube";


				if (false == m_bIsNaviCheck[0])
				{
					if (true == pGameInstance->Picking_To_Terrain(m_pTransform, m_pTerrainBuffer, &fDist))
					{
						for (auto& Pair : m_mapCube)
						{
							for (auto& iter : Pair.second)
							{
								if (true == pGameInstance->Picking_To_Colider(iter->m_pTransformCom, iter->m_pVIBufferCom, &vPickPos, &vDist))
								{
									if ((fDistSave >= vDist) || fDistSave == 0.f)
									{
										fDistSave = vDist;
										XMStoreFloat3(&vPickPos, iter->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
										bCheck = true;
									}
								}
							}
						}
						if (false == bCheck)
							m_vPos[0] = fDist;
						else
							m_vPos[0] = vPickPos;

						m_bIsNaviCheck[0] = true;
						m_bIsNaviCheck[1] = false;
						return S_OK;
					}
				}
				if (false == m_bIsNaviCheck[1])
				{
					if (true == pGameInstance->Picking_To_Terrain(m_pTransform, m_pTerrainBuffer, &fDist))
					{
						for (auto& Pair : m_mapCube)
						{
							for (auto& iter : Pair.second)
							{
								if (true == pGameInstance->Picking_To_Colider(iter->m_pTransformCom, iter->m_pVIBufferCom, &vPickPos, &vDist))
								{
									if ((fDistSave >= vDist) || fDistSave == 0.f)
									{
										fDistSave = vDist;
										XMStoreFloat3(&vPickPos, iter->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
										bCheck = true;
									}
								}
							}
						}
						if (false == bCheck)
							m_vPos[1] = fDist;
						else
							m_vPos[1] = vPickPos;
						m_bIsNaviCheck[1] = true;
						m_bIsNaviCheck[2] = false;
						return S_OK;
					}
				}
				if (false == m_bIsNaviCheck[2])
				{
					if (true == pGameInstance->Picking_To_Terrain(m_pTransform, m_pTerrainBuffer, &fDist))
					{
						for (auto& Pair : m_mapCube)
						{
							for (auto& iter : Pair.second)
							{
								if (true == pGameInstance->Picking_To_Colider(iter->m_pTransformCom, iter->m_pVIBufferCom, &vPickPos, &vDist))
								{
									if ((fDistSave >= vDist) || fDistSave == 0.f)
									{
										fDistSave = vDist;
										XMStoreFloat3(&vPickPos,iter->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
										bCheck = true;
									}
								}
							}
						}
						if (false == bCheck)
							m_vPos[2] = fDist;
						else
							m_vPos[2] = vPickPos;

						m_bIsNaviCheck[2] = true;
						m_bIsNaviCheck[0] = false;
						bIsCheck = true;
					}
				}
				if (true == bIsCheck)
				{
					_bool	TrueCheck = false;
					wsprintf(tLayerTag2, tLayerTag.c_str(), m_iIndex);
					wsprintf(szMapTag, strMapTag.c_str(), m_iIndex);
					strMapTag = szMapTag;

					_float fCheck = (m_vPos[0].x*m_vPos[1].z) + (m_vPos[1].x*m_vPos[2].z) + (m_vPos[2].x*m_vPos[0].z)- ((m_vPos[0].z*m_vPos[1].x) + (m_vPos[1].z*m_vPos[2].x) + (m_vPos[2].z*m_vPos[0].x));
					if (0 > fCheck)
					{
				/*		_float3 vTemp = m_vPos[1];
						m_vPos[1] = m_vPos[2];
						m_vPos[2] = vTemp;*/
						TrueCheck = true;
					}

					if (true == TrueCheck)
					{
						++m_iIndex;
						dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->SetUp_Component_Tool(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Navigation", tLayerTag2, m_vPos, szObejctTag2, strMapTag,CNavigationTool::PROPERTY_FALL);
						CString		StrIndex;
						StrIndex = strMapTag.c_str();
						m_ComboBoxIndex.AddString(StrIndex);
						m_ListBoxPoint.ResetContent();
						for (_int i = 0; i < 3; ++i)
						{
							CString PointAdd = L"";
							TCHAR	Point[MAX_PATH] = L"%d";
							wsprintf(Point, Point, i);
							PointAdd = Point;
							m_ListBoxPoint.AddString(PointAdd);
						}
					}

			}
		}
				Safe_Release(pGameInstance);
	}
	return S_OK;
}

HRESULT CNaviTool::Mesh_Picking()
{
	if (m_CheckPicking.GetCheck() == BST_CHECKED)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (pGameInstance->Key_Down(VK_LBUTTON))
		{
		/*	if (nullptr == m_pCurrentObject)
				return E_FAIL;*/


			CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CMyForm2*	pMyForm = dynamic_cast<CMyForm2*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
			
			//vector<CVIBuffer*>	vecMeshContatinal = *m_pCurrentObject->m_pVIBufferCom->Get_MeshContatinerVector();
			map<wstring, vector<CNavigationCube*>>	m_mapCube = dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->Get_MapCube();

			_float	vDist = 0.f;
			_float	fDistSave = 0.f;
			_float3 vPickPos;
			
			_float	fMeshDistSave = 0.f;
			_float	fMeshDist = 0.f;
			_float3	vMeshPickPos;

			_bool	bIsCheck = false;
			_bool	bCheck = false;
			TCHAR	tLayerTag2[MAX_PATH] = L"";
			wstring	tLayerTag = L"Clone_Navi%d";

			TCHAR	szMapTag[MAX_PATH] = L"";
			wstring strMapTag = L"%d";

			TCHAR	szObejctTag2[MAX_PATH] = L"Clone_Cube";

			for (auto& iter : pMyForm->m_pMapTool->m_MapObject)
			{
				for (auto& PairSecond : iter.second)
				{
					vector<CVIBuffer*>	vecMeshContatinal2 = *PairSecond->m_pVIBufferCom->Get_MeshContatinerVector();
					for (auto& iter3 : vecMeshContatinal2)
					{
						if (true == pGameInstance->Picking_To_Model(PairSecond->m_pTransformCom, iter3, &vMeshPickPos, &fMeshDist))
						{
							if ((fMeshDistSave >= fMeshDist) || fMeshDistSave == 0.f)
							{
								fMeshDistSave = fMeshDist;
								XMStoreFloat3(&m_vPos[m_iCount], XMVector3TransformCoord(XMLoadFloat3(&vMeshPickPos), PairSecond->m_pTransformCom->Get_WorldMatrix()));
								bCheck = true;
							}

							for (auto& Pair : m_mapCube)
							{
								for (auto& iter2 : Pair.second)
								{
									if (true == pGameInstance->Picking_To_Colider(iter2->m_pTransformCom, iter2->m_pVIBufferCom, &vPickPos, &vDist))
									{
										if ((fDistSave >= vDist) || fDistSave == 0.f)
										{
											fDistSave = vDist;
											XMStoreFloat3(&m_vPos[m_iCount], iter2->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
										}
									}
								}
							}
						}
					}
				}
			}

			if (true == bCheck)
			{
				bCheck = false;
				++m_iCount;
			}

			if (3 <= m_iCount)
			{
				m_iCount = 0;
				_bool	TrueCheck = false;
				wsprintf(tLayerTag2, tLayerTag.c_str(), m_iIndex);
				wsprintf(szMapTag, strMapTag.c_str(), m_iIndex);
				strMapTag = szMapTag;

				_float fCheck = (m_vPos[0].x*m_vPos[1].z) + (m_vPos[1].x*m_vPos[2].z) + (m_vPos[2].x*m_vPos[0].z) - ((m_vPos[0].z*m_vPos[1].x) + (m_vPos[1].z*m_vPos[2].x) + (m_vPos[2].z*m_vPos[0].x));
				if (0 > fCheck)
				{
			/*		_float3 vTemp = m_vPos[1];
					m_vPos[1] = m_vPos[2];
					m_vPos[2] = vTemp;*/
					TrueCheck = true;
				}

				if (true == TrueCheck)
				{
					++m_iIndex;
					dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->SetUp_Component_Tool(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Navigation", tLayerTag2, m_vPos, szObejctTag2, strMapTag, CNavigationTool::PROPERTY_FALL);
					CString		StrIndex;
					StrIndex = strMapTag.c_str();
					m_ComboBoxIndex.AddString(StrIndex);
					m_ListBoxPoint.ResetContent();
					for (_int i = 0; i < 3; ++i)
					{
						CString PointAdd = L"";
						TCHAR	Point[MAX_PATH] = L"%d";
						wsprintf(Point, Point, i);
						PointAdd = Point;
						m_ListBoxPoint.AddString(PointAdd);
					}

				}
			}


		}
		Safe_Release(pGameInstance);
	}
	return S_OK;
}

HRESULT CNaviTool::Triangle_Picking()
{
	if (m_CheckPicking.GetCheck() == BST_CHECKED)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (pGameInstance->Key_Down(VK_LBUTTON))
		{
			map<wstring, vector<CNavigationCube*>>	m_mapCube = dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->Get_MapCube();

			_float	vDist;
			_float	fDistSave = 0.f;
			_float3 fDist;
			_float3 vPickPos;
			_bool	bIsCheck = false;
			_bool	bCheck = false;
			TCHAR	tLayerTag2[MAX_PATH] = L"";
			wstring	tLayerTag = L"Clone_Navi%d";

			TCHAR	szMapTag[MAX_PATH] = L"";
			wstring strMapTag = L"%d";

			TCHAR	szObejctTag2[MAX_PATH] = L"Clone_Cube";


			if (false == m_bIsNaviCheck[0])
			{
				if (true == pGameInstance->Picking_To_Colider(m_pTransform, m_pTerrainBuffer, &vPickPos,&vDist))
				{

					for (auto& Pair : m_mapCube)
					{
						for (auto& iter : Pair.second)
						{
							if (true == pGameInstance->Picking_To_Colider(iter->m_pTransformCom, iter->m_pVIBufferCom, &vPickPos, &vDist))
							{
								if ((fDistSave >= vDist) || fDistSave == 0.f)
								{
									fDistSave = vDist;
									XMStoreFloat3(&vPickPos, iter->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
									bCheck = true;
								}
							}
						}
					}
					if (false == bCheck)
						m_vPos[0] = fDist;
					else
						m_vPos[0] = vPickPos;

					m_bIsNaviCheck[0] = true;
					m_bIsNaviCheck[1] = false;
					return S_OK;
				}
			}
			if (false == m_bIsNaviCheck[1])
			{
				if (true == pGameInstance->Picking_To_Terrain(m_pTransform, m_pTerrainBuffer, &fDist))
				{
					for (auto& Pair : m_mapCube)
					{
						for (auto& iter : Pair.second)
						{
							if (true == pGameInstance->Picking_To_Colider(iter->m_pTransformCom, iter->m_pVIBufferCom, &vPickPos, &vDist))
							{
								if ((fDistSave >= vDist) || fDistSave == 0.f)
								{
									fDistSave = vDist;
									XMStoreFloat3(&vPickPos, iter->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
									bCheck = true;
								}
							}
						}
					}
					if (false == bCheck)
						m_vPos[1] = fDist;
					else
						m_vPos[1] = vPickPos;
					m_bIsNaviCheck[1] = true;
					m_bIsNaviCheck[2] = false;
					return S_OK;
				}
			}
			if (false == m_bIsNaviCheck[2])
			{
				if (true == pGameInstance->Picking_To_Terrain(m_pTransform, m_pTerrainBuffer, &fDist))
				{
					for (auto& Pair : m_mapCube)
					{
						for (auto& iter : Pair.second)
						{
							if (true == pGameInstance->Picking_To_Colider(iter->m_pTransformCom, iter->m_pVIBufferCom, &vPickPos, &vDist))
							{
								if ((fDistSave >= vDist) || fDistSave == 0.f)
								{
									fDistSave = vDist;
									XMStoreFloat3(&vPickPos, iter->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
									bCheck = true;
								}
							}
						}
					}
					if (false == bCheck)
						m_vPos[2] = fDist;
					else
						m_vPos[2] = vPickPos;

					m_bIsNaviCheck[2] = true;
					m_bIsNaviCheck[0] = false;
					bIsCheck = true;
				}
			}
			if (true == bIsCheck)
			{
				_bool	TrueCheck = false;
				wsprintf(tLayerTag2, tLayerTag.c_str(), m_iIndex);
				wsprintf(szMapTag, strMapTag.c_str(), m_iIndex);
				strMapTag = szMapTag;

				_float fCheck = (m_vPos[0].x*m_vPos[1].z) + (m_vPos[1].x*m_vPos[2].z) + (m_vPos[2].x*m_vPos[0].z) - ((m_vPos[0].z*m_vPos[1].x) + (m_vPos[1].z*m_vPos[2].x) + (m_vPos[2].z*m_vPos[0].x));
				if (0 > fCheck)
					TrueCheck = true;

				if (true == TrueCheck)
				{
					++m_iIndex;
					dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->SetUp_Component_Tool(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Navigation", tLayerTag2, m_vPos, szObejctTag2, strMapTag,0);
					CString		StrIndex;
					StrIndex = strMapTag.c_str();
					m_ComboBoxIndex.AddString(StrIndex);
					m_ListBoxPoint.ResetContent();
					for (_int i = 0; i < 3; ++i)
					{
						CString PointAdd = L"";
						TCHAR	Point[MAX_PATH] = L"%d";
						wsprintf(Point, Point, i);
						PointAdd = Point;
						m_ListBoxPoint.AddString(PointAdd);
					}
				}

			}
		}
		Safe_Release(pGameInstance);
	}
	return S_OK;
}

HRESULT CNaviTool::Select_Picking()
{
	if (m_ButtonDelete.GetCheck() == BST_CHECKED)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (pGameInstance->Key_Down(VK_LBUTTON))
		{
			_float	vDist;
			_float3 vPickPos;
			_uint iIndex = 0;

			auto begin = dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom.begin();
			auto end = dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom.end();

			for (auto& iter2 : dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom)
				iter2->Set_ColorCheck(false);

			for (; begin != dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom.end(); ++begin)
			{
				if (pGameInstance->Picking_To_Model(dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_pTransformCom, *begin, &vPickPos, &vDist))
				{
					(*begin)->Set_ColorCheck(true);
					m_ComboBoxIndex.SetCurSel(iIndex);
				}
				++iIndex;
			}

			Safe_Release(pGameInstance);
		}
	}
	return S_OK;
}

HRESULT CNaviTool::Finder_Point()
{

	_int iComboIndex = m_ComboBoxIndex.GetCurSel();
	_int iListIndex = m_ListBoxPoint.GetCurSel();
	vector<CVIBuffer_Navigation*>	vectorNavi;
	auto iter = dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom.begin();
	iter += iComboIndex;

	const _ulong* pIndices = (_ulong*)(*iter)->Get_Indices();
	const _float3* pVerticesPos = (*iter)->Get_VerticesPos();

	XMLoadFloat3(&pVerticesPos[pIndices[0]]);
	XMLoadFloat3(&pVerticesPos[pIndices[1]]);
	XMLoadFloat3(&pVerticesPos[pIndices[2]]);
	
	for (auto& VectorIndex : dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom)
	{
		if (*iter == VectorIndex)
			continue;

		if (iter[iListIndex]->Get_Pos() == VectorIndex->Get_Pos())
			vectorNavi.push_back(VectorIndex);
	}





	return S_OK;
}

HRESULT CNaviTool::Set_CurrentObject(CFloor_Tool * FloorObject)
{
	if (nullptr == FloorObject)
		return E_FAIL;

	m_pCurrentObject = FloorObject;

	return S_OK;
}


void CNaviTool::OnCbnSelchangeIndexPoint()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_int iComboIndex = m_ComboBoxIndex.GetCurSel();
	CString		GetString = L"";
	if (!dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom.empty())
	{
		for (auto& iter2 : dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom)
			iter2->Set_ColorCheck(false);

		if ((size_t)iComboIndex <= dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom.size() - 1)
		{
			auto iter = dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom.begin();
			iter += iComboIndex;

			(*iter)->Set_ColorCheck(true);
		}

	}
}

void CNaviTool::OnLbnSelchangePointListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//Finder_Point();
	
	_int iComboIndex = m_ComboBoxIndex.GetCurSel();
	CString		GetString = L"";
	vector<CNavigationCube*>	vecPoint;
	if (!dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom.empty())
	{
		if ((size_t)iComboIndex <= dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom.size() - 1)
		{
			for (auto& Pair : dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_MapCube)
			{
				for (auto& PairSecond : Pair.second)
				{
					PairSecond->m_pTransformCom->Set_Scailing(1.5f);
					PairSecond->Set_ColorCheck(false);
				}
			}
			m_ComboBoxIndex.GetLBText(iComboIndex, GetString);
			wstring FinderTag = GetString;
			

			auto Vector = dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom.begin();
			Vector += iComboIndex;



			auto Pair = find_if(dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_MapCube.begin(),
				dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_MapCube.end(), CTagFinderWstring(FinderTag.c_str()));

			if (Pair == dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_MapCube.end())
				return;


			_int iPointIndex = m_ListBoxPoint.GetCurSel();
			auto iter = Pair->second.begin();
			iter += iPointIndex;

			for (auto& Pair : dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_MapCube)
			{
				for (auto& Second : Pair.second)
				{
					if (XMVector3Equal((*iter)->m_pTransformCom->Get_State(CTransform::STATE_POSITION), Second->m_pTransformCom->Get_State(CTransform::STATE_POSITION)))
						vecPoint.push_back(Second);
				}
			}

			for (auto& Point : vecPoint)
			{
				Point->m_pTransformCom->Set_Scailing(1.6f);
				Point->Set_ColorCheck(true);
			}

		}

	}




}


void CNaviTool::OnBnClickedDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_int iComboIndex = m_ComboBoxIndex.GetCurSel();
	CString		GetString = L"";
	if (!dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom.empty())
	{
		if ((size_t)iComboIndex <= dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom.size()-1)
		{
			auto iter = dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom.begin();
			iter += iComboIndex;
			Safe_Release(*iter);
			iter = dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom.erase(iter);


			m_ComboBoxIndex.GetLBText(iComboIndex, GetString);
			wstring FinderTag = GetString;
			dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->Finder_MapCube(FinderTag);


			m_ComboBoxIndex.DeleteString(iComboIndex);
			--m_iIndex;
			
		}

	}

}


void CNaviTool::OnBnClickedSaveButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data File(*.dat) | *.dat ||", this);

	_tchar szBuff[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szBuff);
	PathRemoveFileSpec(szBuff);
	lstrcat(szBuff, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szBuff;

	if (IDOK == Dlg.DoModal())
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		HANDLE hFile = CreateFile(Dlg.GetPathName().GetString(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte = 0;

		for (auto& iter : dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->m_vVIBufferCom)
		{
			const _ulong* pIndices = (_ulong*)iter->Get_Indices();
			const _float3* pVerticesPos = iter->Get_VerticesPos();
			_uint	iPropertyType = dynamic_cast<CVIBuffer_Navigation*>(iter)->Get_Property();

			WriteFile(hFile, pVerticesPos, sizeof(_float3) * 3, &dwByte, nullptr);
			WriteFile(hFile, &iPropertyType, sizeof(_uint), &dwByte, nullptr);
		}
		CloseHandle(hFile);
	}
}


void CNaviTool::OnBnClickedLoadButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog DIg(TRUE, L".dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"../Data Files(*.dat)|*.dat||", this);

	TCHAR szBuffer[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szBuffer);

	PathRemoveFileSpec(szBuffer);

	lstrcat(szBuffer, L"\\Data");

	DIg.m_pOFN->lpstrInitialDir = szBuffer;

	if (IDOK == DIg.DoModal())
	{
		CString				strPathName = DIg.GetPathName().GetString();
		const _tchar*		pPath2 = strPathName.GetString();


		TCHAR	szObejctTag2[MAX_PATH] = L"Clone_Cube";

		HANDLE hFile = CreateFile(pPath2, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;
		_float3 aa[3];
		_int iIndex = 0;
		_uint iProperty = 0;
		DWORD	dwByte = 0;
		while (true)
		{
			ReadFile(hFile, aa, sizeof(_float3) * 3, &dwByte, nullptr);
			ReadFile(hFile,&iProperty, sizeof(_uint), &dwByte, nullptr);

			if (0 == dwByte)
				break;

			TCHAR	tLayerTag2[MAX_PATH] = L"";
			wstring	tLayerTag = L"Clone_Navi%d";

			TCHAR	szMapTag[MAX_PATH] = L"";
			wstring strMapTag = L"%d";

			wsprintf(tLayerTag2, tLayerTag.c_str(), m_iIndex);
			wsprintf(szMapTag, strMapTag.c_str(), m_iIndex);
			strMapTag = szMapTag;
			++m_iIndex;
			dynamic_cast<CNavigationTool*>(m_pNaviMeshTool)->SetUp_Component_Tool(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Navigation", tLayerTag2, aa, szObejctTag2, strMapTag,0);
			CString		StrIndex;
			StrIndex = strMapTag.c_str();
			m_ComboBoxIndex.AddString(StrIndex);
			m_ListBoxPoint.ResetContent();
		}

		for (_int i = 0; i < 3; ++i)
		{
			CString PointAdd = L"";
			TCHAR	Point[MAX_PATH] = L"%d";
			wsprintf(Point, Point, i);
			PointAdd = Point;
			m_ListBoxPoint.AddString(PointAdd);
		}










		CloseHandle(hFile);
	}
}
