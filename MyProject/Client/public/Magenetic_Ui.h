#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CMagenetic_Ui final : public CGameObject
{
private:
	CMagenetic_Ui(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMagenetic_Ui(const CMagenetic_Ui& rhs);
	virtual ~CMagenetic_Ui() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_RenderCheck(_bool bCheck) { m_bRenderCheck = bCheck; }
private:
	HRESULT Trans_Render();

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
	CGameObject*			m_pMagenetic_Skill = nullptr;
private:
	_bool					m_bRenderStateCheck = false;
	_uint					m_iPassIndex = 0;
	_float4					m_fRectColor;
private:
	_bool					m_bRenderCheck = false;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT SetUp_ConstantFrontTable();
	HRESULT SetUp_ConstantBackTable();


public:
	static CMagenetic_Ui* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;
};

END