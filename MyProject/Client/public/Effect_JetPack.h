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
class CEffect_JetPack final : public CGameObject
{
private:
	explicit CEffect_JetPack(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_JetPack(const CEffect_JetPack& rhs);
	virtual ~CEffect_JetPack() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_DeadCheck(_bool bCheck) { m_bDeadCheck = bCheck; }
private:
	HRESULT	Move_Effect(_double TimeDelta);
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
	_double			m_dVelocity = 0.0;
private:
	_float			m_fScale = 0.f;
	_float			m_fBackUpScale = 0.f;
private:
	_bool			m_bOneCheck = false;
	_bool			m_bTwoCheck = false;
	_bool			m_bThreeCheck = false;
	_bool			m_bForCheck = false;
	_bool			m_bFiveCheck = false;
private:
	_bool			m_bDeadCheck = false;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CEffect_JetPack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END