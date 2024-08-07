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
class CTimeSphere;
class CTimePedstal final : public CGameObject
{
private:
	explicit CTimePedstal(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CTimePedstal(const CTimePedstal& rhs);
	virtual ~CTimePedstal() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
private:
	HRESULT Collision_Check(_double TimeDelta);
private:
	CRenderer*		m_pRendererCom = nullptr;
	CColider*		m_pSphere = nullptr;
private:
	CTransform*		m_pTransformCom = nullptr;
	CTransform*		m_pOneTransformCom = nullptr;
	CTransform*		m_pTwoTransformCom = nullptr;
	CTransform*		m_pThreeTransformCom = nullptr;
	CTransform*		m_pForTransformCom = nullptr;
private:
	CModel*			m_pModelCom = nullptr;
	CModel*			m_pOneModelCom = nullptr;
	CModel*			m_pTwoModelCom = nullptr;
	CModel*			m_pThreeModelCom = nullptr;
	CModel*			m_pForModelCom = nullptr;
private:
	CTimeSphere*	m_pTimeSphere = nullptr;
private:
	_double			m_dTimeAcc = 0.0;
	_double			m_dAnimTimeAcc = 0.0;
private:
	_float4			m_vSourcePosition;
	_float4			m_vOneSourcePosition;
	_float4			m_vDestPosition;
	_float4			m_vOneDestPosition;
	_float4			m_vTwoDestPosition;
	_float4			m_vThreeDestPosition;
private:
	_bool			m_bKeyCheck = false;
	_bool			m_bPosCheck = false;
private:
	HRESULT SetUp_Component();
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_ConstantTable();
public:
	static	CTimePedstal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;

};

END