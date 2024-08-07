#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CVIBuffer_Rect;
class CColider;
class CTexture;
END


BEGIN(Client)
class CEffect_SwingAttack final : public CGameObject
{
private:
	explicit CEffect_SwingAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_SwingAttack(const CEffect_SwingAttack& rhs);
	virtual ~CEffect_SwingAttack() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	HRESULT	Move_Effect(_double TimeDelta);
	HRESULT Effect_Create(_double TimeDelta);
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CVIBuffer_Rect*	m_pModelCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
private:
	_double			m_dTimeAccFilp = 0.0;
	_float			m_fU = 0.f;
	_float			m_fV = 0.f;
private:
	_float			m_fRotY = 0.f;
private:
	_double			m_dTimeAccDead = 0.f;
	_float			m_fGlowValue = 0.f;
	_float			m_fScale = 0.f;
	_float			m_fAlphaColor = 0.f;
private:
	_double			m_dCreateTimeAcc = 0.f;
	_bool			m_bCreateCheck = false;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CEffect_SwingAttack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END