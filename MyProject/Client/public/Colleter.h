#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CModel;
class CColider;
END


BEGIN(Client)
class CColleter final : public CGameObject
{
private:
	explicit CColleter(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CColleter(const CColleter& rhs);
	virtual ~CColleter() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_TargetObject(CGameObject* pGameObject);
private:
	HRESULT Blender_Pos_Rot(_double TimeDelta);
	_int	Player_Trace(_double TImeDelta);
	_int	Cage_Trace(_double TimeDelta);
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CColider*		m_pSphere = nullptr;
private:
	CGameObject*	m_pGameObject = nullptr;
private:
	_float4			m_vRandomDestPos;
	_float4			m_vRandomSourcePos;
	_float3			m_vResultPos;

	_float3			m_vRandomDestRotaition;
	_float3			m_vRandomSourceRotation;

	_float			m_fRandomDestScale = 0.f;
	_float			m_fRandomSourceScale = 0.f;

	_double			m_dRandomBlenderTime = 0.0;
	_double			m_dPlayer_BlnederTime = 0.0;
	_double			m_dPlayer_ScaleTime = 0.0;
	
	_double			m_dRandomBlendTime = 0.0;

private:
	_bool			m_bIsScaleCheck = false;
	_bool			m_bIsChangeCheck = false;
private:
	_bool			m_bSoundCheck = false;

private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CColleter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;

};

END