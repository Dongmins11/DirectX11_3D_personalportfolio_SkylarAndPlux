#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CFadeInOut_UI final : public CGameObject
{
private:
	CFadeInOut_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CFadeInOut_UI(const CFadeInOut_UI& rhs);
	virtual ~CFadeInOut_UI() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_Value(_uint iValue) { m_iValue = iValue; }
	void	Set_FadeCheck(_bool bCheck) { m_bFadeInCheck = bCheck; }
	void	Set_ShutCheck_Open(_bool bCheck) { m_bShutOpen = bCheck; }
private:
	_float					m_fPosX, m_fPosY, m_fSizeX, m_fSizeY;
	_float4x4				m_ProjectionMatrix;
private:
	_float4					m_vColor;
	_double					m_dDeadTime = 0.0;
private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
private:
	_bool					m_bRenderCheck = false;
	_bool					m_bShutOpen = false;
	_bool					m_bShutClose = false;
	_bool					m_bFadeInCheck = false;
	_uint					m_iValue = 0;

private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();


public:
	static CFadeInOut_UI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;
};

END