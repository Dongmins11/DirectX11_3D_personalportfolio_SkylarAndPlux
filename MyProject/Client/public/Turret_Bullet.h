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

class CTurret_Bullet final : public CGameObject
{
private:
	explicit CTurret_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CTurret_Bullet(const CTurret_Bullet& rhs);
	virtual ~CTurret_Bullet() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	HRESULT Effect_Create(_double TimeDelta);
	HRESULT HitEffect_Create(_double TimeDelta);
private:
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CColider*				m_pAABB = nullptr;
	CColider*				m_pSphere = nullptr;
private:
	_double					m_fAgeTime = 0.0;
private:
	_double					m_dCreateTimeAcc = 0.0;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();


public:
	static CTurret_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};
END

