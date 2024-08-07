#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Rock.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CModel;
class CColider;
END


BEGIN(Client)
class CScaffolding final : public CGameObject
{
private:
	explicit CScaffolding(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CScaffolding(const CScaffolding& rhs);
	virtual ~CScaffolding() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	HRESULT Collision_Check();
	HRESULT My_Collision_Check();
	void	ColorChange(_double TimeDelta);
	void	DownChange(_double TimeDelta);
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CModel*			m_pSpinnigModelCom = nullptr;
	CColider*		m_pAABB = nullptr;
private:
	_float4			m_vColor;
	CRock::PLAYERCOLLISIONDESC	m_tPlayerCollisionDesc;
private:
	_bool			m_dDownCheck = false;
	_bool			m_bTransCheck = false;
	_bool			m_bTransUpCheck = false;
	_bool			m_bGreenColor = false;
	_bool			m_bPlayer_CollisionCheck = false;
	_double			m_dColorTimeAcc = 0.0;
	_double			m_dDownTimeAcc = 0.0;
	_float4			m_vDsetBackupPos;
	_float4			m_vSourceBackupPos;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CScaffolding* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;

};

END