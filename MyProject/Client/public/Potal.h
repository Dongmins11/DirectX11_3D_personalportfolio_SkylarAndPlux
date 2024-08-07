#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CModel;
class CColider;
class CTexture;
END


BEGIN(Client)
class CPotal final : public CGameObject
{
private:
	explicit CPotal(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CPotal(const CPotal& rhs);
	virtual ~CPotal() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	HRESULT Set_VecCollider();
	void	Collision_Check(_double TimeDelta);
private:
	HRESULT Collision_Check_Potal();
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CColider*		m_pSphere = nullptr;
	CColider*		m_pAABB = nullptr;
	CTexture*		m_pDissolveTextrue = nullptr;
private:
	vector<CColider*>	m_vecTargetCollider;
private:
	_bool			m_SetVector = false;
	_bool			m_bRenderCheck = false;
private:
	_float			m_fTimeDelta = 0.f;
private:
	_float			m_fRatio = 0.f;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CPotal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;

};

END