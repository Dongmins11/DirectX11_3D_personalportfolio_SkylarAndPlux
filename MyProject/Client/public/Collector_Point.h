#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Engine)
class CTransform;
class CTexture;
class CRenderer;
class CVIBuffer_PointInstance_Clone;
class CColider;
END

BEGIN(Client)

class CCollector_Point final : public CGameObject
{
private:
	CCollector_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCollector_Point(const CCollector_Point& rhs);
	virtual ~CCollector_Point() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_DeadCheck(_bool Check) { m_bDeadCheck = Check; }

private:
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_PointInstance_Clone*	m_pVIBufferCom = nullptr;
	CColider*				m_pSphere = nullptr;
private:
	CVIBuffer_PointInstance_Clone::POINTINST	m_tPointInst;
private:
	_float					m_dScale = 0.0f;
private:
	_bool					m_bDeadCheck = false;
private:
	_double					m_dTimeAcc = 0.0;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();


public:
	static CCollector_Point* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END