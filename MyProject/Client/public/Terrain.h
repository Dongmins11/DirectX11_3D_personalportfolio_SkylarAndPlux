#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CRenderer;
class CTexture;
class CTransform;
class CVIBuffer_Terrain;
class CNavigation;

END


BEGIN(Client)

class CTerrain final :public CGameObject
{
private:
	explicit CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() =default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Setup_Component();
	HRESULT Setup_ConstantTable();
private:
	CRenderer*			m_pRenderCom = nullptr;
	CTexture*			m_pTextureCom = nullptr;
	CTexture*			m_pTextureNomalCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_Terrain*	m_pVIBufferCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
public:
	static  CTerrain*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END
