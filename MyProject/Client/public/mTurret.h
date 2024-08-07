#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CModel;
class CNavigation;
class CTransform;
class CColider;
END

BEGIN(Client)

class CmTerret final : public CGameObject
{
private:
	CmTerret(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CmTerret(const CmTerret& rhs);
	virtual ~CmTerret() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	HRESULT	Trace_Player(_double TimeDelta);
	HRESULT	Hit_Monster(_double TimeDelta);

private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
	CColider*				m_pAABB = nullptr;
	CColider*				m_pSphere = nullptr;
	CColider*				m_pHitSphere = nullptr;
private:
	_uint					m_iCurrntIndex = 0;
	_uint					m_iCurMotion = 0;
	_double					m_dTimeAcc = 0.0;
	_double					m_dCollisionTimeAcc = 0.0;
	_bool					m_bCheckCollision = false;
private:
	_float3					m_vBackDirection;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();



public:
	static CmTerret* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END