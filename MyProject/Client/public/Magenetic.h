#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CColider;
class CRenderer;
class CTransform;
class CModel;
class CTexture;
END

BEGIN(Client)
class CMagenetic_Skill;
class CEffect_Magenetic;
class CMagenetic final : public CGameObject
{
private:
	CMagenetic(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMagenetic(const CMagenetic& rhs);
	virtual ~CMagenetic() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex)override;
	virtual _int Tick(_double TimeDelta);
	virtual _int LateTick(_double TimeDelta);
	virtual HRESULT Render();

private:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CColider*				m_pSphereCom = nullptr;
	CModel*					m_pModelCom = nullptr;
private:
	CMagenetic_Skill*		m_pSkill_Object = nullptr;
	CEffect_Magenetic*		m_pEffect_Object = nullptr;
private:
	_float4x4				m_pPlayerWorldMat;

private:
	_float4x4				m_OffsetMatrix;
	_float4x4				m_PivotMatrix;
	const _float4x4*		m_pCombinedMatrix = nullptr;
	const _float4x4*		m_pTargetWorldMatrix;

private:
	HRESULT SetUp_Component();
	HRESULT SetUp_TargetBoneMatrices(const char* pBoneName);
	HRESULT SetUp_ConstantTable();


public:
	static CMagenetic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;
};

END