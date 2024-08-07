#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CModel;
class CColider;
END


BEGIN(Client)
class CBossPotal final : public CGameObject
{
private:
	explicit CBossPotal(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBossPotal(const CBossPotal& rhs);
	virtual ~CBossPotal() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	void	Collision_Check();
private:
	HRESULT Collision_Check_Potal();
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CColider*		m_pSphere = nullptr;
	CColider*		m_pAABB = nullptr;
private:
	_bool			m_SetVector = false;
	_bool			m_bRenderCheck = false;
private:
	_float			m_fTimeDelta = 0.f;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CBossPotal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END