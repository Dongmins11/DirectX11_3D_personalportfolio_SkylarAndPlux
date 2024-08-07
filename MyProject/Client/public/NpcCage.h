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
class CNpcCage final : public CGameObject
{
private:
	explicit CNpcCage(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CNpcCage(const CNpcCage& rhs);
	virtual ~CNpcCage() = default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_Locker_Count(_uint iCount) { m_iLockerCount += iCount; }
	void	Set_AnimationStart(_bool bStart) { m_bIsLockCheck = bStart; }
private:
	HRESULT	Collision_Check_Cage();
private:
	HRESULT			Animation_Play(_double TimeDelta);
private:
	HRESULT			Animation_Main(_double TimeDelta);
	HRESULT			Animation_Door(_double TimeDelta);
	HRESULT			Animation_Left(_double TimeDelta);
	HRESULT			Animation_Right(_double TimeDelta);
	HRESULT			Animation_Back(_double TimeDelta);
	HRESULT			Animation_Locker(_double TimeDelta);
private:
	CRenderer*		m_pRendererCom = nullptr;
private:
	CTransform*		m_pFloorTransformCom = nullptr;
	CTransform*		m_pMainDoorTransformCom = nullptr;
	CTransform*		m_pLeftDoorTransformCom = nullptr;
	CTransform*		m_pRightDoorTransformCom = nullptr;
	CTransform*		m_pBackDoorTransformCom = nullptr;
	CTransform*		m_pLockTransformCom = nullptr;
	CTransform*		m_pCeilingTransformCom = nullptr;
private:
	CModel*			m_pFloorModelCom = nullptr;
	CModel*			m_pMainDoorModelCom = nullptr;
	CModel*			m_pLeftDoorModelCom = nullptr;
	CModel*			m_pRightDoorModelCom = nullptr;
	CModel*			m_pBackDoorModelCom = nullptr;
	CModel*			m_pLockModelCom = nullptr;
	CModel*			m_pCeilingModelCom = nullptr;
private:
	_double			m_dAnimationPlayTime = 0.0;
	_bool			m_bIsPlayCheck = false;
private:
	_float			m_fStartValue = 0.f;
	_float			m_fEndValue = 0.f;
	_float			m_fBlendValue = 0.f;
	_uint			m_iAnimationIndex = 0;
	_uint			m_iBlendTime = 0;
private:
	_uint			m_iLockerCount = 0;
	_double			m_dLockerTime = 0.0;
	_bool			m_bIsLockCheck = false;
private:
	CColider*		m_pSphere = nullptr;
private:
	_float4			m_vMainDestPos;
	_float4			m_vLockerDestPos;
	_float4			m_vMainSourcePos;
	_float4			m_vLockerSourcePos;
private:
	_bool			m_bMainRenderCheck =false;
	_bool			m_bLastRenderCheck = false;
private:
	_bool			m_bMainAnimationCheck = false;
private:
	_double			m_dDeadTime = 0.0;
private:
	CGameObject*	m_pNpcObject = nullptr;
private:
	_bool			m_bEffectCheck = false;
private:
	HRESULT SetUp_Component_Load();
	HRESULT SetUp_Component();
	HRESULT SetUp_ConstantTable();
public:
	static	CNpcCage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;

};

END