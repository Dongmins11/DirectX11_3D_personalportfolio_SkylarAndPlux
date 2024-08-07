#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CModel;
class CTransform;
END

BEGIN(Tool)

class CRenderObject final : public CGameObject
{
public:

private:
	CRenderObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CRenderObject(const CRenderObject& rhs);
	virtual ~CRenderObject() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	HRESULT Set_Pos();
	HRESULT Set_Scaile();
	HRESULT Set_ScaileXYZ();
	HRESULT Set_Rotation();
	HRESULT	Set_ModelDesc(MODELDESC* pModelDesc);

private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
private:
	_bool					m_bIsClone = false;

private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();


public:
	static CRenderObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END