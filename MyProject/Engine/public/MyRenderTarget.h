#pragma once

#include "Base.h"

BEGIN(Engine)

class CMyRenderTarget final : public CBase
{
private:
	explicit CMyRenderTarget(ID3D11Device* pDevice,ID3D11DeviceContext* pDeviceContext);
	virtual ~CMyRenderTarget() =default;
public:
	HRESULT		NativeConstruct(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
	HRESULT		Clear_RenderTarget();
public:
	ID3D11RenderTargetView* Get_RenderTargetView() { return m_pRenderTargetView; }

	ID3D11ShaderResourceView* Get_ShaderResourceView() { return m_pShaderResoureceView; }

#ifdef _DEBUG
public:
	HRESULT		Ready_Debug_Desc(_float fStartX, _float fStartY, _float fSizeX, _float fSizeY);
	HRESULT		Render_Debug_Buffer(class CVIBuffer* pVIBuffer);
#endif // _DEBUG

private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;

private:
	ID3D11Texture2D*			m_pTexture = nullptr;
	ID3D11RenderTargetView*		m_pRenderTargetView = nullptr;
	ID3D11ShaderResourceView*	m_pShaderResoureceView = nullptr;
	_float4						m_vClearColor;

private:
#ifdef _DEBUG
	_float4x4				m_TransformMatrix;
#endif // _DEBUG

public:
	static CMyRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
	virtual void Free() override;
};

END