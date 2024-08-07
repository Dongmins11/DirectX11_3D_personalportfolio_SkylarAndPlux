#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Rock.h"
BEGIN(Engine)
class CTransform;
class CRenderer;
class CModel;
class Colider;
END


BEGIN(Client)
class CChangeFloorFor final : public CGameObject
{
private:
	explicit CChangeFloorFor(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CChangeFloorFor(const CChangeFloorFor& rhs);
	virtual ~CChangeFloorFor() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	HRESULT	Change_Scale(_double TimeDelta);
	HRESULT Change_Model();
private:
	HRESULT Collision_Check();
private:
	_float3			m_vDestScale;
	_float3			m_vSourceScale;
	_double			m_dTimeAcc = 0.0;
	_double			m_dChangeTimeAcc = 0.0;
	_bool			m_bIsChange = false;
	_bool			m_bIsNonChange = false;
	_bool			m_bRenderChange = false;
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	Colider*		m_pColiderCom = nullptr;
	CColider*		m_pAABB = nullptr;
private:
	CRock::PLAYERCOLLISIONDESC	m_tPlayerCollisionDesc;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CChangeFloorFor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END