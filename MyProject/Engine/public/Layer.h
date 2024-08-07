#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)
class CLayer final :public CBase
{
private:
	CLayer();
	virtual ~CLayer() =default;
public:
	class CComponent*	Get_Components(const _tchar* pComponentTag, _uint iIndex,_uint iLayerIndex);
	_uint				Get_LayerSize();
public:
	CGameObject* Finder_LayerGameObject();
	HRESULT		 Release_Layer(_uint iIndex);
public:
	HRESULT Add_GameObject(CGameObject* pGameObject);
	_int	Tick(_double TimeDelta);
	_int	LateTick(_double TimeDelta);
private:
	list<CGameObject*> m_listGameObject;
private:
	_uint	m_iDeadNumber;
public:
	static CLayer* Create();
	virtual void Free() override;
};

END