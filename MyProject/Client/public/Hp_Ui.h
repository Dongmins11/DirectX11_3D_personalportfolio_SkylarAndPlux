#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CRock;
class CHp_Ui final : public CGameObject
{
public:
	typedef struct tagHpUiDesc
	{
		_float fPosX;
		_float fPosY;
		_float fSizeX;
		_float fSizeY;
		_float fScaleSet;
		_float fweight;
	}HPUIDESC;
private:
	CHp_Ui(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CHp_Ui(const CHp_Ui& rhs);
	virtual ~CHp_Ui() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_RenderCheck(_bool bCheck) { m_bRenderCheck = bCheck; }
public:
	void	Set_TransCheck(_bool bCheck) { m_bSetTransCheck = bCheck; }
	void	Set_PassIndedx(_uint iPassIndex) { m_iPassIndex = iPassIndex; }
private:
	HRESULT Trans_Render();

private:
	_float					m_fPosX, m_fPosY, m_fSizeX, m_fSizeY;
	_float4x4				m_ProjectionMatrix;
	_float					m_fScaleSet = 0.f;
	_float					m_fweight = 0.f;
	_bool					m_bIsScale[2];
	_bool					m_bSetTransCheck = false;
private:
	_uint					m_iPassIndex = 0;
	_float					m_fAlphaColor = 0.f;
private:
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pTextureBackCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferBackCom = nullptr;
private:
	HPUIDESC				m_tHpUiDesc;
private:
	_bool					m_bRenderCheck = false;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT SetUp_ConstantBackTable();


public:
	static CHp_Ui* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END