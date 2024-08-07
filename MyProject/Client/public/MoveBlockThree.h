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
class CMoveBlockThree final : public CGameObject
{
private:
	explicit CMoveBlockThree(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CMoveBlockThree(const CMoveBlockThree& rhs);
	virtual ~CMoveBlockThree() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg, _uint iMyLevelIndex) override;
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
private:
	HRESULT Set_Pos();
private:
	HRESULT Player_Collision_Check(_double TimeDelta);
	HRESULT Rope_Look(_double TimeDelta);
	HRESULT Move_State(_double TimeDelta);
private:
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CTransform*		m_pRopeTransformCom = nullptr;
	CModel*			m_pModelCom = nullptr;
	CModel*			m_pRopeModelCom = nullptr;
	CColider*		m_pSphere = nullptr;
	CColider*		m_pAABB = nullptr;

private:/*For.Colider*/
	_bool			m_bIsRenderCheck = false;
	_bool			m_bIsRopeCollisionCheck = false;
private:/*For.Rotation*/
	_float4x4		m_MatRopeWorldMatrix;
	_bool			m_bRotationCheck = false;
	_double			m_Rotation = 0.0;
private:
	_bool			m_bMoveCheck = false;
	_float4			m_vMyPos;
	_float4			m_vBackUpPos;
	_double			m_dTimeAcc = 0.0;
private:
	CRock::PLAYERCOLLISIONDESC	m_tPlayerCollisionDesc;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CMoveBlockThree* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg, _uint iMyLevelIndex) override;
	virtual void Free() override;

};

END