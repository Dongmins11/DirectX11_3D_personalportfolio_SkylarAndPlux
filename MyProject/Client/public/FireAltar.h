#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CModel;
class CTexture;
class Colider;
END


BEGIN(Client)
class CFireAltar final : public CGameObject
{
private:
	explicit CFireAltar(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CFireAltar(const CFireAltar& rhs);
	virtual ~CFireAltar() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CFireAltar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END