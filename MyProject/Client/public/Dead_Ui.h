#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CDead_Ui final : public CGameObject
{
private:
	CDead_Ui(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CDead_Ui(const CDead_Ui& rhs);
	virtual ~CDead_Ui() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();

private:
	_float					m_fPosX, m_fPosY, m_fSizeX, m_fSizeY;
	_float4x4				m_ProjectionMatrix;
private:
	_float4					m_vColor;
	_double					m_dDeadTime = 0.0;
private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
private:
	_bool					m_bRenderCheck = false;
	_bool					m_bShutOpen = false;
	_bool					m_bShutClose = false;


private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();


public:
	static CDead_Ui* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END