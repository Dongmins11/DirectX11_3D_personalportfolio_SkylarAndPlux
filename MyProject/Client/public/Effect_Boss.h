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
class CEffect_Boss final : public CGameObject
{
private:
	explicit CEffect_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_Boss(const CEffect_Boss& rhs);
	virtual ~CEffect_Boss() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	HRESULT Scale_Up(_double TimeDelta);
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CVIBuffer_Rect*	m_pModelCom = nullptr;
	CColider*		m_pSphere = nullptr;
	CTexture*		m_pTextureCom = nullptr;
private:
	_float			m_fScaleing = 0.f;
	_double			m_dScaleTimeAcc = 0.0;
	_bool			m_bScaleCheck = false;
private:
	_float			m_fGlowStength = 0.f;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CEffect_Boss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END