#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CModel;
class CTransform;
class CColider;
END

BEGIN(Client)

#define NPC_CRY		0
#define	NPC_SKY		1
#define NPC_IDLE	2
#define NPC_SKYLAND 3
#define NPC_TALK	4

class CBossPet final : public CGameObject
{
private:
	explicit CBossPet(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBossPet(const CBossPet& rhs);
	virtual ~CBossPet() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	HRESULT Boss_Trace(_double TimeDelta);
	HRESULT Lerp_Y(_double TimeDelta);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
private:
	_uint					m_iCurMotion = 0;
	_bool					m_bIsAnimEndCheck = false;
	_bool					m_bAnimCageCheck = false;
	_bool					m_bRenderCheck = false;
private:
	_double					m_dPlayer_BlnederTime = 0.0;
	_double					m_dPlayer_ReadyTime = 0.0;
private:
	CTransform*				m_pTargetTransform = nullptr;
private:
	const _float4x4*		m_pPlayerMatrix = nullptr;
	_vector					m_Distance;
private:
	_bool					m_YCheck = false;
	_float					m_fDisY = 0.f;
private:
	_bool					m_bEffectCheck = false;
	_double					m_dLookTimeDelta = 0.0;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();


public:
	static CBossPet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;
};
END

