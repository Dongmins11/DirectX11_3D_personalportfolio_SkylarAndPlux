#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CJetPack_Ui final : public CGameObject
{
private:
	CJetPack_Ui(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CJetPack_Ui(const CJetPack_Ui& rhs);
	virtual ~CJetPack_Ui() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_JetPackUi_Render(_bool bCheck) { m_bRenderCheck = bCheck; }
	void	Set_JetPackUsing(_bool bCheck) { m_bJetPackUsingCheck = bCheck; }
	_bool	Get_JetPackPowerCheck() { return m_bPowerCheck; }
private:
	HRESULT Trans_Render(_double TimeDelta);

private:
	_float					m_fPosX, m_fPosY, m_fSizeX, m_fSizeY;
	_float4x4				m_ProjectionMatrix;
	_float					m_fScaleSet = 0.f;
	_float					m_fweight = 0.f;
	_bool					m_bIsScale[2];
private:
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pTextureBackCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferFrontCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferBackCom = nullptr;
private:
	_float					m_fJetPackPower = 0.f;
	_float4					m_vRectColor;
	_uint					m_iPassIndex = 0;
	_bool					m_bJetPackUsingCheck = false;
	_bool					m_bPowerCheck = false;
private:
	_bool					m_bRenderCheck = false;

private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT SetUp_ConstantFrontTable();
	HRESULT SetUp_ConstantBackTable();


public:
	static CJetPack_Ui* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;
};

END