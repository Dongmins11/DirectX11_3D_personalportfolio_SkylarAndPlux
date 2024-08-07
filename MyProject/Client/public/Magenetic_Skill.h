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
class CMagenetic_Skill final : public CGameObject
{
private:
	explicit CMagenetic_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMagenetic_Skill(const CMagenetic_Skill& rhs);
	virtual ~CMagenetic_Skill() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	_bool	Get_RenderCheck() { return m_bRenderCheck; }
	void	Set_RenderCheck(_bool RenderCheck) { m_bRenderCheck = RenderCheck; }
public:
	void	Set_KeyCheck(_bool bKeyCheck) { m_bKeyCheck = bKeyCheck; }
	void	Set_Position();
public:/*For Bullet*/
	void	Set_Magenetic_Bullet(_bool bCheck) { m_bBulletCheck = bCheck; }
	_bool	Get_Magenetic_Bullet_Check() { return m_bBulletCheck; }
private:
	HRESULT Gravitional_Check(_double TimeDelta);
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CTransform*		m_pTargetTransformCom = nullptr;
private:
	CColider*		m_pSphere = nullptr;
	CColider*		m_pAABB = nullptr;
private:
	_bool			m_bScaleCheck = false;
	_bool			m_bKeyCheck = false;
	_double			m_dTimeAcc = 0.0;
private:
	_bool			m_bBulletCheck = false;
private:
	_bool			m_bRenderCheck = false;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CMagenetic_Skill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END