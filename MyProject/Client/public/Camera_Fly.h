#pragma once

#include "Client_Defines.h"
#include "Camera.h"


BEGIN(Client)

class CCamera_Fly final :public CCamera
{
private:
	explicit CCamera_Fly(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCamera_Fly(const CCamera_Fly& rhs);
	virtual ~CCamera_Fly() =default;
public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;
	virtual _int LateTick(_double TimeDelta) override;
	virtual HRESULT Render() override;
public:
	void	Set_MainLook(_bool bMainLookCheck) { m_bMainLookCheck = bMainLookCheck; }
	void	Set_iCamIndex(_uint iIndex) { m_iCamIndex = iIndex; }
public:
	void	Set_DistanceCheck(_bool bCheck) { m_bDistanceCheck = bCheck; }
private:
	HRESULT	Main_Look(_double TimeDelta);
	
private:
	HRESULT Boss_Look(_double TimeDelta);
	HRESULT Stage2Boss_Look(_double TimeDelta);
	HRESULT BackUp_Look(_double TimeDelta);
	HRESULT Gravity_Look(_double TimeDelta);
	HRESULT Talk_NpcLook(_double TimeDelta);
	HRESULT Talk_ItemLook(_double TimeDelta);
	HRESULT Cam_Shake(_double TimeDelta);
	HRESULT Cam_ShakeTwo(_double TimeDelta);
	HRESULT Cam_ShakeThree(_double TimeDelta);
	HRESULT Cam_ShakeFor(_double TimeDelta);
	HRESULT End_Scence(_double TimeDelta);
public:
	_matrix	Get_WorldMatrixCam() { return m_pTransform->Get_WorldMatrix(); }
private:
	const _float4x4*		m_pPlayerMatrix = nullptr;
	_vector					m_Distance;
	_float					m_fDisZ = 0.f;
	_float					m_fDisY = 0.f;
private:
	_bool					m_bIsCheckDot[2] = { false };
private:
	_bool					m_bMainLookCheck = false;
	_bool					m_bNpcLookCheck = false;
	_uint					m_iCamIndex = 0;
	_uint					m_iPreIndex = 0;
private:
	_bool					m_bTestCheck = false;
	_double					m_bTestTime = 0.0;
private:
	_float4x4				m_bBackupMat;
private:
	_double					m_dBossLookTimeAcc = 0.0;
	_bool					m_bBossCheck = false;
	_bool					m_bBossCheckTwo = false;
private:
	_bool					m_bDistanceCheck = false;
private:
	_double					m_bShakeTime = 0.0;
private:
	_bool					m_bEndLookCheck = false;
private:
	HRESULT SetUp_Component();
	HRESULT Player_SetPosition_Shake(_double TimeDelta);
	HRESULT Player_SetPosition_ShakeTwo(_double TimeDelta);
	HRESULT Player_SetPosition_ShakeThree(_double TimeDelta);
	HRESULT Player_SetPosition_ShakeFor(_double TimeDelta);
	HRESULT Player_SetPosition(_double TimeDelta);
	_bool Update_MouseMove(_double TimneDelta, _long MouseMove);
public:
	static CCamera_Fly* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;

};

END