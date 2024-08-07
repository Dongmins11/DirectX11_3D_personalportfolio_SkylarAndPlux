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
class CTexture;
END

BEGIN(Client)

class CPunchRock final : public CGameObject
{
private:
	CPunchRock(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CPunchRock(const CPunchRock& rhs);
	virtual ~CPunchRock() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	HRESULT	Hit_Monster(_double TimeDelta);
	HRESULT Hit_Motion(_double TimeDelta);

private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
	CColider*				m_pAABB = nullptr;
	CColider*				m_pSphere = nullptr;
	CColider*				m_pHitSphere = nullptr;
	CTexture*				m_pTexture = nullptr;
private:
	_uint					m_iCurrntIndex = 0;
	_uint					m_iCurMotion = 0;
	_double					m_dTimeAcc = 0.0;
	_double					m_dCollisionTimeAcc = 0.0;
	_bool					m_bCheckCollision = false;
private:
	_double					m_vTimeAcc = 0.0;
	_float3					m_vBackDirection;
private:
	_float					m_vStartValue = 0.f;
	_float					m_vEndValue = 0.f;
	_float					m_vBlendValue = 0.f;
	_uint					m_iScaleIndex = 0;
	_double					m_dBlendTime = 0.f;
	_bool					m_bMotionCheck = false;
private:
	_uint					m_iHitIndex = 0;
private:
	_float					m_fRatio = 0.f;
	_bool					m_bDissolveCheck = false;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();



public:
	static CPunchRock* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END