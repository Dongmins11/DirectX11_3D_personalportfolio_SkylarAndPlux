#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CCrystal_Ui final : public CGameObject
{
private:
	CCrystal_Ui(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCrystal_Ui(const CCrystal_Ui& rhs);
	virtual ~CCrystal_Ui() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_CristalAdd(_uint iNumber) { m_iCristal += iNumber; }
	void	Set_CristalMinus(_uint iNumber) { m_iCristal -= iNumber; }
public:
	_uint	Get_CristalNumber() { return m_iCristal; }
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
	_tchar					m_szBuffer[20] = L"";
	_uint					m_iCristal = 0;
private:
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pTextureBackCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferBackCom = nullptr;
private:
	_bool					m_bRenderCheck = false;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
	HRESULT SetUp_ConstantBackTable();


public:
	static CCrystal_Ui* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END