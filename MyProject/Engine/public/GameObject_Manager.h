#pragma once
#include "Base.h"



BEGIN(Engine)
class CGameObject;
class CLayer;

class CGameObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CGameObject_Manager)
private:
	CGameObject_Manager();
	virtual ~CGameObject_Manager() = default;
public:
	class CComponent*	Get_Components(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndex,_uint iLayerIndex);
	CGameObject*		Get_GameObject(_uint iLevelIndex,const _tchar* pLayerTag);
public:
	_uint	Get_GameObjectSize(_uint iLevelIndex, const _tchar* pLayerTag);
public:
	HRESULT	Clear_Object(_uint iLevelIndex);
	HRESULT Release_GameObject(_uint iLevelIndex,const _tchar* pLayerTag); //레이어전체를 지워버린다는식
	HRESULT Release_GameObject_Layer(_uint iNumLevel, const _tchar* pLayerTag, _uint iIndex);
public:
	HRESULT Reserve_GameObjectManager(_uint iLevelIndex);
	HRESULT Add_Prototype(_uint iLevelIndex,const _tchar* pPrototypeTag,CGameObject* pGameObject);
	HRESULT Add_ObjectToLayer(_uint iLevelIndex,const _tchar* pPrototypeTag,_uint iLayerLevelIndex,const _tchar* pLayerTag,void* pArg,CGameObject** ppOutGameObject = nullptr);
	_int	Tick(_double TimeDelta);
	_int	LateTick(_double TimeDelta);
private:
	CGameObject*	Finder_GameObject(_uint iLevelIndex,const _tchar* pPrototypeTag);
	CLayer*			Finder_Layer(_uint iLevelIndex,const _tchar* pLayerTag);
private:
	map<const _tchar*, CGameObject*>*	m_mapPrototype = nullptr;
private:
	map<const _tchar*, CLayer*>*			m_mapLayer = nullptr;
private:
	_uint	m_iNumLevel =0;

public:
	virtual void Free() override;
};
END

