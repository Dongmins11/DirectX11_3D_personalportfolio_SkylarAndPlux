#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END


BEGIN(Client)

class CSkyBox final : public CGameObject
{
private:
	explicit CSkyBox(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox() =default;
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
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_Cube*		m_pVIBufferCom = nullptr;

public:
	static CSkyBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END