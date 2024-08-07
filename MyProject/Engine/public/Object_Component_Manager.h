#pragma once

#include "Base.h"
BEGIN(Engine)

class CObject_Component_Manager final :public CBase
{
private:
	CObject_Component_Manager();
	virtual ~CObject_Component_Manager() =default;
public:
	HRESULT		Add_Component_Layer(class CComponent*	pComponent);
	class CComponent*	Get_Component(_uint iIndex);
	HRESULT		Relese_ComponentLayer(_uint iIndex);
private:
	list<class CComponent*>	m_ListComponentLayer;
public:
	static	CObject_Component_Manager*	Create();
	virtual void Free() override;
};

END