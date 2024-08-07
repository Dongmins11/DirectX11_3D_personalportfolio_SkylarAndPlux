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
class CEffect_Roket final : public CGameObject
{
private:
	explicit CEffect_Roket(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_Roket(const CEffect_Roket& rhs);
	virtual ~CEffect_Roket() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	void	BillBord_Object();
	_int Effect_Change(_double TimeDelta);
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
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CEffect_Roket* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END