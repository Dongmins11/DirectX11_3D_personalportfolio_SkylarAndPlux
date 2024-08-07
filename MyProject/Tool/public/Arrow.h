#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
class CTransform;
END

BEGIN(Tool)

class CArrow final : public CGameObject
{
public:

private:
	CArrow(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CArrow(const CArrow& rhs);
	virtual ~CArrow() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	HRESULT Set_Pos(_float4 vPos);
private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
private:
	_float3					m_vRotation;
	_float					m_fRotY = 0.f;
	_bool					m_bIsClone = false;

private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static CArrow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END