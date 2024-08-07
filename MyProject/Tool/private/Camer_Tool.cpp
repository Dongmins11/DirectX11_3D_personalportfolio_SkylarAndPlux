#include "stdafx.h"
#include "..\public\Camer_Tool.h"
#include "GameInstance.h"

CCamer_Tool::CCamer_Tool(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CCamera(pDevice, pDeviceContext)
{
}

CCamer_Tool::CCamer_Tool(const CCamer_Tool & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamer_Tool::Native_Construct_Prototype()
{
	if (FAILED(__super::Native_Construct_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CCamer_Tool::Native_Construct(void* pArg, _uint iMyLevelIndex)
{
	if (FAILED(__super::Native_Construct(pArg,iMyLevelIndex)))
		return E_FAIL;

	return S_OK;
}

_int CCamer_Tool::Tick(_double TimeDelta)
{

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
	{
		m_pTransform->Go_Straight(TimeDelta * 0.09);
	}
	if (pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
	{
		m_pTransform->Go_Backward(TimeDelta * 0.09);
	}

	if (pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
	{
		m_pTransform->Go_Left(TimeDelta * 0.09);
	}
	if (pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
	{
		m_pTransform->Go_Right(TimeDelta * 0.09);
	}
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	_long		MouseMove = 0;

	if (pGameInstance->Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE::MBS_RBUTTON) & 0x80)
	{
		if ((MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE::MMS_X)) && 0 < pt.x&&g_iWinCX > pt.x && 0 < pt.y&&g_iWinCY > pt.y)
			m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * MouseMove * 0.009f);
		if ((MouseMove = pGameInstance->Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE::MMS_Y)) && 0 < pt.x&&g_iWinCX > pt.x && 0 < pt.y&&g_iWinCY > pt.y)
			m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), TimeDelta * MouseMove * 0.009f);
	}

	RELEASE_INSTANCE(CGameInstance);


	return __super::Tick(TimeDelta);
}

_int CCamer_Tool::LateTick(_double TimeDelta)
{
	return _int();
}

HRESULT CCamer_Tool::Render()
{
	return S_OK;
}

CCamer_Tool * CCamer_Tool::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CCamer_Tool* pInstance = new CCamer_Tool(pDevice, pDeviceContext);

	if (pInstance->NativeConstruct_Prototype())
	{
		MSGBOX("Failed to Create CameraFly");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamer_Tool::Clone(void* pArg, _uint iMyLevelIndex)
{
	CCamer_Tool* pInstance = new CCamer_Tool(*this);

	if (pInstance->Native_Construct(pArg,iMyLevelIndex))
	{
		MSGBOX("Failed to Create CameraFly");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamer_Tool::Free()
{
	__super::Free();
}
