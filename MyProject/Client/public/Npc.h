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

class CNpc final : public CGameObject
{
private:
	explicit CNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CNpc(const CNpc& rhs);
	virtual ~CNpc() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void Set_CageCheck(_bool bCageCheck) { m_bAnimCageCheck = bCageCheck;}
private:
	HRESULT Animation_Change();
private:
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CColider*				m_pSphere = nullptr;
private:
	_uint					m_iCurMotion = 0;
	_bool					m_bIsAnimEndCheck = false;
	_bool					m_bAnimCageCheck = false;
	_bool					m_bRenderCheck = false;
private:
	_bool					m_bSoundCheck = false;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();


public:
	static CNpc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};
END

