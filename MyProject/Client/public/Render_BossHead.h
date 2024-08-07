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
class CRender_BossHead final : public CGameObject
{
private:
	explicit CRender_BossHead(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CRender_BossHead(const CRender_BossHead& rhs);
	virtual ~CRender_BossHead() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_AnimCheck(_bool bCheck) { m_bAnimCheck = bCheck; }
private:
	HRESULT Animation_Boss(_double TimeDelta);
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CColider*		m_pSphere = nullptr;
private:
	_double			m_dTimeAcc = 0.0;
	_bool			m_bAnimCheck = false;
	_bool			m_bFristCheck = false;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CRender_BossHead* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END