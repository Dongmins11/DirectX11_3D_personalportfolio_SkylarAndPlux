#pragma once

#include "Base.h"

BEGIN(Engine)
class CGameObject;
class CRendererLayer final :public CBase
{
private:
	CRendererLayer();
	virtual ~CRendererLayer() =default;
public:
	HRESULT Add_RenderGroup(CGameObject* pGameObject);
	HRESULT Render();
	HRESULT Sort_Render();
private:
	list<CGameObject*>		m_listRendererLayer;
public:
	static CRendererLayer* Create();
	virtual void Free() override;
};

END