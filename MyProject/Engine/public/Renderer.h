#pragma once

#include "Component.h"

BEGIN(Engine)
class CRendererLayer;
class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDER_GROUP{ RENDER_PRIORITY,RENDER_NONALPHA,RENDER_LIGHT,RENDER_NONLIGHT,RENDER_ALPHA,RENDER_MAPBLUR,RENDER_EFFECT,RNDER_BULR,RENDER_UI,RENDER_ENDSCENE,RENDER_END};
private:
	explicit CRenderer(ID3D11Device* pDevice,ID3D11DeviceContext* pDeviceContext);
	virtual ~CRenderer() =default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	HRESULT Add_RenderGroup(RENDER_GROUP eRenderState, class CGameObject* pGameObject);
public:
	void	Set_RenderTargetCheck(_bool bCheck) { m_bIsRender = bCheck; }
public:
	HRESULT Draw_Object();
private:
	HRESULT Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Light();
	HRESULT Render_Blend();
	HRESULT	Render_NonLight();
	HRESULT Render_Alpha();
	HRESULT Render_EFFECT();
	HRESULT Render_Total();
	HRESULT Render_MapBlur();
	HRESULT Render_Blur();
	HRESULT Render_EndScene();
	HRESULT Render_UI();
private:
	class CRenderTarget_Manager*			m_pTarget_Manager = nullptr;
	class CLight_Manager*					m_pLight_Manager = nullptr;
private:
	class CVIBuffer_Rect*					m_pVIBuffer = nullptr;
	class CVIBuffer_Rect*					m_pTotalVIBuffer = nullptr;
	class CVIBuffer_Rect*					m_pMapVerticalBlurVIBuffer = nullptr;
	class CVIBuffer_Rect*					m_pMapHorizonBlurVIBuffer = nullptr;
	class CVIBuffer_Rect*					m_pVerticalBlurVIBuffer = nullptr;
	class CVIBuffer_Rect*					m_pHorizonBlurVIBuffer = nullptr;
	_float4x4								m_TransformMatrix;
	_float4x4								m_ProjMatrix;
private:
	_bool									m_bIsRender = false;
private:
	D3D11_VIEWPORT							m_tViewPortDesc;
private:
	char* buf;
	_float f;
private:
	_float Weight[13] = {};
private:
	CRendererLayer*	m_pRendererGroup[RENDER_END];
public:
	static  CRenderer*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

