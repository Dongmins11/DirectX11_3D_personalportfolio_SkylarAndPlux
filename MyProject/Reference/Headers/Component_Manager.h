#pragma once

#include "Model.h"
#include "Texture.h"
#include "Renderer.h"
#include "Transform.h"
#include "Navigation.h"
#include "Colider.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Colider.h"
#include "VIBuffer_Navigation.h"
#include "VIBuffer_Sphere.h"
#include "VIBuffer_RectInstance.h"
#include "VIBuffer_PointInstance.h"
#include "VIBuffer_PointInstance_Clone.h"

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)
public:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;
public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg);
	HRESULT Clear_Component(_uint iLevelIndex);
private:
	map<const _tchar*, class CComponent*>*				m_pPrototypes = nullptr;
	typedef map<const _tchar*, class CComponent*>		PROTOTYPES;

private:
	_uint				m_iNumLevels = 0;

private:
	CComponent* Find_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag);

public:
	virtual void Free() override;
};

END