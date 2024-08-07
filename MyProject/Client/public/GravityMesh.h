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
class CGravityMesh final : public CGameObject
{
private:
	explicit CGravityMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGravityMesh(const CGravityMesh& rhs);
	virtual ~CGravityMesh() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_MoveCheck(_bool bCheck) { m_bMoveCheck = bCheck; }
private:
	HRESULT	Mesh_Move(_double TimeDelta);
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CColider*		m_pSphere = nullptr;
	CColider*		m_pAABB = nullptr;
private:
	CGameObject*	m_pGravityField = nullptr;
	_double			m_dTimeAcc = 0.0;
	_bool			m_bMoveCheck = false;
	_bool			m_bSuceesce = false;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CGravityMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END