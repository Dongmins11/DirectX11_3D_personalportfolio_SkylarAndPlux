#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBuffer_PointInstance;
class CColider;
END

BEGIN(Client)

class CEffect_Point final : public CGameObject
{
private:
	CEffect_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_Point(const CEffect_Point& rhs);
	virtual ~CEffect_Point() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	_int Filp_Anim(_double TimeDelta);
	void	Set_DeadCheck(_bool Check) { m_bDeadCheck = Check; }

private:
	CTransform*				m_pTransformCom = nullptr;
	CTransform*				m_pLeftTransformCom = nullptr;
	CTransform*				m_pRightTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_PointInstance*	m_pVIBufferCom = nullptr;
	CColider*				m_pSphere = nullptr;
private:
	_bool					m_bFirstCheck = false;
private:
	CTransform*				m_pRightTargetTransform = nullptr;
private:
	CVIBuffer_PointInstance::POINTINST	m_tPointInst;
private:
	_double					m_dScale = 0.0;
private:
	_bool					m_bDeadCheck = false;
private:
	_double					m_dTimeAcc = 0.0;
private:
	_float					m_fTimeAcc = 0.f;
private:
	_double			m_dTimeAccFilp = 0.0;
	_float			m_fU = 0.f;
	_float			m_fV = 0.f;
	_uint			m_iIndex = 0;
	_float			m_fGlowStength = 0.f;
private:
	_float4			m_vRectRandom;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();


public:
	static CEffect_Point* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END