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
class CEffect_FireCrt final : public CGameObject
{
private:
	explicit CEffect_FireCrt(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_FireCrt(const CEffect_FireCrt& rhs);
	virtual ~CEffect_FireCrt() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_RenderCheck(_bool bCheck) { m_bEffectRenderCheck = bCheck; }
	void	Set_DeadCheck(_bool bCheck) { m_bDeadCheck = bCheck; }
private:
	void	BillBord_Object();
	_int Effect_Change(_double TimeDelta);
	_int Effect_ChangeTwo(_double TimeDelta);
	HRESULT	Move_Effect(_double TimeDelta);
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CVIBuffer_Rect*	m_pModelCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
private:
	CVIBuffer_Rect*	m_pLeftUpBuffer = nullptr;
	CVIBuffer_Rect*	m_pRightUpBuffer = nullptr;
	CVIBuffer_Rect*	m_pLeftDownBuffer = nullptr;
	CVIBuffer_Rect*	m_pRightDownBuffer = nullptr;
private:
	CTransform*		m_pLeftUpTransformCom = nullptr;
	CTransform*		m_pLeftDownTransformCom = nullptr;
	CTransform*		m_pRightUpTransformCom = nullptr;
	CTransform*		m_pRightDownTransformCom = nullptr;
private:
	_double			m_dTimeAccFilp = 0.0;
	_float			m_fU = 0.f;
	_float			m_fV = 0.f;
private:
	_float			m_fScale = 0.f;
private:
	_bool			m_bDeadCheck = false;
	_bool			m_bEffectRenderCheck = false;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT SetUp_Component_Buffer();
	HRESULT SetUp_ConstantTable_Buffer();
public:
	static	CEffect_FireCrt* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END