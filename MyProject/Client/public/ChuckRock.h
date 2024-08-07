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
class CChuckRock final : public CGameObject
{
private:
	explicit CChuckRock(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CChuckRock(const CChuckRock& rhs);
	virtual ~CChuckRock() =default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	Colider*		m_pColiderCom = nullptr;
	CTexture*		m_pSourceTextureCom = nullptr;
	CTexture*		m_pFilterTextureCom = nullptr;
	CTexture*		m_pNomalTextureCom = nullptr;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CChuckRock* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;

};

END