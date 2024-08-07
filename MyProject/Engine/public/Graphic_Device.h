#pragma once

#include "Base.h"

BEGIN(Engine)
class CGraphic_Device final :public CBase 
{
	DECLARE_SINGLETON(CGraphic_Device)
public:
	enum WINMODE {MODE_FULL ,MODE_WIN , MODE_END};
public:
	typedef struct tagDeviceDesc
	{
		HWND			hWnd;
		unsigned int	iWinCX, iWinCY;
		WINMODE			eWinMode;
	}DEVICEDESC;
private:
	CGraphic_Device();
	virtual ~CGraphic_Device() =default;
public:
	HRESULT Ready_For_Device(const DEVICEDESC& tDeviceDesc, ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppDeviceContextOut);
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_Depth_Stencil_View();
	HRESULT Present();
private:
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pDeviceContext;
private:
	IDXGISwapChain*			m_pSwapChain;

	ID3D11RenderTargetView*	m_pRenderTargetArray[2];
	ID3D11RenderTargetView*	m_pRenderTarget;
	ID3D11DepthStencilView* m_pDepthStencilView;
private:
	HRESULT Ready_SwapChain(const DEVICEDESC& tDeviceDesc);
	HRESULT Ready_BackBuffer_RenderTargetView();
	HRESULT Ready_DepthStencil_RenderTargetView(_uint iWinCX,_uint iWinCY);
public:
	virtual void Free() override;
};
END
