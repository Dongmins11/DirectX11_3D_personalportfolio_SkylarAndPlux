#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Rock.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CModel;
class CColider;
END


BEGIN(Client)
class CRopeRobot final : public CGameObject
{
private:
	explicit CRopeRobot(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CRopeRobot(const CRopeRobot& rhs);
	virtual ~CRopeRobot() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_RenderCheck(_bool bRenderCheck) { m_bIsRenderCheck = bRenderCheck; }
	void	Set_RotationCheck(_bool bRotationCheck) { m_bRotationCheck = bRotationCheck; }
	void	Set_Rotation(_double dRotation) { m_Rotation = dRotation; }
	_bool	Get_RenderCheck() { return m_bIsRenderCheck; }
public:
	_bool	Get_RotationCheck() { return m_bRotationCheck; }
	_bool	Get_RopeCollisionCheck() { return m_bIsRopeCollisionCheck; }
public:
	_matrix	Get_RopeWolrdMatrix();
private:
	HRESULT	Rope_Swing(_double TimeDelta);
	HRESULT Rope_Look(_double TimeDelta);
private:
	HRESULT	Rope_CollisionCheck();
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CTransform*		m_pSphereTransform = nullptr;
	CTransform*		m_pRopeTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CModel*			m_pSphereModelCom = nullptr;
	CModel*			m_pRopeModelCom = nullptr;
	CColider*		m_pSphere = nullptr;
private:/*For.Colider*/
	_bool			m_bIsRenderCheck = false;
	_bool			m_bIsRopeCollisionCheck = false;
private:/*For.Rotation*/
	_float4x4		m_MatRopeWorldMatrix;
	_bool			m_bRotationCheck = false;
	_double			m_Rotation = 0.0;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CRopeRobot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;

};

END