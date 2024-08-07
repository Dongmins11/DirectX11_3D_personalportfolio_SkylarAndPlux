#include "..\public\GameInstance.h"

IMPLEMENT_SINGLETON(CGameInstance)
CGameInstance::CGameInstance()
	:m_pGraphic_Device(CGraphic_Device::Get_Instance())
	, m_pInput_Device(CInput_Device::Get_Instance())
	, m_pTime_Manager(CTime_Manager::Get_Instance())
	, m_pLevel_Manager(CLevel_Manager::Get_Instance())
	, m_pGameObject_Manager(CGameObject_Manager::Get_Instance())
	, m_pComponent_Manager(CComponent_Manager::Get_Instance())
	, m_pPipeLine(CPipeLine::Get_Instance())
	, m_pLight_Manager(CLight_Manager::Get_Instance())
	, m_pPicking(CPicking::Get_Instance())
	, m_pLoad_Manager(CLoad_Manager::Get_Instance())
	, m_pKey_Manager(CKey_Manager::Get_Instance())
	, m_pAnimation_Manager(CAnimation_Control_Manager::Get_Instance())
	, m_pTarget_Manager(CRenderTarget_Manager::Get_Instance())
	, m_pFont_Manager(CFont_Manager::Get_Instance())
	, m_pFrustum(CFrustum::Get_Instance())
{
	Safe_AddRef(m_pFont_Manager);
	Safe_AddRef(m_pAnimation_Manager);
	Safe_AddRef(m_pKey_Manager);
	Safe_AddRef(m_pPicking);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pGameObject_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pTime_Manager);
	Safe_AddRef(m_pLoad_Manager);
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pFrustum);
}

HRESULT CGameInstance::Initalize_Engine(_uint iNumLevel,HINSTANCE hInst, const CGraphic_Device::DEVICEDESC & tDeivce_Desc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pInput_Device ||
		nullptr == m_pGameObject_Manager ||
		nullptr == m_pComponent_Manager ||
		nullptr == m_pLoad_Manager ||
		nullptr == m_pTarget_Manager ||
		nullptr == m_pLight_Manager)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->Ready_For_Device(tDeivce_Desc, ppDeviceOut, ppDeviceContextOut)))
		return E_FAIL;

	if (FAILED(m_pInput_Device->Ready_Input_Device(hInst, tDeivce_Desc.hWnd)))
		return E_FAIL;

	if (FAILED(m_pGameObject_Manager->Reserve_GameObjectManager(iNumLevel)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Reserve_Manager(iNumLevel)))
		return E_FAIL;

	if (FAILED(m_pFrustum->Intialize_Frustum()))
		return E_FAIL;

	if (FAILED(m_pPicking->SetUp_Picking(tDeivce_Desc.hWnd, *ppDeviceContextOut)))
		return E_FAIL;

	if (FAILED(m_pLoad_Manager->Set_Up_LoadFinder(*ppDeviceOut, *ppDeviceContextOut)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->NativeConsturct(*ppDeviceOut, *ppDeviceContextOut)))
		return E_FAIL;

	if (FAILED(m_pLight_Manager->NativeConstruct(*ppDeviceOut, *ppDeviceContextOut)))
		return E_FAIL;


	return S_OK;
}

HRESULT CGameInstance::Initalize_Engine_Tool(_uint iNumLevel, HINSTANCE hInst, const CGraphic_Device::DEVICEDESC & tDeivce_Desc, ID3D11Device ** ppDeviceOut, ID3D11DeviceContext ** ppDeviceContextOut)
{
	if (nullptr == m_pGraphic_Device ||
		nullptr == m_pInput_Device ||
		nullptr == m_pGameObject_Manager ||
		nullptr == m_pComponent_Manager ||
		nullptr == m_pPicking ||
		nullptr == m_pTarget_Manager ||
		nullptr == m_pLight_Manager)
		return E_FAIL;

	if (FAILED(m_pGraphic_Device->Ready_For_Device(tDeivce_Desc, ppDeviceOut, ppDeviceContextOut)))
		return E_FAIL;

	if (FAILED(m_pInput_Device->Ready_Input_Device(hInst, nullptr)))
		return E_FAIL;

	if (FAILED(m_pGameObject_Manager->Reserve_GameObjectManager(iNumLevel)))
		return E_FAIL;

	if (FAILED(m_pComponent_Manager->Reserve_Manager(iNumLevel)))
		return E_FAIL;

	if (FAILED(m_pPicking->SetUp_Picking(tDeivce_Desc.hWnd,*ppDeviceContextOut)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->NativeConsturct(*ppDeviceOut, *ppDeviceContextOut)))
		return E_FAIL;

	if (FAILED(m_pLight_Manager->NativeConstruct(*ppDeviceOut, *ppDeviceContextOut)))
		return E_FAIL;

	return S_OK;
}

_int CGameInstance::Tick_Engine(_double TimeDelta)
{
	if (nullptr == m_pInput_Device ||
		nullptr == m_pLevel_Manager ||
		nullptr == m_pGameObject_Manager ||
		nullptr == m_pPipeLine)
		return -1;

	m_pInput_Device->Setup_InputDeviceState();

	m_pGameObject_Manager->Tick(TimeDelta);

	m_pFrustum->Update();

	m_pPipeLine->Update_Matrix();

	Transform_ToWorldSapce();

	m_pGameObject_Manager->LateTick(TimeDelta);

	m_pLevel_Manager->Tick(TimeDelta);

	return _int();
}

HRESULT CGameInstance::Render_Engine()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	if (FAILED(m_pLevel_Manager->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Clear_LevelResource(_uint iLevelIndex)
{
	if (nullptr == m_pGameObject_Manager ||
		nullptr == m_pComponent_Manager)
		return E_FAIL;

	if (FAILED(m_pGameObject_Manager->Clear_Object(iLevelIndex)))
		return E_FAIL;
	
	if (FAILED(m_pComponent_Manager->Clear_Component(iLevelIndex)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_Depth_Stencil_View()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_Depth_Stencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Present();
}

_byte CGameInstance::Get_DIKeyState(_ubyte eKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(eKeyID);
}

_long CGameInstance::Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseMoveState(eMouseMoveState);
}

_byte CGameInstance::Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseButtonState(eMouseButtonState);
}

HRESULT CGameInstance::Add_Timer(const _tchar * pTimerTag)
{
	if (nullptr == m_pTime_Manager)
		return E_FAIL;

	return m_pTime_Manager->Add_Timer(pTimerTag);
}

_double CGameInstance::Compute_TimeDleta(const _tchar * pTimerTag)
{
	if (nullptr == m_pTime_Manager)
		return 0.0;

	return m_pTime_Manager->Compute_TimeDleta(pTimerTag);
}

HRESULT CGameInstance::Open_Level(_uint iCurrurntLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iCurrurntLevelIndex,pNewLevel);
}

CComponent * CGameInstance::Get_Components(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex, _uint iLayerIndex)
{
	if (nullptr == m_pGameObject_Manager)
		return nullptr;

	return m_pGameObject_Manager->Get_Components(iLevelIndex, pLayerTag, pComponentTag, iIndex, iLayerIndex);
}

CGameObject * CGameInstance::Get_GameObject(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pGameObject_Manager)
		return nullptr;

	return m_pGameObject_Manager->Get_GameObject(iLevelIndex, pLayerTag);
}

HRESULT CGameInstance::Release_GameObject(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pGameObject_Manager)
		return E_FAIL;

	return m_pGameObject_Manager->Release_GameObject(iLevelIndex, pLayerTag);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _tchar * pPrototypeTag, CGameObject * pGameObject)
{
	if (nullptr == m_pGameObject_Manager)
		return E_FAIL;

	return m_pGameObject_Manager->Add_Prototype(iLevelIndex, pPrototypeTag, pGameObject);
}

HRESULT CGameInstance::Add_ObjectToLayer(_uint iLevelIndex, const _tchar * pPrototypeTag, _uint iLayerLevelIndex, const _tchar * pLayerTag, void * pArg, CGameObject ** ppOutGameObject)
{
	if (nullptr == m_pGameObject_Manager)
		return E_FAIL;

	return m_pGameObject_Manager->Add_ObjectToLayer(iLevelIndex, pPrototypeTag, iLayerLevelIndex, pLayerTag, pArg, ppOutGameObject);
}

_uint CGameInstance::Get_GameObjectSize(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pGameObject_Manager)
		return -1;

	return m_pGameObject_Manager->Get_GameObjectSize(iLevelIndex, pLayerTag);
}

HRESULT CGameInstance::Add_Prototype(_uint iNumLevelIndex, const _tchar * pPrototypeTag, CComponent * pComponents)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iNumLevelIndex, pPrototypeTag, pComponents);
}

CComponent * CGameInstance::Clone_Component(_uint iNumLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iNumLevelIndex, pPrototypeTag, pArg);
}

void CGameInstance::Set_HoldingMatrix(CPipeLine::TRANSFORMSTATETYPE eState, _fmatrix HoldingMatrix)
{
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_HoldingMatrix(eState, HoldingMatrix);
}

_matrix CGameInstance::Get_HoldingMatrix(CPipeLine::TRANSFORMSTATETYPE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_HoldingMatrix(eState);
}

_vector CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
		return XMVectorZero();

	return m_pPipeLine->Get_CamPosition();
}

HRESULT CGameInstance::Set_LightDesc(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const LIGHTDESC & tLightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Set_LightDesc(pDevice,pDeviceContext,tLightDesc);
}

LIGHTDESC * CGameInstance::Get_LighDesc(_uint iIndex)
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LighDesc(iIndex);
}

void CGameInstance::Set_LightRenderCheck(_uint iIndex, _bool bCheck)
{
	if (nullptr == m_pLight_Manager)
		return;

	m_pLight_Manager->Set_LightRenderCheck(iIndex, bCheck);
}

HRESULT CGameInstance::Clear_Light()
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Clear_Light();
}

void CGameInstance::Transform_ToWorldSapce()
{
	if (nullptr == m_pPicking)
		return;

	m_pPicking->Transform_ToWorldSapce();
}

_bool CGameInstance::Picking_To_Terrain(CTransform * pTransform, CVIBuffer * pVIBuffer, _float3 * vOut)
{
	if (nullptr == m_pPicking)
		return false;

	return m_pPicking->Picking_To_Terrain(pTransform, pVIBuffer, vOut);
}

_bool CGameInstance::Picking_To_Model(CTransform * pTransform, CVIBuffer * pVIBuffer, _float3 * vOut, _float * fOutDist)
{
	if (nullptr == m_pPicking)
		return false;
	return m_pPicking->Picking_To_Model(pTransform, pVIBuffer, vOut, fOutDist);
}

_bool CGameInstance::Picking_To_Colider(CTransform * pTransform, CVIBuffer * pVIBuffer, _float3 * vOut, _float * fOutDist)
{
	if (nullptr == m_pPicking)
		return false;

	return m_pPicking->Picking_To_Colider(pTransform, pVIBuffer, vOut, fOutDist);
}

HRESULT CGameInstance::Load_PathFinder_To_Model(const _tchar * tPath, _uint iLevel)
{
	if (nullptr == m_pLoad_Manager)
		return E_FAIL;

	return m_pLoad_Manager->Load_PathFinder_To_Model(tPath, iLevel);
}

HRESULT CGameInstance::Load_GameObject_Tool(const _tchar * pFilePath, const _tchar * pMapPrototypeTag, _uint iLevel)
{
	if (nullptr == m_pLoad_Manager)
		return E_FAIL;
		
	return m_pLoad_Manager->Load_GameObject_Tool(pFilePath,pMapPrototypeTag,iLevel);
}

_bool CGameInstance::Key_Up(_int Key)
{
	if (nullptr == m_pKey_Manager)
		return false;

	return m_pKey_Manager->Key_Up(Key);
}

_bool CGameInstance::Key_Down(_int Key)
{
	if (nullptr == m_pKey_Manager)
		return false;

	return m_pKey_Manager->Key_Down(Key);
}

_bool CGameInstance::Key_Presing(_int Key)
{
	if (nullptr == m_pKey_Manager)
		return false;

	return m_pKey_Manager->Key_Presing(Key);
}

HRESULT CGameInstance::Set_Key_Check(_int InputKey, CModel * pModel)
{
	if (nullptr == m_pAnimation_Manager)
		return E_FAIL;

	return m_pAnimation_Manager->Set_Key_Check(InputKey,pModel);
}

HRESULT CGameInstance::Update_Player_Key_Contral(CModel * pModel, _double TimeDelta)
{
	if (nullptr == m_pAnimation_Manager)
		return E_FAIL;

	return m_pAnimation_Manager->Update_Player_Key_Contral(pModel,TimeDelta);
}

ID3D11ShaderResourceView * CGameInstance::Get_ShaderResourceView(const _tchar * pTargetTag)
{
	if (nullptr == m_pTarget_Manager)
		return nullptr;

	return m_pTarget_Manager->Get_ShaderResourceView(pTargetTag);
}

HRESULT CGameInstance::Add_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const _tchar * pFontTag, const _tchar * pFontFilePath)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Add_Font(pDevice,pDeviceContext,pFontTag,pFontFilePath);
}

HRESULT CGameInstance::Draw_Font(const _tchar * pFontTag, const _tchar * pText, _float2 vPosition, _fvector vColor, _float fScale)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Draw_Font(pFontTag, pText, vPosition, vColor, fScale);
}

_bool CGameInstance::isIn_WorldSpace(_fvector vWorldPoint, _float fRange)
{
	if (nullptr == m_pFrustum)
		return false;

	return m_pFrustum->isIn_WorldSpace(vWorldPoint, fRange);
}

void CGameInstance::Release_Engine()
{

	if (0 != CGameInstance::Get_Instance()->Destroy_Instance())
		return;

	if (0 != CLevel_Manager::Get_Instance()->Destroy_Instance())
		return;

	if (0 != CComponent_Manager::Get_Instance()->Destroy_Instance())
		return;

	if (0 != CGameObject_Manager::Get_Instance()->Destroy_Instance())
		return;

	if (0 != CLoad_Manager::Get_Instance()->Destroy_Instance())
		return;

	if (0 != CAnimation_Control_Manager::Get_Instance()->Destroy_Instance())
		return;

	if (0 != CRenderTarget_Manager::Get_Instance()->Destroy_Instance())
		return;

	if (0 != CPicking::Get_Instance()->Destroy_Instance())
		return;

	if (0 != CPipeLine::Get_Instance()->Destroy_Instance())
		return;

	if (0 != CLight_Manager::Get_Instance()->Destroy_Instance())
		return;

	if (0 != CFrustum::Get_Instance()->Destroy_Instance())
		return;

	if (0 != CKey_Manager::Get_Instance()->Destroy_Instance())
		return;

	if (0 != CTime_Manager::Get_Instance()->Destroy_Instance())
		return;

	if (0 != CInput_Device::Get_Instance()->Destroy_Instance())
		return;

	if (0 != CGraphic_Device::Get_Instance()->Destroy_Instance())
		return;
}

void CGameInstance::Free()
{
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pPicking);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pTime_Manager);
	Safe_Release(m_pAnimation_Manager);
	Safe_Release(m_pKey_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pLoad_Manager);
	Safe_Release(m_pGameObject_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pGraphic_Device);
}
