#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Rock.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CVIBuffer_Rect;
class CColider;
class CTexture;
END


BEGIN(Client)
class CEffect_Flip final : public CGameObject
{
private:
	explicit CEffect_Flip(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_Flip(const CEffect_Flip& rhs);
	virtual ~CEffect_Flip() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_Position(_fvector vPos);
private:
	HRESULT	Change_UV(_double TimeDelta);
private:
	HRESULT Set_VecCollider();
	void	Collision_Check();
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CColider*		m_pSphere = nullptr;
	CVIBuffer_Rect*	m_pVIBuffer = nullptr;
	CTexture*		m_pTextureCom = nullptr;
private:
	_double			m_dTimeAcc = 0.0;
	_float			m_fU = 0.f;
	_float			m_fV = 0.f;
	_uint			m_iIndex = 0;
private:
	vector<CColider*>	m_vecTargetCollider;
private:
	_bool			m_SetVector = false;
	_bool			m_bRenderCheck = false;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CEffect_Flip* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END