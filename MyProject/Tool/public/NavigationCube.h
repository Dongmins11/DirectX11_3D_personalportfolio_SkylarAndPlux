#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Cube;
class CTransform;
END

BEGIN(Tool)

class CNavigationCube final : public CGameObject
{
public:

private:
	CNavigationCube(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CNavigationCube(const CNavigationCube& rhs);
	virtual ~CNavigationCube() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	HRESULT Set_Pos(_float4 vPos);
	void	Set_ColorCheck(_bool bCheck) { m_bIsColorCheck = bCheck; }
public:
	_bool	Get_ColorCheck() { return m_bIsColorCheck; }
private:
	CRenderer*				m_pRendererCom = nullptr;
public:	
	CVIBuffer_Cube*			m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
private:
	_float3					m_vRotation;
	_float					m_fRotY = 0.f;
	_bool					m_bIsClone = false;
	_bool					m_bIsColorCheck = false;

private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static CNavigationCube* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END