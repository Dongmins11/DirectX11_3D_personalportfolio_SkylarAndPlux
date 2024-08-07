#pragma once

#include "Base.h"


BEGIN(Engine)
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(ID3D11Device* pDevice,ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() =default;
public:
	class CComponent*	Get_Components(const _tchar* pComponentTag,_uint iLayerIndex = 0);
public:
	virtual HRESULT Native_Construct_Prototype();
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex);
	virtual _int	Tick(_double TimeDelta);
	virtual _int	LateTick(_double TimeDelta);
	virtual HRESULT Render();
public:
	_float	Get_ViewZ() { return m_fViewZ; }
public:
	void	Set_DieLayer(_uint iDeadNumber) { m_iDeadNumber = iDeadNumber; }
public:
	HRESULT Release_Component();
	HRESULT Relese_Layer_Component(const _tchar* pLayerTag,_uint iIndex);
public:
	void	Set_Collision_Check(_bool bCheck) { m_bIsColision = bCheck; }
	_bool	Get_Collision_Check() { return m_bIsColision; }
public:
	void	Set_WolrdMatrix(_fmatrix WolrdMatrix) { XMStoreFloat4x4(&m_WolrdMatrix, WolrdMatrix); }
	void	Set_MyLevel(_uint iLevel) { m_iMyLevel = iLevel; }
public:
	_float4x4 Get_WolrdMatrix4x4() { return m_WolrdMatrix; }
	_matrix	Get_WolrdMatrix() { return XMLoadFloat4x4(&m_WolrdMatrix); }
	_uint	Get_MyLevel() { return m_iMyLevel; }
protected:
	void	Compute_ViewZ(class CTransform* pTransform);
protected:
	virtual HRESULT SetUp_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentsTag, CComponent** ppComponentOut, void* pArg =nullptr);
	virtual _int Get_ComponentSize(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentsTag);
protected:
	class CObject_Component_Manager*	Finder_Components(const _tchar* pComponentsTag);

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	map<const _tchar*,class CObject_Component_Manager*>		m_mapComponents;
protected:
	MODELDESC*				m_pModelDesc = nullptr;
	MODELDESC_CLIENT*		m_pModelDesc_Client = nullptr;
	_bool					m_bIsColision = false;
	_bool					m_bIsClone = false;
	_uint					m_iMyLevel = 0;
protected:
	_uint					m_iDeadNumber = 0;
	_float4x4				m_WolrdMatrix;
protected:
	_bool					m_bCollisionShutDownCheck = false;
protected:
	_float					m_fViewZ = 0.f;
public:
	virtual CGameObject* Clone(void* pArg,_uint	iMyLevelIndex)PURE;
	virtual void Free() override;
};

END