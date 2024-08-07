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
class CGravitionalField final : public CGameObject
{
private:
	explicit CGravitionalField(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGravitionalField(const CGravitionalField& rhs);
	virtual ~CGravitionalField() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_RenderCheck(_bool RenderCheck) { m_bRenderCheck = RenderCheck; }
	_bool	Get_RenderCheck() { return m_bRenderCheck; }
private:
	HRESULT Gravity_Coord(_double TimeDelta);
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CColider*		m_pSphere = nullptr;
	CColider*		m_pAABB = nullptr;
private:
	_bool			m_bReadyCheck = false;
private:
	_float			m_fTimer = 0.f;
private:
	_bool			m_bRenderCheck = false;
	_bool			m_bFirstCheck = false;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CGravitionalField* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END