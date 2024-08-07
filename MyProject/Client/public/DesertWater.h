#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CModel;
class Colider;
END


BEGIN(Client)
class CDesertWater final : public CGameObject
{
private:
	explicit CDesertWater(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CDesertWater(const CDesertWater& rhs);
	virtual ~CDesertWater() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	_float			m_fTimeDelta = 0.f;
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	Colider*		m_pColiderCom = nullptr;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CDesertWater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;

};

END