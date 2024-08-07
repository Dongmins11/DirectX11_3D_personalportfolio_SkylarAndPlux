#pragma once

#include "Base.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Time_Manager.h"
#include "Level_Manager.h"
#include "GameObject_Manager.h"
#include "Key_Manager.h"
#include "Component_Manager.h"
#include "Load_Manager.h"
#include "PipeLine.h"
#include "Light_Manager.h"
#include "Colider.h"
#include "Picking.h"
#include "Animation_Control_Manager.h"
#include "RenderTarget_Manager.h"
#include "Font_Manager.h"
#include "Frustum.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final :public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() =default;
public:
	HRESULT Initalize_Engine(_uint iNumLevel,HINSTANCE hInst, const CGraphic_Device::DEVICEDESC& tDeivce_Desc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	HRESULT Initalize_Engine_Tool(_uint iNumLevel, HINSTANCE hInst, const CGraphic_Device::DEVICEDESC& tDeivce_Desc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	_int	Tick_Engine(_double TimeDelta);
	HRESULT Render_Engine();
	HRESULT Clear_LevelResource(_uint iLevelIndex);
public:/*For.Graphic_Device*/
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_Depth_Stencil_View();
	HRESULT Present();

public:/*For.Input_Device*/
	_byte Get_DIKeyState(_ubyte eKeyID);
	_long Get_DIMouseMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveState);
	_byte Get_DIMouseButtonState(CInput_Device::MOUSEBUTTONSTATE eMouseButtonState);

public:/*For.Time_Manager*/
	HRESULT Add_Timer(const _tchar* pTimerTag);
	_double Compute_TimeDleta(const _tchar* pTimerTag);

public:/*For.Level_Manager*/
	HRESULT	Open_Level(_uint iCurrurntLevelIndex, CLevel* pNewLevel);

public:/*For.GameObject_Manager*/
	CComponent*		Get_Components(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex,_uint iLayerIndex = 0);
	CGameObject*	Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag);
	HRESULT			Release_GameObject(_uint iLevelIndex, const _tchar* pLayerTag);
	HRESULT			Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CGameObject* pGameObject);
	HRESULT			Add_ObjectToLayer(_uint iLevelIndex, const _tchar* pPrototypeTag, _uint iLayerLevelIndex, const _tchar* pLayerTag, void* pArg = nullptr, CGameObject** ppOutGameObject = nullptr);
	_uint			Get_GameObjectSize(_uint iLevelIndex, const _tchar* pLayerTag);

public:/*For.Component_Manager*/
	HRESULT			Add_Prototype(_uint iNumLevelIndex, const _tchar* pPrototypeTag, CComponent* pComponents);
	CComponent*		Clone_Component(_uint iNumLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);
public:/*For.PipeLine*/
	void	Set_HoldingMatrix(CPipeLine::TRANSFORMSTATETYPE eState, _fmatrix HoldingMatrix);
	_matrix Get_HoldingMatrix(CPipeLine::TRANSFORMSTATETYPE eState);
	_vector Get_CamPosition();

public:/*For.Light_Manager*/
	HRESULT Set_LightDesc(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& tLightDesc);
	LIGHTDESC*	Get_LighDesc(_uint iIndex);
	void	Set_LightRenderCheck(_uint iIndex, _bool bCheck);
	HRESULT	Clear_Light();
public:/*For. Picking*/
	void Transform_ToWorldSapce();
	_bool Picking_To_Terrain(CTransform* pTransform, class CVIBuffer* pVIBuffer, _float3* vOut);
	_bool Picking_To_Model(CTransform* pTransform, class CVIBuffer* pVIBuffer, _float3* vOut, _float* fOutDist);
	_bool Picking_To_Colider(CTransform* pTransform, class CVIBuffer* pVIBuffer, _float3* vOut, _float* fOutDist);

public:/*For. Load_Manager*/
	HRESULT	Load_PathFinder_To_Model(const _tchar* tPath, _uint iLevel);
	HRESULT Load_GameObject_Tool(const _tchar* pFilePath, const _tchar* pMapPrototypeTag, _uint iLevel);

public:/*For. Key_Manager*/
	_bool	Key_Up(_int	Key);
	_bool	Key_Down(_int Key);
	_bool	Key_Presing(_int Key);

public:/*For. Animation_Contrial*/
	HRESULT		Set_Key_Check(_int InputKey, CModel* pModel);
	HRESULT		Update_Player_Key_Contral(CModel* pModel, _double TimeDelta);
public:/*For.RenderTarget*/
	ID3D11ShaderResourceView* Get_ShaderResourceView(const _tchar* pTargetTag);

public: /* For.Font_Manager */
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pFontTag, const _tchar* pFontFilePath);
	HRESULT Draw_Font(const _tchar* pFontTag, const _tchar* pText, _float2 vPosition, _fvector vColor,_float fScale);
public: /* For.Frustum */
	_bool isIn_WorldSpace(_fvector vWorldPoint, _float fRange = 0.f);

private:
	CAnimation_Control_Manager*	m_pAnimation_Manager = nullptr;
	CLight_Manager*			m_pLight_Manager = nullptr;
	CPipeLine*				m_pPipeLine = nullptr;
	CComponent_Manager*		m_pComponent_Manager = nullptr;
	CGameObject_Manager*	m_pGameObject_Manager = nullptr;
	CLevel_Manager*			m_pLevel_Manager = nullptr;
	CGraphic_Device*		m_pGraphic_Device = nullptr;
	CInput_Device*			m_pInput_Device = nullptr;
	CTime_Manager*			m_pTime_Manager = nullptr;
	CColider*				m_pColider = nullptr;
	CPicking*				m_pPicking = nullptr;
	CLoad_Manager*			m_pLoad_Manager = nullptr;
	CKey_Manager*			m_pKey_Manager = nullptr;
	CRenderTarget_Manager*	m_pTarget_Manager = nullptr;
	CFont_Manager*			m_pFont_Manager = nullptr;
	CFrustum*				m_pFrustum = nullptr;
public:
	static void Release_Engine();
	virtual void Free() override;
};

END

