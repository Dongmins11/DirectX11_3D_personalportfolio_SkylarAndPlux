#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CTalk_UI final : public CGameObject
{
private:
	CTalk_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CTalk_UI(const CTalk_UI& rhs);
	virtual ~CTalk_UI() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_TalkIndex(_uint iIndex) { m_iTalkIndex = iIndex; }
	void	Set_RenderCheck(_bool bCheck) { m_bRenderCheck = bCheck; }
	void	Set_DeadCheck(_bool bCheck) { m_bDeadCheck = bCheck; }
private:
	_float					m_fPosX, m_fPosY, m_fSizeX, m_fSizeY;
	_float4x4				m_ProjectionMatrix;
private:
	_uint					m_iTalkIndex = 0;
	_bool					m_bRenderCheck = false;
	_bool					m_bDeadCheck = false;

private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;

private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();


public:
	static CTalk_UI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END