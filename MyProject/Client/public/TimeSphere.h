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
class CTimeSphere final : public CGameObject
{
private:
	explicit CTimeSphere(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CTimeSphere(const CTimeSphere& rhs);
	virtual ~CTimeSphere() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_KeyCheck(_bool bKeyCheck) { m_bKeyCheck = bKeyCheck; }
	void	Set_WolrdMat(_float4x4 WorldMat);
	void	Set_Position(_fvector vPos);
private:
	HRESULT	Change_Scale(_double TimeDelta);
private:
	HRESULT Set_VecCollider();
	HRESULT Collision_Check();

private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
private:
	CColider*		m_pSphere = nullptr;
private:
	_bool			m_bScaleCheck = false;
	_bool			m_bKeyCheck = false;
	_double			m_dTimeAcc = 0.0;
private:
	vector<CColider*>	m_vecTargetCollider;
private:
	CGameObject*	m_pDefferd_TimeSphere = nullptr;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CTimeSphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;

};

END