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

class CLeft_Bullet final : public CGameObject
{
private:
	explicit CLeft_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CLeft_Bullet(const CLeft_Bullet& rhs);
	virtual ~CLeft_Bullet() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	HRESULT HitEffect_Create(_double TimeDelta);
	HRESULT Effect_Create(_double TimeDelta);
private:
	HRESULT Floor_CollisionCheck(_double TimeDelta);
	HRESULT Magenetic_IsIn(_double TimeDelta);
	_int Boss_CollisionCheck(_double TimeDelta);
private:
	HRESULT Go_Direction(_double TimeDelta);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CColider*				m_pAABB = nullptr;
	CColider*				m_pSphere = nullptr;
private:
	_double					m_fAgeTime = 0.0;
private:
	_bool					m_bDeadCheck = false;
private:
	_bool					m_bBossLookBackUpCheck = false;
	_bool					m_bLerpCheck = false;
	_bool					m_bGoCheck = false;
	_bool					m_bMageneticCheck = false;
	_bool					m_bDirGoCheck = false;
	_float3					m_vBossDir;
private:
	_double					m_dCreateTimeAcc = 0.0;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();


public:
	static CLeft_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;
};
END

