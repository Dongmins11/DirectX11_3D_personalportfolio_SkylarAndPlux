#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CModel;
class CTransform;
class CColider;
class CVIBuffer_Rect;
END

BEGIN(Client)

#define NPC_CRY		0
#define	NPC_SKY		1
#define NPC_IDLE	2
#define NPC_SKYLAND 3
#define NPC_TALK	4

class CTalkNpc final : public CGameObject
{
private:
	explicit CTalkNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CTalkNpc(const CTalkNpc& rhs);
	virtual ~CTalkNpc() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void Set_CageCheck(_bool bCageCheck) { m_bAnimCageCheck = bCageCheck; }
	void	Set_DeadCheck(_bool bCheck) { m_bDeadCheck = bCheck; }
private:
	HRESULT Talk_Start(_double TimeDelta);
	HRESULT Collision_Check();
private:
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CColider*				m_pSphere = nullptr;
private:
	CTransform*				m_pChatTransform = nullptr;
	CVIBuffer_Rect*			m_pChatBuffer = nullptr;
	CTexture*				m_pChatTexture = nullptr;
	CGameObject*			m_pTalkUi = nullptr;
private:
	_bool					m_bAnimStart = false;
	_uint					m_iChatIndex = 0;
private:
	_uint					m_iCurMotion = 0;
	_bool					m_bIsAnimEndCheck = false;
	_bool					m_bAnimCageCheck = false;
	_bool					m_bRenderCheck = false;
private:
	_double					m_dDeadTime = 0.0;
	_bool					m_bDeadCheck = false;
	_bool					m_bEffectCheck = false;
private:
	_bool					m_bCryCheck = false;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();


public:
	static CTalkNpc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};
END

