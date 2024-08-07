#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CColider;
class CRenderer;
class CTransform;
class CModel;
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)

class CItemMagenetic final : public CGameObject
{
private:
	CItemMagenetic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CItemMagenetic(const CItemMagenetic& rhs);
	virtual ~CItemMagenetic() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex)override;
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();
private:
	HRESULT Collision_Check();

private:
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CTransform*				m_pBufferTransformCom = nullptr;
	CColider*				m_pSphereCom = nullptr;
	CVIBuffer_Rect*			m_pVIBuffer = nullptr;
	CTexture*				m_pTextureCom = nullptr;
private:
	CGameObject*			m_pEffectObject = nullptr;
private:
	_float4x4				m_OffsetMatrix;
	_float4x4				m_PivotMatrix;
	const _float4x4*		m_pCombinedMatrix = nullptr;
	const _float4x4*		m_pTargetWorldMatrix;
private:
	_double					m_dRotationY = 0.0;
private:
	_bool					m_bDeadCheck = false;


private:
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();


public:
	static CItemMagenetic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;
};

END