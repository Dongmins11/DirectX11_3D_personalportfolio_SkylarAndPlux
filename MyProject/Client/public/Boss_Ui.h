#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CBoss_Ui final : public CGameObject
{
private:
	CBoss_Ui(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CBoss_Ui(const CBoss_Ui& rhs);
	virtual ~CBoss_Ui() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_HitHp(_float fHp) { m_fHp = fHp; }
	void	Set_RenderCheck(_bool bCheck) { m_bRenderCheck = bCheck; }
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
	_float4					m_vRectColor;
	_float					m_fHp = 0.f;
	_float					m_fTraceHp = 0.f;
	_uint					m_iPassIndex = 0;
	_bool					m_bJetPackUsingCheck = false;
	_bool					m_bPowerCheck = false;
	_bool					m_bRenderCheck = false;
	


private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT SetUp_ConstantFrontTable();
	HRESULT SetUp_ConstantBackTable();


public:
	static CBoss_Ui* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;
};

END