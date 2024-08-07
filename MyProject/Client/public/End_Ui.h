#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CEnd_Ui final : public CGameObject
{
private:
	CEnd_Ui(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEnd_Ui(const CEnd_Ui& rhs);
	virtual ~CEnd_Ui() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_EndUiCheck(_bool bCheck) { m_bEndCheck = bCheck; }
private:
	HRESULT End_Scene(_double TimeDelta);

private:
	_float					m_fPosX, m_fPosY, m_fSizeX, m_fSizeY;
	_float4x4				m_ProjectionMatrix;
private:
	_float					m_fDownPosX, m_fDownPosY, m_fDownSizeX, m_fDownSizeY;
	_float4x4				m_DownProjectionMatrix;
private:
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pDownTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pUpVIBufferCom = nullptr;
	CVIBuffer_Rect*			m_pDownVIBufferCom = nullptr;
private:
	_bool					m_bEndCheck = false;
	_bool					m_bNpcAnimStartCheck = false;

private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();


public:
	static CEnd_Ui* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;
};

END