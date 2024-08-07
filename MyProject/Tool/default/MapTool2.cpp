// MapTool2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool2.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MyForm2.h"
#include "PathFinder.h"
#include "afxdialogex.h"
#include "Floor_Tool.h"
#include "NaviTool.h"
#include "RenderObject.h"
#include "Arrow.h"
#include "GameInstance.h"

// CMapTool2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool2, CDialogEx)

CMapTool2::CMapTool2(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAPTOOL2, pParent)
{

}

CMapTool2::~CMapTool2()
{
	for (auto& Pair : m_MapObject)
	{
		for (auto& iter : Pair.second)
		{
			(*iter).Set_DieLayer(1);
		}

		Pair.second.clear();
	}
	m_MapObject.clear();

	for (auto& Pair : m_MapModelDesc)
	{
		for (auto& iter : Pair.second)
			Safe_Delete(iter);

		Pair.second.clear();
	}
	m_MapModelDesc.clear();

	Safe_Release(m_pRenderObject);
	Safe_Release(m_pArrow);
	Safe_Release(m_pTerrainBuffer);
	Safe_Release(m_pTransform);
}

void CMapTool2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComboBox);
	DDX_Control(pDX, IDC_LIST1, m_ObjectListBox);
	DDX_Control(pDX, IDC_SLIDER4, m_SliderScaile);
	DDX_Control(pDX, IDC_EDIT7, m_EditScaile);
	DDX_Control(pDX, IDC_CHECK1, m_CheckPicking);
	DDX_Control(pDX, IDC_SLIDER7, m_SliderRotationX);
	DDX_Control(pDX, IDC_SLIDER8, m_SliderRotationY);
	DDX_Control(pDX, IDC_SLIDER9, m_SliderRotationZ);
	DDX_Control(pDX, IDC_EDIT10, m_EditRotX);
	DDX_Control(pDX, IDC_EDIT11, m_EditRotY);
	DDX_Control(pDX, IDC_EDIT12, m_EditRotZ);
	DDX_Control(pDX, IDC_LIST2, m_AddListBox);
	DDX_Control(pDX, IDC_EDIT2, m_PosX);
	DDX_Control(pDX, IDC_EDIT3, m_PosY);
	DDX_Control(pDX, IDC_EDIT1, m_PosZ);
	DDX_Control(pDX, IDC_SPIN3, m_SpinCtalX);
	DDX_Control(pDX, IDC_SPIN2, m_SpinCtalY);
	DDX_Control(pDX, IDC_SPIN4, m_SpinCtalZ);
	DDX_Control(pDX, IDC_SLIDER10, m_SliderX);
	DDX_Control(pDX, IDC_SLIDER11, m_SliderY);
	DDX_Control(pDX, IDC_SLIDER12, m_SliderZ);
	DDX_Control(pDX, IDC_EDIT8, m_EditScaleX);
	DDX_Control(pDX, IDC_EDIT9, m_EditScaleY);
	DDX_Control(pDX, IDC_EDIT13, m_EditScaleZ);
	DDX_Control(pDX, IDC_RADIO1, m_CheckRadio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_CheckRadio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_CheckRadio[2]);
	DDX_Control(pDX, IDC_CHECK2, m_CheckPickPos);
	DDX_Control(pDX, IDC_CHECK4, m_CheckRender);
	DDX_Control(pDX, IDC_CHECK3, m_ButtonDelete);
	DDX_Control(pDX, IDC_CHECK5, m_CheckPickObject);
	DDX_Control(pDX, IDC_CHECK8, m_CheckMesh);
}


BEGIN_MESSAGE_MAP(CMapTool2, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMapTool2::OnCbnSelchangeComboBox)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool2::OnLbnObjectSeleteChancePick)
	ON_BN_CLICKED(IDC_CHECK1, &CMapTool2::OnBnClickedPickingOn)
	ON_WM_HSCROLL()
	ON_LBN_SELCHANGE(IDC_LIST2, &CMapTool2::OnLbnSelchangeModelDesc)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool2::OnBnClickedSetUpPosButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CMapTool2::OnDeltaposPosX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CMapTool2::OnDeltaposPosY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CMapTool2::OnDeltaposPosZ)
	ON_BN_CLICKED(IDC_BUTTON5, &CMapTool2::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapTool2::OnBnClickedSaveButton)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapTool2::OnBnClickedLoadButton)
END_MESSAGE_MAP()


// CMapTool2 메시지 처리기입니다.


BOOL CMapTool2::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	m_SliderRotationX.SetRange(0, 360);
	m_SliderRotationX.SetPos(0);
	m_SliderRotationX.SetRangeMin(0);
	m_SliderRotationX.SetRangeMax(360);
	m_SliderRotationX.SetTicFreq(1);
	m_SliderRotationX.SetPageSize(10);

	m_SliderRotationY.SetRange(0, 360);
	m_SliderRotationY.SetPos(0);
	m_SliderRotationY.SetRangeMin(0);
	m_SliderRotationY.SetRangeMax(360);
	m_SliderRotationY.SetTicFreq(1);
	m_SliderRotationY.SetPageSize(10);

	m_SliderRotationZ.SetRange(0, 360);
	m_SliderRotationZ.SetPos(0);
	m_SliderRotationZ.SetRangeMin(0);
	m_SliderRotationZ.SetRangeMax(360);
	m_SliderRotationZ.SetTicFreq(1);
	m_SliderRotationZ.SetPageSize(10);


	m_SliderScaile.SetRange(1, 100);
	m_SliderScaile.SetPos(1);
	m_SliderScaile.SetRangeMin(1);
	m_SliderScaile.SetRangeMax(100);
	m_SliderScaile.SetTicFreq(10);
	m_SliderScaile.SetPageSize(10);

	m_SliderX.SetRange(1, 200);
	m_SliderX.SetPos(1);
	m_SliderX.SetRangeMin(1);
	m_SliderX.SetRangeMax(200);
	m_SliderX.SetTicFreq(10);
	m_SliderX.SetPageSize(10);

	m_SliderY.SetRange(1, 200);
	m_SliderY.SetPos(1);
	m_SliderY.SetRangeMin(1);
	m_SliderY.SetRangeMax(200);
	m_SliderY.SetTicFreq(10);
	m_SliderY.SetPageSize(10);

	m_SliderZ.SetRange(1, 200);
	m_SliderZ.SetPos(1);
	m_SliderZ.SetRangeMin(1);
	m_SliderZ.SetRangeMax(200);
	m_SliderZ.SetTicFreq(10);
	m_SliderZ.SetPageSize(10);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_RenderObject"), LEVEL_STATIC, L"Clone_RenderObject", nullptr, (CGameObject**)&m_pRenderObject)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Arrow"), LEVEL_STATIC, L"Clone_Arrow", nullptr, (CGameObject**)&m_pArrow)))
		return E_FAIL;


	m_pTransform = (CTransform*)pGameInstance->Get_Components(LEVEL_STATIC, TEXT("Clone_Terrain"), TEXT("Com_Transform"), 0);
	if (nullptr == m_pTransform)
		return E_FAIL;

	m_pTerrainBuffer = (CVIBuffer_Terrain*)pGameInstance->Get_Components(LEVEL_STATIC, TEXT("Clone_Terrain"), TEXT("Com_VIBuffer"), 0);
	if (nullptr == m_pTerrainBuffer)
		return E_FAIL;

	Safe_Release(pGameInstance);


	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

CFloor_Tool * CMapTool2::Finder_Object(wstring ObjectTag, _uint iIndex)
{
	auto iter = find_if(m_MapObject.begin(), m_MapObject.end(), CTagFinderWstring(ObjectTag.c_str()));
	if (iter == m_MapObject.end())
		return nullptr;

	auto Listiter = iter->second;
	auto Listiter2 = Listiter.begin();


	for (_uint i = 0; i < iIndex; ++i)
		++Listiter2;

	return *Listiter2;
}

MODELDESC * CMapTool2::Finder_ModelDesc(wstring DescTag, _uint iIndex)
{
	auto iter = find_if(m_MapModelDesc.begin(), m_MapModelDesc.end(), CTagFinderWstring(DescTag.c_str()));
	if (iter == m_MapModelDesc.end())
		return nullptr;

	auto Listiter = iter->second;
	auto Listiter2 = Listiter.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++Listiter2;

	return *Listiter2;
}

HRESULT CMapTool2::Render_MapTool()
{
	if (FAILED(Check_Picking()))
		return E_FAIL;

	if (FAILED(For_Picking_Terrain()))
		return E_FAIL;


	if (!m_MapObject.empty())
	{
		Radio_Value_Set();
		for (auto& Pair : m_MapObject)
		{
			if (!Pair.second.empty())
			{
				Select_Picking();
				MESH_Picking();
				for (auto& iter : Pair.second)
				{
					iter->Set_Pos();
					iter->Set_Scaile();
					iter->Set_ScaileXYZ();
					iter->Set_Rotation();
				}
			}
		}
		Render_Object();
		Release_Picking();
	}
	return S_OK;
}

HRESULT CMapTool2::For_Picking_Terrain()
{
	if (m_CheckPicking.GetCheck() == BST_CHECKED)
	{
		if (m_strCurruntTag == L"")
			return E_FAIL;
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			m_fTimeAcc += 0.01f;
			if (0.09f <= m_fTimeAcc)
			{
				m_fTimeAcc = 0.f;
				_float3 fDist;
				_bool	bIsCheck = false;
				CGameInstance* pGameInstance = CGameInstance::Get_Instance();
				Safe_AddRef(pGameInstance);

				CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
				CMyForm2*	pMyForm = dynamic_cast<CMyForm2*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
				CString		StrCloneName;
				_uint		iObjectSelete = m_ObjectListBox.GetCurSel();
				m_ObjectListBox.GetText(iObjectSelete, StrCloneName);
				wstring aaa = StrCloneName.GetString();
				//auto iter = find(pMyForm->m_pPathFinder->m_ListFileName.begin(), pMyForm->m_pPathFinder->m_ListFileName.end(), CTagFinderWstring(aaa.c_str()));
				//*iter;
				wstring CloneTag = m_strCurruntTag;
				_uint	iLayerCount = 0;

				TCHAR	tListName[MAX_PATH] = L"";


				if (true == pGameInstance->Picking_To_Terrain(m_pTransform, m_pTerrainBuffer, &fDist))
				{
					MODELDESC* pModelDesc = new MODELDESC;
					pModelDesc->iType = CFloor_Tool::TYPE_NONANIM;
					pModelDesc->viLevel = 0;
					pModelDesc->vPos = _float4(fDist, 1.f);
					pModelDesc->vRotaition = _float3(0.f, 0.f, 0.f);
					pModelDesc->vScaile = 1.f;
					pModelDesc->vScaileXYZ = _float3(1.f, 1.f, 1.f);
					pModelDesc->wstrPrototypeTag = L"";
					pModelDesc->wstrObjKey = L"";
					pModelDesc->wstrPath = m_strCurruntTag;


					//tListName 맵 태그 맵태그가 레이어태그도된다
					auto Pair = find_if(m_MapObject.begin(), m_MapObject.end(), CTagFinderWstring(tListName));

					if (Pair == m_MapObject.end())
					{
						wsprintf(tListName, CloneTag.c_str(), iLayerCount);
					}

					if (m_strCurrentObjName == L"Map")
					{
						pModelDesc->iType = CFloor_Tool::TYPE_NONANIM;
						pModelDesc->wstrObjKey = L"Map";
						pModelDesc->wstrPrototypeTag = m_strPrototypeCurrent;
						if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Floor"), LEVEL_STATIC, tListName, pModelDesc, (CGameObject**)&m_pObject)))
							return E_FAIL;

					}
					else if (m_strCurrentObjName == L"Filter")
					{
						pModelDesc->iType = CFloor_Tool::TYPE_FILTER;
						pModelDesc->wstrObjKey = L"Filter";
						pModelDesc->wstrPrototypeTag = m_strPrototypeCurrent;
						if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Floor"), LEVEL_STATIC, tListName, pModelDesc, (CGameObject**)&m_pObject)))
							return E_FAIL;

					}
					else if (m_strCurrentObjName == L"Rotation")
					{
						pModelDesc->iType = CFloor_Tool::TYPE_ROTATION;
						pModelDesc->wstrObjKey = L"Rotation";
						pModelDesc->wstrPrototypeTag = m_strPrototypeCurrent;
						if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Floor"), LEVEL_STATIC, tListName, pModelDesc, (CGameObject**)&m_pObject)))
							return E_FAIL;

					}
					else
					{
						if (m_strPrototypeCurrent != L"Prototype_GameObject_Fiona")
							pModelDesc->vScaile = 0.01f;

						pModelDesc->iType = CFloor_Tool::TYPE_ANIM;
						pModelDesc->wstrObjKey = L"Anim";
						pModelDesc->wstrPrototypeTag = m_strPrototypeCurrent;
						if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Floor"), LEVEL_STATIC, tListName, pModelDesc, (CGameObject**)&m_pObject)))
							return E_FAIL;
					}


					auto iter = find_if(m_MapModelDesc.begin(), m_MapModelDesc.end(), CTagFinderWstring(tListName));

					if (iter == m_MapModelDesc.end())
					{
						m_ListModelDesc.push_back(pModelDesc);
						m_MapModelDesc.emplace(tListName, m_ListModelDesc);
						m_ListModelDesc.clear();
					}
					else
						iter->second.emplace_back(pModelDesc);

					////////////////////////////////////////////////////////////////////

					auto iterObject = find_if(m_MapObject.begin(), m_MapObject.end(), CTagFinderWstring(tListName));

					if (iterObject == m_MapObject.end())
					{
						dynamic_cast<CFloor_Tool*>(m_pObject)->Set_MyAddCount(0);
						m_ListObject.push_back(m_pObject);
						m_MapObject.emplace(tListName, m_ListObject);
						m_ListObject.clear();
					}
					else
					{
						_uint Counting = iterObject->second.back()->Get_MyCount() + 1;
						dynamic_cast<CFloor_Tool*>(m_pObject)->Set_MyAddCount(Counting);
						iterObject->second.emplace_back(m_pObject);
					}

					bIsCheck = true;
				}
				if (true == bIsCheck)
				{
					m_AddListBox.AddString(tListName);
				}

				Safe_Release(pGameInstance);
			}
		}
	}


	return S_OK;
}

HRESULT CMapTool2::Check_Picking()
{
	if (GetKeyState(VK_F1) & 0x8000)
	{
		if (m_CheckPicking.GetCheck() == BST_UNCHECKED)
		{
			m_CheckPicking.SetCheck(BST_CHECKED);
			MessageBox(L"Picking On", 0, MB_OK);
		}
	}
	if (GetKeyState(VK_F2) & 0x8000)
	{
		if (m_CheckPicking.GetCheck() == BST_CHECKED)
		{
			m_CheckPicking.SetCheck(BST_UNCHECKED);
			MessageBox(L"Picking Off", 0, MB_OK);
		}
	}
	if (GetKeyState(VK_F3) & 0x8000)
	{
		if (m_CheckPickPos.GetCheck() == BST_UNCHECKED)
		{
			m_CheckPickPos.SetCheck(BST_CHECKED);
			MessageBox(L"PickingPos On", 0, MB_OK);
		}
	}
	if (GetKeyState(VK_F4) & 0x8000)
	{
		if (m_CheckPickPos.GetCheck() == BST_CHECKED)
		{
			m_CheckPickPos.SetCheck(BST_UNCHECKED);
			MessageBox(L"PickingPos Off", 0, MB_OK);
		}
	}
	if (GetKeyState(0x31) & 0x8000)
	{
		if (m_CheckPickObject.GetCheck() == BST_UNCHECKED)
		{
			m_CheckPickObject.SetCheck(BST_CHECKED);
			MessageBox(L"PickingObject On", 0, MB_OK);
		}
	}
	if (GetKeyState(0x32) & 0x8000)
	{
		if (m_CheckPickObject.GetCheck() == BST_CHECKED)
		{
			m_CheckPickObject.SetCheck(BST_UNCHECKED);
			MessageBox(L"PickingObject Off", 0, MB_OK);
		}
	}


	if (GetKeyState(0x33) & 0x8000)
	{
		if (m_CheckMesh.GetCheck() == BST_UNCHECKED)
		{
			m_CheckMesh.SetCheck(BST_CHECKED);
			MessageBox(L"PickMesh On", 0, MB_OK);
		}
	}
	if (GetKeyState(0x34) & 0x8000)
	{
		if (m_CheckMesh.GetCheck() == BST_CHECKED)
		{
			m_CheckMesh.SetCheck(BST_UNCHECKED);
			MessageBox(L"PickMesh Off", 0, MB_OK);
		}
	}   


	return S_OK;
}

HRESULT CMapTool2::Release_Picking()
{
	UpdateData(TRUE);
	if (m_ButtonDelete.GetCheck() == BST_CHECKED)
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			m_fTimeAcc += 0.01f;
			if (0.09f <= m_fTimeAcc)
			{
				CGameInstance* pGameInstance = CGameInstance::Get_Instance();
				Safe_AddRef(pGameInstance);

				m_fTimeAcc = 0.f;
				_float	vDist;
				_float	fDistSave = 0.f;
				_float3 vOriginPos;
				_float3 vPickPos;
				_float3 vPosSave;
				_bool	bIsCheck = false;
				_int iCubeIndex = 0;
				_uint	iListIndex = 0;
				_bool	bCheck = false;
				wstring LayerTag = L"";

				if (m_MapObject.empty())
				{
					Safe_Release(pGameInstance);
					return E_FAIL;
				}

				for (auto& Pair : m_MapObject)
				{
					for (auto& iter : Pair.second)
					{
						if (true == pGameInstance->Picking_To_Colider(iter->m_pColiderTransformCom, iter->m_pColiderCom, &vPickPos, &vDist))
						{

							if ((fDistSave >= vDist) || fDistSave == 0.f)
							{
								fDistSave = vDist;
								iCubeIndex = m_AddListBox.FindStringExact(0, Pair.first.c_str());
								LayerTag = Pair.first;
								iListIndex = iter->Get_MyCount();
								bCheck = true;
							}
						}
					}
				}
				if (true == bCheck)
				{
					if (!m_MapModelDesc.empty())
					{
						auto iterModel = find_if(m_MapObject.begin(), m_MapObject.end(), CTagFinderWstring(LayerTag.c_str()));
						auto iterDesc = find_if(m_MapModelDesc.begin(), m_MapModelDesc.end(), CTagFinderWstring(LayerTag.c_str()));

						auto iterModelLayer = iterModel->second.begin();
						auto iterDescLayer = iterDesc->second.begin();

						for (_uint i = 0; i < iListIndex; ++i)
						{
							++iterModelLayer;
							++iterDescLayer;
						}

						if (iterModel == m_MapObject.end() || iterDesc == m_MapModelDesc.end())
						{
							Safe_Release(pGameInstance);
							return E_FAIL;
						}
						auto iterDest = iterModelLayer;

						for (; iterDest != iterModel->second.end(); ++iterDest)
						{
							(*iterDest)->Set_MyMinusCount(1);
						}

						Safe_Delete(*iterDescLayer);
						iterDesc->second.erase(iterDescLayer);


						(*iterModelLayer)->Set_DieLayer(1);
						iterModel->second.erase(iterModelLayer);


						if (iterDesc->second.empty())
							iterDesc = m_MapModelDesc.erase(iterDesc);
						if (iterModel->second.empty())
							iterModel = m_MapObject.erase(iterModel);

					}
					m_AddListBox.DeleteString(iCubeIndex);
				}

				Safe_Release(pGameInstance);
			}
		}
	}
	UpdateData(FALSE);

	return S_OK;
}

HRESULT CMapTool2::Select_Picking()
{
	if (m_CheckPickObject.GetCheck() == BST_CHECKED)
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			m_fTimeAcc += 0.01f;
			if (0.09f <= m_fTimeAcc)
			{
				CGameInstance* pGameInstance = CGameInstance::Get_Instance();
				Safe_AddRef(pGameInstance);

				m_fTimeAcc = 0.f;
				_float	vDist;
				_float	fDistSave = 0.f;
				_float3 vPickPos;
				_int	iCubeIndex = 0;
				_uint	iListIndex = 0;
				_bool	bCheck = false;
				wstring LayerTag = L"";

				if (m_MapObject.empty())
				{
					Safe_Release(pGameInstance);
					return E_FAIL;
				}

				//auto Pair = find_if(m_MapObject.begin(), m_MapObject.end(), CTagFinderWstring(m_strCurruntTag.c_str()));
				//if(Pair != m_MapObject.end())
				//{

				for(auto& Pair : m_MapObject)
				{
					for (auto& iter : Pair.second)
					{
						if (true == pGameInstance->Picking_To_Colider(iter->m_pColiderTransformCom, iter->m_pColiderCom, &vPickPos, &vDist))
						{

							if ((fDistSave >= vDist) || fDistSave == 0.f)
							{
								fDistSave = vDist;
								iCubeIndex = m_AddListBox.FindStringExact(0, Pair.first.c_str());
								LayerTag = Pair.first;
								iListIndex = iter->Get_MyCount();
								bCheck = true;
							}
						}
					}
				}
				if (true == bCheck)
				{
					if (!m_MapModelDesc.empty())
					{
						auto iterModel = find_if(m_MapObject.begin(), m_MapObject.end(), CTagFinderWstring(LayerTag.c_str()));
						auto iterDesc = find_if(m_MapModelDesc.begin(), m_MapModelDesc.end(), CTagFinderWstring(LayerTag.c_str()));

						auto iterModelLayer = iterModel->second.begin();
						auto iterDescLayer = iterDesc->second.begin();

						for (_uint i = 0; i < iListIndex; ++i)
						{
							++iterModelLayer;
							++iterDescLayer;
						}

						if (iterModel == m_MapObject.end() || iterDesc == m_MapModelDesc.end())
						{
							Safe_Release(pGameInstance);
							return E_FAIL;
						}
						wstring ComboStr = LayerTag.substr(0, LayerTag.size() - 5);
						_uint iComboIndex = m_ComboBox.FindStringExact(-1, ComboStr.c_str());
						if (-1 == iComboIndex)
						{
							Safe_Release(pGameInstance);
							return E_FAIL;
						}
						m_ComboBox.SetCurSel(iComboIndex);
						Set_ComboBox_List(iComboIndex);


						_uint iObjectIndex = m_ObjectListBox.FindStringExact(-1, LayerTag.c_str());
						if (-1 == iObjectIndex)
						{
							Safe_Release(pGameInstance);
							return E_FAIL;
						}
						m_ObjectListBox.SetCurSel(iObjectIndex);
						Set_ObjectBox_List(iObjectIndex);

						_uint iIndex = (*iterModelLayer)->Get_MyCount();
						m_AddListBox.SetCurSel(iIndex);

						Set_Object_Value(iIndex);

						CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
						CMyForm2*	pMyForm = dynamic_cast<CMyForm2*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
						pMyForm->m_pNaviTool->Set_CurrentObject(*iterModelLayer);
						m_pCurruntObject = *iterModelLayer;
					}
				}

				Safe_Release(pGameInstance);
			}
		}
	}
	UpdateData(FALSE);

	return S_OK;
}

HRESULT CMapTool2::MESH_Picking()
{
	if (m_CheckMesh.GetCheck() == BST_CHECKED)
	{
		if (m_strCurruntTag == L"")
			return E_FAIL;
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		if (pGameInstance->Key_Down(VK_LBUTTON))
		{
			m_fTimeAcc = 0.f;
			_float3 fDist;
			_float fDistSave = 0.f;
			_float	Dist = 0.f;
			_bool	bIsCheck = false;
			_bool	bCheck = false;

			CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
			CMyForm2*	pMyForm = dynamic_cast<CMyForm2*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
			CString		StrCloneName;
			_uint		iObjectSelete = m_ObjectListBox.GetCurSel();
			m_ObjectListBox.GetText(iObjectSelete, StrCloneName);
			wstring aaa = StrCloneName.GetString();
			wstring CloneTag = m_strCurruntTag;
			_uint	iLayerCount = 0;
			vector<_float3>	SortVector;
			TCHAR	tListName[MAX_PATH] = L"";

			if (nullptr == m_pCurruntObject)
				return S_OK;

			vector<CVIBuffer*>  VecContainal = *m_pCurruntObject->m_pVIBufferCom->Get_MeshContatinerVector();
			for (auto& iter : VecContainal)
			{
				if (true == pGameInstance->Picking_To_Model(m_pCurruntObject->m_pTransformCom, iter, &fDist, &Dist))
				{
					if ((fDistSave >= Dist) || fDistSave == 0.f)
					{
						fDistSave = Dist;
						SortVector.push_back(fDist);
						bCheck = true;
					}
				}
			}
			if (true == bCheck)
			{
				auto vectorbegin = SortVector.begin();
				XMStoreFloat3(&fDist, XMVector3TransformCoord(XMLoadFloat3(&(*vectorbegin)), m_pCurruntObject->m_pTransformCom->Get_WorldMatrix()));
				MODELDESC* pModelDesc = new MODELDESC;
				pModelDesc->viLevel = 0;
				pModelDesc->vPos = _float4(fDist, 1.f);
				pModelDesc->vRotaition = _float3(0.f, 0.f, 0.f);
				pModelDesc->vScaile = 1.f;
				pModelDesc->vScaileXYZ = _float3(1.f, 1.f, 1.f);
				pModelDesc->wstrPrototypeTag = L"";
				pModelDesc->wstrObjKey = L"";
				pModelDesc->wstrPath = m_strCurruntTag;


				//tListName 맵 태그 맵태그가 레이어태그도된다
				auto Pair = find_if(m_MapObject.begin(), m_MapObject.end(), CTagFinderWstring(tListName));

				if (Pair == m_MapObject.end())
				{
					wsprintf(tListName, CloneTag.c_str(), iLayerCount);
				}

				if (m_strCurrentObjName == L"Map")
				{
					pModelDesc->iType = CFloor_Tool::TYPE_NONANIM;
					pModelDesc->wstrObjKey = L"Map";
					pModelDesc->wstrPrototypeTag = m_strPrototypeCurrent;
					if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Floor"), LEVEL_STATIC, tListName, pModelDesc, (CGameObject**)&m_pObject)))
						return E_FAIL;

				}
				else if (m_strCurrentObjName == L"Filter")
				{
					pModelDesc->iType = CFloor_Tool::TYPE_FILTER;
					pModelDesc->wstrObjKey = L"Filter";
					pModelDesc->wstrPrototypeTag = m_strPrototypeCurrent;
					if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Floor"), LEVEL_STATIC, tListName, pModelDesc, (CGameObject**)&m_pObject)))
						return E_FAIL;

				}
				else if (m_strCurrentObjName == L"Rotation")
				{
					pModelDesc->iType = CFloor_Tool::TYPE_ROTATION;
					pModelDesc->wstrObjKey = L"Rotation";
					pModelDesc->wstrPrototypeTag = m_strPrototypeCurrent;
					if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Floor"), LEVEL_STATIC, tListName, pModelDesc, (CGameObject**)&m_pObject)))
						return E_FAIL;

				}
				else
				{
					if (m_strPrototypeCurrent != L"Prototype_GameObject_Fiona")
						pModelDesc->vScaile = 0.01f;

					pModelDesc->iType = CFloor_Tool::TYPE_ANIM;
					pModelDesc->wstrObjKey = L"Anim";
					pModelDesc->wstrPrototypeTag = m_strPrototypeCurrent;
					if (FAILED(pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Floor"), LEVEL_STATIC, tListName, pModelDesc, (CGameObject**)&m_pObject)))
						return E_FAIL;
				}


				auto iter = find_if(m_MapModelDesc.begin(), m_MapModelDesc.end(), CTagFinderWstring(tListName));

				if (iter == m_MapModelDesc.end())
				{
					m_ListModelDesc.push_back(pModelDesc);
					m_MapModelDesc.emplace(tListName, m_ListModelDesc);
					m_ListModelDesc.clear();
				}
				else
					iter->second.emplace_back(pModelDesc);

				////////////////////////////////////////////////////////////////////

				auto iterObject = find_if(m_MapObject.begin(), m_MapObject.end(), CTagFinderWstring(tListName));

				if (iterObject == m_MapObject.end())
				{
					dynamic_cast<CFloor_Tool*>(m_pObject)->Set_MyAddCount(0);
					m_ListObject.push_back(m_pObject);
					m_MapObject.emplace(tListName, m_ListObject);
					m_ListObject.clear();
				}
				else
				{
					_uint Counting = iterObject->second.back()->Get_MyCount() + 1;
					dynamic_cast<CFloor_Tool*>(m_pObject)->Set_MyAddCount(Counting);
					iterObject->second.emplace_back(m_pObject);
				}

				bIsCheck = true;
			}

			if (true == bIsCheck)
			{
				m_AddListBox.AddString(tListName);
			}

		}
		Safe_Release(pGameInstance);
	}


	return S_OK;
}

void CMapTool2::Set_ComboBox_List(_uint iIndex)
{
	UpdateData(FALSE);

	m_ObjectListBox.ResetContent();
	m_AddListBox.ResetContent();
	CString	StrCombo;
	m_ComboBox.GetLBText(iIndex, StrCombo);
	wstring State = StrCombo.operator LPCWSTR();
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm2*	pMyForm = dynamic_cast<CMyForm2*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
	for (auto& iter : pMyForm->m_pPathFinder->m_PathList)
	{
		if (State == iter->wstrStateKey)
		{
			TCHAR	pState[MAX_PATH] = L"";
			m_strPairName = iter->wstrStateKey;
			m_strPrototypeCurrent = iter->wstrPrototypeTag;
			//현재나의 프로토타입
			m_strCurrentObjName = iter->wstrObjKey;
			//분류할녀석
			for (_int i = 0; i < iter->iCount; ++i)
			{
				wsprintf(pState, iter->wstrPath.c_str(), i);
				m_ObjectListBox.AddString(pState);
			}
		}
	}

	UpdateData(TRUE);
}

void CMapTool2::Set_ObjectBox_List(_uint iIndex)
{

	CString	StrCombo;
	m_ObjectListBox.GetText(iIndex, StrCombo);
	m_strCurruntTag = StrCombo.operator LPCWSTR();

	if (!m_MapModelDesc.empty())
	{
		auto Pair = find_if(m_MapModelDesc.begin(), m_MapModelDesc.end(), CTagFinderWstring(m_strCurruntTag.c_str()));
		m_AddListBox.ResetContent();
		if (Pair != m_MapModelDesc.end())
		{
			auto iter = Pair->second;
			for (size_t i = 0; i < iter.size(); ++i)
			{
				CString Add = Pair->first.c_str();
				m_AddListBox.AddString(Add);
			}
		}

	}
}

void CMapTool2::Radio_Value_Set()
{
	if (m_CheckPickPos.GetCheck() == BST_CHECKED)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		m_vMouse = Get_Mouse(m_hWnd);
		if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON) & 0x80)
		{
			if ((pGameInstance->Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE::MMS_X)) && 0 < m_vMouse.x&&g_iWinCX > m_vMouse.x && 0 < m_vMouse.y&&g_iWinCY > m_vMouse.y)
			{
				if (m_vMouse.x > m_vMouseBackUp.x)
				{
					m_iSign = 1;
					m_vMouseBackUp = m_vMouse;
				}
				if (m_vMouse.x < m_vMouseBackUp.x)
				{
					m_iSign = -1;
					m_vMouseBackUp = m_vMouse;
				}
				if (nullptr == m_pModelDesc)
					return;

				m_fValueAcc[0] = m_pModelDesc->vPos.x;
				m_fValueAcc[1] = m_pModelDesc->vPos.y;
				m_fValueAcc[2] = m_pModelDesc->vPos.z;


				if (m_CheckRadio[0].GetCheck() == BST_CHECKED)
				{
					m_fValueAcc[0] += 0.1f*m_iSign;
				}
				if (m_CheckRadio[1].GetCheck() == BST_CHECKED)
				{
					m_fValueAcc[1] += 0.1f*m_iSign;
				}
				if (m_CheckRadio[2].GetCheck() == BST_CHECKED)
				{
					m_fValueAcc[2] += 0.1f*m_iSign;
				}

				m_pModelDesc->vPos = _float4(m_fValueAcc[0], m_fValueAcc[1], m_fValueAcc[2], 1.f);

				int iSelect = m_AddListBox.GetCurSel();
				CString	strTemp;

				m_AddListBox.GetText(iSelect, strTemp);
				m_pObject = Finder_Object(strTemp.GetString(), iSelect);
				if (nullptr == m_pObject)
					return;
				CString PosX, PosY, PosZ;
				PosX.Format(L"%.1f", m_pModelDesc->vPos.x);
				PosY.Format(L"%.1f", m_pModelDesc->vPos.y);
				PosZ.Format(L"%.1f", m_pModelDesc->vPos.z);
				SetDlgItemText(IDC_EDIT2, PosX);
				SetDlgItemText(IDC_EDIT3, PosY);
				SetDlgItemText(IDC_EDIT1, PosZ);

				m_pObject->Set_ModelDesc(m_pModelDesc);
			}

		}
		Safe_Release(pGameInstance);
	}
}

void CMapTool2::Render_Object()
{
	if (m_CheckRender.GetCheck() == BST_CHECKED)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		_float3 fDist;
		if (true == pGameInstance->Picking_To_Terrain(m_pTransform, m_pTerrainBuffer, &fDist))
		{
			MODELDESC* pModelDesc = new MODELDESC;
			ZeroMemory(pModelDesc, sizeof(MODELDESC));

			pModelDesc->wstrPath = m_strCurruntTag;
			pModelDesc->vPos = _float4(fDist, 1.f);
			m_pRenderObject->Set_ModelDesc(pModelDesc);
			Safe_Delete(pModelDesc);
		}

		Safe_Release(pGameInstance);
	}
}

void CMapTool2::Set_Object_Value(_uint iIndex)
{
	UpdateData(TRUE);

	CString StrListBox = L"";

	m_AddListBox.GetText(iIndex, StrListBox);

	m_pModelDesc = Finder_ModelDesc(StrListBox.GetString(), iIndex);
	m_pObject = Finder_Object(StrListBox.GetString(), iIndex);
	if (nullptr == m_pModelDesc)
	{
		MessageBox(L"Finder Failed ModelDesc", 0, MB_OK);
		return;
	}
	if (nullptr == m_pObject)
	{
		MessageBox(L"Finder Failed m_pObject", 0, MB_OK);
		return;
	}

	m_pObject->Set_ModelDesc(m_pModelDesc);
	m_pArrow->Set_Pos(m_pModelDesc->vPos);


	CString	strTest;
	strTest.Format(L"%.1f", m_pModelDesc->vPos.x);
	m_PosX.SetWindowText(strTest);


	CString	strTest1;
	strTest1.Format(L"%.1f", m_pModelDesc->vPos.y);
	m_PosY.SetWindowText(strTest1);

	CString	strTest2;
	strTest2.Format(L"%.1f", m_pModelDesc->vPos.z);
	m_PosZ.SetWindowText(strTest2);

	CString	RotX;
	RotX.Format(L"%.1f", m_pModelDesc->vRotaition.x);
	m_SliderRotationX.SetPos((_int)m_pModelDesc->vRotaition.x * 10);
	m_EditRotX.SetWindowText(RotX);

	CString	RotY;
	RotY.Format(L"%.1f", m_pModelDesc->vRotaition.y);
	m_SliderRotationY.SetPos((_int)m_pModelDesc->vRotaition.y * 10);
	m_EditRotY.SetWindowText(RotY);

	CString	RotZ;
	RotZ.Format(L"%.1f", m_pModelDesc->vRotaition.z);
	m_SliderRotationZ.SetPos((_int)m_pModelDesc->vRotaition.z * 10);
	m_EditRotZ.SetWindowText(RotZ);

	CString	Scale;
	Scale.Format(L"%.1f", m_pModelDesc->vScaile);
	m_SliderScaile.SetPos((_int)m_pModelDesc->vScaile * 10);
	m_EditScaile.SetWindowText(Scale);

	CString	ScaleX;
	ScaleX.Format(L"%.1f", m_pModelDesc->vScaileXYZ.x);
	m_SliderX.SetPos((_int)m_pModelDesc->vScaileXYZ.x * 10);
	m_EditScaleX.SetWindowText(ScaleX);

	CString	ScaleY;
	ScaleY.Format(L"%.1f", m_pModelDesc->vScaileXYZ.y);
	m_SliderY.SetPos((_int)m_pModelDesc->vScaileXYZ.y * 10);
	m_EditScaleY.SetWindowText(ScaleY);

	CString	ScaleZ;
	ScaleZ.Format(L"%.1f", m_pModelDesc->vScaileXYZ.z);
	m_SliderZ.SetPos((_int)m_pModelDesc->vScaileXYZ.z * 10);
	m_EditScaleZ.SetWindowText(ScaleZ);


	UpdateData(FALSE);
}

void CMapTool2::OnCbnSelchangeComboBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(FALSE);

	m_ObjectListBox.ResetContent();
	m_AddListBox.ResetContent();
	_uint iIndex = m_ComboBox.GetCurSel();
	CString	StrCombo;
	m_ComboBox.GetLBText(iIndex, StrCombo);
	wstring State = StrCombo.operator LPCWSTR();
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMyForm2*	pMyForm = dynamic_cast<CMyForm2*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
	for (auto& iter : pMyForm->m_pPathFinder->m_PathList)
	{
		if (State == iter->wstrStateKey)
		{
			TCHAR	pState[MAX_PATH] = L"";

			m_strPrototypeCurrent = iter->wstrPrototypeTag;
			//현재나의 프로토타입
			m_strCurrentObjName = iter->wstrObjKey;
			//분류할녀석
			for (_int i = 0; i < iter->iCount; ++i)
			{
				wsprintf(pState, iter->wstrPath.c_str(), i);
				m_ObjectListBox.AddString(pState);
			}
		}
	}

	UpdateData(TRUE);
}


void CMapTool2::OnLbnObjectSeleteChancePick()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	_uint iIndex = m_ObjectListBox.GetCurSel();
	CString	StrCombo;
	m_ObjectListBox.GetText(iIndex, StrCombo);
	m_strCurruntTag = StrCombo.operator LPCWSTR();

	if (!m_MapModelDesc.empty())
	{
		auto Pair = find_if(m_MapModelDesc.begin(), m_MapModelDesc.end(), CTagFinderWstring(m_strCurruntTag.c_str()));
		m_AddListBox.ResetContent();
		if (Pair != m_MapModelDesc.end())
		{
			auto iter = Pair->second;
			for (size_t i = 0; i < iter.size(); ++i)
			{
				CString Add = Pair->first.c_str();
				m_AddListBox.AddString(Add);
			}
		}

	}
}


void CMapTool2::OnBnClickedDeleteButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_uint iSeletIndex = m_AddListBox.GetCurSel();
	CString StrListBox = L"";
	m_AddListBox.GetText(iSeletIndex, StrListBox);

	if (!m_MapModelDesc.empty())
	{
		auto iter = find_if(m_MapModelDesc.begin(), m_MapModelDesc.end(), CTagFinderWstring(StrListBox.GetString()));
		if (iter == m_MapModelDesc.end())
			return;

		//auto Listiter = iter->second;
		auto TTT = iter->second.begin();

		if (0 != iSeletIndex)
		{
			for (_uint i = 0; i < iSeletIndex; ++i)
			{
				++TTT;
			}
		}
		Safe_Delete(*TTT);
		TTT = iter->second.erase(TTT);

		if (m_MapModelDesc.empty())
		{
			iter = m_MapModelDesc.erase(iter);
		}
	}
	if (!m_MapObject.empty())
	{
		auto iter = find_if(m_MapObject.begin(), m_MapObject.end(), CTagFinderWstring(StrListBox.GetString()));
		if (iter == m_MapObject.end())
			return;

		//auto Listiter11 = iter->second;
		auto Listiter22 = iter->second.begin();
		if (0 != iSeletIndex)
		{
			for (_uint i = 0; i < iSeletIndex; ++i)
			{
				++Listiter22;
			}
		}

		(*Listiter22)->Set_DieLayer(1);
		iter->second.erase(Listiter22++);

		if (m_MapObject.empty())
		{
			iter = m_MapObject.erase(iter);
		}
	}
	m_AddListBox.DeleteString(iSeletIndex);


}


void CMapTool2::OnBnClickedPickingOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMapTool2::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	_int iSeletIndex = m_AddListBox.GetCurSel();
	CString StrListBox = L"";

	m_AddListBox.GetText(iSeletIndex, StrListBox);

	m_pModelDesc = Finder_ModelDesc(StrListBox.GetString(), iSeletIndex);
	m_pObject = Finder_Object(StrListBox.GetString(), iSeletIndex);
	if (nullptr == m_pModelDesc)
	{
		MessageBox(L"Finder Failed ModelDesc", 0, MB_OK);
		return;
	}

	if (nullptr == m_pModelDesc)
		return;

	if (pScrollBar)
	{
		if (pScrollBar == (CScrollBar*)&m_SliderRotationX)
		{
			_int iRotX = m_SliderRotationX.GetPos();
			m_pModelDesc->vRotaition.x = ((_float)iRotX);
			CString	RotX;
			RotX.Format(L"%.1f", m_pModelDesc->vRotaition.x);
			m_EditRotX.SetWindowText(RotX);
		}
		if (pScrollBar == (CScrollBar*)&m_SliderRotationY)
		{
			_int iRotY = m_SliderRotationY.GetPos();
			m_pModelDesc->vRotaition.y = ((_float)iRotY);
			CString	RotY;
			RotY.Format(L"%.1f", m_pModelDesc->vRotaition.y);
			m_EditRotY.SetWindowText(RotY);

		}
		if (pScrollBar == (CScrollBar*)&m_SliderRotationZ)
		{
			_int iRotZ = m_SliderRotationZ.GetPos();
			m_pModelDesc->vRotaition.z = ((_float)iRotZ);
			CString	RotZ;
			RotZ.Format(L"%.1f", m_pModelDesc->vRotaition.z);
			m_EditRotZ.SetWindowText(RotZ);

		}
		if (pScrollBar == (CScrollBar*)&m_SliderScaile)
		{
			_int iScaileX = m_SliderScaile.GetPos();
			m_pModelDesc->vScaile = ((_float)iScaileX / 10.f);
			CString	Scale;
			Scale.Format(L"%.1f", m_pModelDesc->vScaile);
			m_EditScaile.SetWindowText(Scale);
		}
		if (pScrollBar == (CScrollBar*)&m_SliderX)
		{
			_int iScaileX = m_SliderX.GetPos();
			m_pModelDesc->vScaileXYZ.x = ((_float)iScaileX / 10.f);
			CString	ScaleX;
			ScaleX.Format(L"%.1f", m_pModelDesc->vScaileXYZ.x);
			m_EditScaleX.SetWindowText(ScaleX);
		}
		if (pScrollBar == (CScrollBar*)&m_SliderY)
		{
			_int iScaileY = m_SliderY.GetPos();
			m_pModelDesc->vScaileXYZ.y = ((_float)iScaileY / 10.f);
			CString	ScaleY;
			ScaleY.Format(L"%.1f", m_pModelDesc->vScaileXYZ.y);
			m_EditScaleY.SetWindowText(ScaleY);
		}
		if (pScrollBar == (CScrollBar*)&m_SliderZ)
		{
			_int iScaileZ = m_SliderZ.GetPos();
			m_pModelDesc->vScaileXYZ.z = ((_float)iScaileZ / 10.f);
			CString	ScaleZ;
			ScaleZ.Format(L"%.1f", m_pModelDesc->vScaileXYZ.z);
			m_EditScaleZ.SetWindowText(ScaleZ);
		}

	}
	m_pObject->Set_ModelDesc(m_pModelDesc);


	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}



void CMapTool2::OnLbnSelchangeModelDesc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	_int iSeletIndex = m_AddListBox.GetCurSel();
	CString StrListBox = L"";

	m_AddListBox.GetText(iSeletIndex, StrListBox);

	m_pModelDesc = Finder_ModelDesc(StrListBox.GetString(), iSeletIndex);
	m_pObject = Finder_Object(StrListBox.GetString(), iSeletIndex);
	if (nullptr == m_pModelDesc)
	{
		MessageBox(L"Finder Failed ModelDesc", 0, MB_OK);
		return;
	}
	if (nullptr == m_pObject)
	{
		MessageBox(L"Finder Failed m_pObject", 0, MB_OK);
		return;
	}

	m_pObject->Set_ModelDesc(m_pModelDesc);
	m_pArrow->Set_Pos(m_pModelDesc->vPos);


	CString	strTest;
	strTest.Format(L"%.1f", m_pModelDesc->vPos.x);
	m_PosX.SetWindowText(strTest);


	CString	strTest1;
	strTest1.Format(L"%.1f", m_pModelDesc->vPos.y);
	m_PosY.SetWindowText(strTest1);

	CString	strTest2;
	strTest2.Format(L"%.1f", m_pModelDesc->vPos.z);
	m_PosZ.SetWindowText(strTest2);

	CString	RotX;
	RotX.Format(L"%.1f", m_pModelDesc->vRotaition.x);
	m_SliderRotationX.SetPos((_int)m_pModelDesc->vRotaition.x * 10);
	m_EditRotX.SetWindowText(RotX);

	CString	RotY;
	RotY.Format(L"%.1f", m_pModelDesc->vRotaition.y);
	m_SliderRotationY.SetPos((_int)m_pModelDesc->vRotaition.y * 10);
	m_EditRotY.SetWindowText(RotY);

	CString	RotZ;
	RotZ.Format(L"%.1f", m_pModelDesc->vRotaition.z);
	m_SliderRotationZ.SetPos((_int)m_pModelDesc->vRotaition.z * 10);
	m_EditRotZ.SetWindowText(RotZ);

	CString	Scale;
	Scale.Format(L"%.1f", m_pModelDesc->vScaile);
	m_SliderScaile.SetPos((_int)m_pModelDesc->vScaile * 10);
	m_EditScaile.SetWindowText(Scale);

	CString	ScaleX;
	ScaleX.Format(L"%.1f", m_pModelDesc->vScaileXYZ.x);
	m_SliderX.SetPos((_int)m_pModelDesc->vScaileXYZ.x * 10);
	m_EditScaleX.SetWindowText(ScaleX);

	CString	ScaleY;
	ScaleY.Format(L"%.1f", m_pModelDesc->vScaileXYZ.y);
	m_SliderY.SetPos((_int)m_pModelDesc->vScaileXYZ.y * 10);
	m_EditScaleY.SetWindowText(ScaleY);

	CString	ScaleZ;
	ScaleZ.Format(L"%.1f", m_pModelDesc->vScaileXYZ.z);
	m_SliderZ.SetPos((_int)m_pModelDesc->vScaileXYZ.z * 10);
	m_EditScaleZ.SetWindowText(ScaleZ);


	UpdateData(FALSE);
}


void CMapTool2::OnBnClickedSetUpPosButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMapTool2::OnDeltaposPosX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_int iSeletIndex = m_AddListBox.GetCurSel();
	CString StrListBox = L"";

	m_AddListBox.GetText(iSeletIndex, StrListBox);

	m_pModelDesc = Finder_ModelDesc(StrListBox.GetString(), iSeletIndex);
	m_pObject = Finder_Object(StrListBox.GetString(), iSeletIndex);

	if (nullptr == m_pModelDesc)
	{
		MessageBox(L"Finder Failed ModelDesc", 0, MB_OK);
		return;
	}
	_float fValue = m_pModelDesc->vPos.x + (pNMUpDown->iDelta / 10.f);
	m_pModelDesc->vPos.x = fValue;
	CString		strValue = L"";
	strValue.Format(L"%.1f", m_pModelDesc->vPos.x);
	m_PosX.SetWindowText(strValue);

	m_pObject->Set_ModelDesc(m_pModelDesc);


	*pResult = 0;
}


void CMapTool2::OnDeltaposPosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	_int iSeletIndex = m_AddListBox.GetCurSel();
	CString StrListBox = L"";

	m_AddListBox.GetText(iSeletIndex, StrListBox);

	m_pModelDesc = Finder_ModelDesc(StrListBox.GetString(), iSeletIndex);
	m_pObject = Finder_Object(StrListBox.GetString(), iSeletIndex);
	if (nullptr == m_pModelDesc)
	{
		MessageBox(L"Finder Failed ModelDesc", 0, MB_OK);
		return;
	}
	_float fValue = m_pModelDesc->vPos.y + (pNMUpDown->iDelta / 10.f);
	m_pModelDesc->vPos.y = fValue;
	CString		strValue = L"";
	strValue.Format(L"%.1f", m_pModelDesc->vPos.y);
	m_PosY.SetWindowText(strValue);

	m_pObject->Set_ModelDesc(m_pModelDesc);


	*pResult = 0;
}


void CMapTool2::OnDeltaposPosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	_int iSeletIndex = m_AddListBox.GetCurSel();
	CString StrListBox = L"";

	m_AddListBox.GetText(iSeletIndex, StrListBox);

	m_pModelDesc = Finder_ModelDesc(StrListBox.GetString(), iSeletIndex);
	m_pObject = Finder_Object(StrListBox.GetString(), iSeletIndex);
	if (nullptr == m_pModelDesc)
	{
		MessageBox(L"Finder Failed ModelDesc", 0, MB_OK);
		return;
	}
	_float fValue = m_pModelDesc->vPos.z + (pNMUpDown->iDelta / 10.f);
	m_pModelDesc->vPos.z = fValue;
	CString		strValue = L"";
	strValue.Format(L"%.1f", m_pModelDesc->vPos.z);
	m_PosZ.SetWindowText(strValue);

	m_pObject->Set_ModelDesc(m_pModelDesc);
	*pResult = 0;
}



void CMapTool2::OnBnClickedSaveButton()
{
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

		DWORD	dwPairName = 0;
		DWORD	dwStrPrototype = 0; //레이어의프로토타입녀석
		DWORD	dwStrObjectKey = 0; //맵인지 애니메이션인지 구별할녀석
		DWORD	dwStrPath = 0; //레이어태그 //모델이름
		DWORD	dwByte = 0;

		_float4x4	WorldMatrix;

		//_uint			iType;
		//_uint			viLevel;
		//_float		vScaile;
		//_float3		vScaileXYZ;
		//_float3		vRotaition;
		//_float4		vPos;
		//월드매트릭스로 통합할예정

		for (auto& Pair : m_MapModelDesc)
		{
			//dwPairName = sizeof(DWORD) * (Pair.first.length() + 1);
			//WriteFile(hFile, &dwPairName,sizeof(DWORD), &dwByte, nullptr);
			//WriteFile(hFile, Pair.first.c_str(), sizeof(dwPairName),&dwByte, nullptr);

			for (auto& PairSecond : Pair.second)
			{
				wstring Prototype = PairSecond->wstrPrototypeTag;
				wstring ObjectKey = PairSecond->wstrObjKey;
				wstring wstrPath = PairSecond->wstrPath;

				dwStrPrototype = sizeof(DWORD) * (Prototype.length() + 1);
				dwStrObjectKey = sizeof(DWORD) * (ObjectKey.length() + 1);
				dwStrPath = sizeof(DWORD) * (wstrPath.length() + 1);

				WriteFile(hFile, &dwStrPrototype, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, Prototype.c_str(), dwStrPrototype, &dwByte, nullptr);

				WriteFile(hFile, &dwStrObjectKey, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, ObjectKey.c_str(), dwStrObjectKey, &dwByte, nullptr);

				WriteFile(hFile, &dwStrPath, sizeof(DWORD), &dwByte, nullptr);
				WriteFile(hFile, wstrPath.c_str(), dwStrPath, &dwByte, nullptr);

				WriteFile(hFile, &PairSecond->iType, sizeof(_uint), &dwByte, nullptr);
				WriteFile(hFile, &PairSecond->viLevel, sizeof(_uint), &dwByte, nullptr);
				WriteFile(hFile, &PairSecond->vScaile, sizeof(_float), &dwByte, nullptr);
				WriteFile(hFile, &PairSecond->vScaileXYZ, sizeof(_float3), &dwByte, nullptr);
				WriteFile(hFile, &PairSecond->vRotaition, sizeof(_float3), &dwByte, nullptr);
				WriteFile(hFile, &PairSecond->vPos, sizeof(_float4), &dwByte, nullptr);
			}
		}
		CloseHandle(hFile);

		Safe_Release(pGameInstance);
	}

}


void CMapTool2::OnBnClickedLoadButton()
{

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

		HANDLE hFile = CreateFile(pPath2, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		for (auto& Pair : m_MapObject)
		{
			for (auto& iter : Pair.second)
			{
				(*iter).Set_DieLayer(1);
			}

			Pair.second.clear();
		}
		m_MapObject.clear();

		for (auto& Pair : m_MapModelDesc)
		{
			for (auto& iter : Pair.second)
				Safe_Delete(iter);

			Pair.second.clear();
		}
		m_MapModelDesc.clear();


		DWORD	dwPairName = 0;
		DWORD	dwStrPrototype = 0; //레이어의프로토타입녀석
		DWORD	dwStrObjectKey = 0; //맵인지 애니메이션인지 구별할녀석DWORD dwByte = 0;
		DWORD	dwStrPath = 0; //레이어태그 //모델이름
		DWORD	dwByte = 0;

		_tchar*	pPairName = nullptr;
		_tchar*	pPrototypeTag = nullptr;
		_tchar*	pObjectKey = nullptr;
		_tchar*	pPath = nullptr;

		_uint		iType;
		_uint		viLevel;
		_float		vScaile;
		_float3		vScaileXYZ;
		_float3		vRotaition;
		_float4		vPos;


		while (true)
		{
			ReadFile(hFile, &dwStrPrototype, sizeof(DWORD), &dwByte, nullptr);
			pPrototypeTag = new _tchar[dwStrPrototype];
			ReadFile(hFile, pPrototypeTag, dwStrPrototype, &dwByte, nullptr);

			ReadFile(hFile, &dwStrObjectKey, sizeof(DWORD), &dwByte, nullptr);
			pObjectKey = new _tchar[dwStrObjectKey];
			ReadFile(hFile, pObjectKey, dwStrObjectKey, &dwByte, nullptr);

			ReadFile(hFile, &dwStrPath, sizeof(DWORD), &dwByte, nullptr);
			pPath = new _tchar[dwStrPath];
			ReadFile(hFile, pPath, dwStrPath, &dwByte, nullptr);

			ReadFile(hFile, &iType, sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &viLevel, sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, &vScaile, sizeof(_float), &dwByte, nullptr);
			ReadFile(hFile, &vScaileXYZ, sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, &vRotaition, sizeof(_float3), &dwByte, nullptr);
			ReadFile(hFile, &vPos, sizeof(_float4), &dwByte, nullptr);

			if (0 == dwByte)
			{
				Safe_Delete_Array(pPairName);
				Safe_Delete_Array(pPrototypeTag);
				Safe_Delete_Array(pObjectKey);
				Safe_Delete_Array(pPath);
				break;
			}


			MODELDESC*	pModelDesc = new MODELDESC;
			pModelDesc->wstrPrototypeTag = pPrototypeTag;
			pModelDesc->wstrObjKey = pObjectKey;
			pModelDesc->wstrPath = pPath;
			pModelDesc->iType = iType;
			pModelDesc->viLevel = viLevel;
			pModelDesc->vScaile = vScaile;
			pModelDesc->vScaileXYZ = vScaileXYZ;
			pModelDesc->vRotaition = vRotaition;
			pModelDesc->vPos = vPos;


			auto iter = find_if(m_MapModelDesc.begin(), m_MapModelDesc.end(), CTagFinderWstring(pPath));

			if (iter == m_MapModelDesc.end())
			{
				m_ListModelDesc.emplace_back(pModelDesc);
				m_MapModelDesc.emplace(pPath, m_ListModelDesc);
				m_ListModelDesc.clear();
			}
			else
				iter->second.emplace_back(pModelDesc);



			Safe_Delete_Array(pPairName);
			Safe_Delete_Array(pPrototypeTag);
			Safe_Delete_Array(pObjectKey);
			Safe_Delete_Array(pPath);
		}
		CloseHandle(hFile);

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		for (auto& Pair : m_MapModelDesc)
		{
			auto PairObject = find_if(m_MapObject.begin(), m_MapObject.end(), CTagFinderWstring(Pair.first.c_str()));

			if (PairObject == m_MapObject.end())
			{
				_uint iIndex = 0;

				for (auto& PairSecond : Pair.second)
				{
					if (PairSecond->wstrObjKey == L"Map")
					{
						MODELDESC*	pModelDesc = new MODELDESC;
						pModelDesc = PairSecond;
						pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Floor"), LEVEL_STATIC, PairSecond->wstrPath.c_str(), pModelDesc, (CGameObject**)&m_pObject);
						if (nullptr == m_pObject)
						{
							Safe_Release(pGameInstance);
							return;
						}
						dynamic_cast<CFloor_Tool*>(m_pObject)->Set_MyAddCount(iIndex++);
						m_ListObject.push_back(m_pObject);
					}
					else
					{
						MODELDESC*	pModelDesc = new MODELDESC;
						memcpy(pModelDesc, PairSecond, sizeof(MODELDESC));
						pGameInstance->Add_ObjectToLayer(LEVEL_STATIC, TEXT("Prototype_GameObject_Floor"), LEVEL_STATIC, PairSecond->wstrPath.c_str(), pModelDesc, (CGameObject**)&m_pObject);
						if (nullptr == m_pObject)
						{
							Safe_Release(pGameInstance);
							return;
						}
						dynamic_cast<CFloor_Tool*>(m_pObject)->Set_MyAddCount(iIndex++);
						m_ListObject.push_back(m_pObject);
					}
				}
				m_MapObject.emplace(Pair.first, m_ListObject);
				m_ListObject.clear();
			}
		}

		Safe_Release(pGameInstance);
	}
}
