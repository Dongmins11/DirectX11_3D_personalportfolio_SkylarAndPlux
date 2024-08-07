#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CEndScene final : public CGameObject
{
private:
	CEndScene(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEndScene(const CEndScene& rhs);
	virtual ~CEndScene() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	HRESULT Chat_IndexChange(_double TimeDelta);
private:
	_float					m_fPosX, m_fPosY, m_fSizeX, m_fSizeY;
	_float4x4				m_ProjectionMatrix;
private:
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
private:
	_uint					m_iCurChatIndex = 0;
	_uint					m_iPassIndex = 0;
	_float					m_fAlphaColor = 0.f;
private:
	_bool					m_bRealEndCheck = false;
	_bool					m_bChatCheck = false;
	_double					m_bChatCheckTime = 0.0;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();


public:
	static CEndScene* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;
};

END