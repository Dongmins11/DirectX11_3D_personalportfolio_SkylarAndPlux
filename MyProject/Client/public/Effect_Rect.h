#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBuffer_RectInstance;
class CColider;
END

BEGIN(Client)

class CEffect_Rect final : public CGameObject
{
private:
	CEffect_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEffect_Rect(const CEffect_Rect& rhs);
	virtual ~CEffect_Rect() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_DeadCheck(_bool Check) { m_bDeadCheck = Check; }

private:
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_RectInstance*	m_pVIBufferCom = nullptr;
	CColider*				m_pSphere = nullptr;
private:
	CVIBuffer_RectInstance::RECTINST	m_tRectInst;
private:
	_bool					m_bDeadCheck = false;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();


public:
	static CEffect_Rect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END