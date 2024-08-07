#pragma once

#include "Base.h"

BEGIN(Engine)
class CMyRenderTarget;
class CRenderTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CRenderTarget_Manager)
private:
	CRenderTarget_Manager();
	virtual ~CRenderTarget_Manager() =default;
public:
	ID3D11ShaderResourceView* Get_ShaderResourceView(const _tchar* pTargetTag);

public:
	HRESULT NativeConsturct(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
	HRESULT Add_MultiRenderTarget(const _tchar* pMultiTargetTag, const _tchar* pRenderTargetTag);
	HRESULT	Begin_MultiRenderTarget(ID3D11DeviceContext* pDeviceContext, const _tchar* pMultiTargetTag);
	HRESULT	End_MultiRenderTarget(ID3D11DeviceContext* pDeviceContext);
#ifdef _DEBUG
public:
	HRESULT	Ready_Debug_Buffer_Desc(const _tchar* pTargetTag, _float fStartX, _float fStartY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_Buffer(const _tchar* pMRTTag);
#endif // _DEBUG

private:
	map<const _tchar*,CMyRenderTarget*>		m_MapRenderTargets;

	ID3D11RenderTargetView*			m_pBackBufferView = nullptr;
	ID3D11DepthStencilView*			m_pDepthStencilView = nullptr;
private:
	map<const _tchar*, list<CMyRenderTarget*>>	m_MapMultiRenderTargets;

#ifdef _DEBUG
private:
	class CVIBuffer_Rect*		m_pVIBuffer = nullptr;
	_float4x4					m_ProjectionMatrix;
#endif // _DEBUG
private:
	CMyRenderTarget*			Finder_RenderTargetView(const _tchar*	pRenderTargetTag);
	list<CMyRenderTarget*>*		Finder_MultiRenderTargetView(const _tchar* pMultiRenderTargetTag);

public:
	virtual void Free() override;
};

END