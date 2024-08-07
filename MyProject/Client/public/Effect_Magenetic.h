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
class CEffect_Magenetic final : public CGameObject
{
public:
	typedef struct tagViewZ
	{
		_float	fSize;
		CVIBuffer_Rect* Rect;
		CTransform* pTransform;
	}VIEWDESC;
private:
	explicit CEffect_Magenetic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_Magenetic(const CEffect_Magenetic& rhs);
	virtual ~CEffect_Magenetic() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	_bool	Get_RenderCheck() { return m_bRenderCheck; }
	void	Set_RenderCheck(_bool bCheck) { m_bRenderCheck = bCheck; }
private:
	HRESULT	Move_Effect(_double TimeDelta);
	HRESULT	Player_PosSet(_double TimeDelta);
private:
	HRESULT Compute_Z(_double TimeDelta);
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
private:
	CTransform*		m_pTransformCom = nullptr;
	CTransform*		m_pTwoTransformCom = nullptr;
	CTransform*		m_pThreeTransformCom = nullptr;
	CTransform*		m_pForTransformCom = nullptr;
private:
	CVIBuffer_Rect*	m_pModelCom = nullptr;
	CVIBuffer_Rect*	m_pTwoModelCom = nullptr;
	CVIBuffer_Rect*	m_pThreeModelCom = nullptr;
	CVIBuffer_Rect*	m_pForModelCom = nullptr;
private:
	_bool			m_bRenderCheck = false;
private:
	_float			m_fStrength = 0.f;
private:
	list<VIEWDESC>	m_ListDesc;
private:
	VIEWDESC			m_fOneViewZ;
	VIEWDESC			m_fTwoViewZ;
	VIEWDESC			m_fThreeViewZ;
	VIEWDESC			m_fForViewZ;
private:
	_double			m_dTimeAccFilp = 0.0;
	_float			m_fU = 0.f;
	_float			m_fV = 0.f;
private:
	_float			m_fScale = 0.f;
	_float			m_fTwoScale = 0.f;
	_float			m_fThreeScale = 0.f;
	_float			m_fForScale = 0.f;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CEffect_Magenetic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END