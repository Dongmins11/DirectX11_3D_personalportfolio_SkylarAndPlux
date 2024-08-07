#pragma once

#include "Component.h"
#include "DebugDraw.h"

BEGIN(Engine)
class CGameObject;
class ENGINE_DLL CColider final : public CComponent
{
public:
	enum TYPE{ TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
public:
	typedef	struct tagColiderDesc
	{
		_float3		vCenter;
		_float3		vExtents;
		_float		fRadius;
	}COLIDERDESC;

	typedef struct tagAABBDesc
	{
		_float3 vMin, vMax;
	}AABBDESC;

	typedef struct tagOBBDesc
	{
		_float3		vCenter;
		_float3		vExtenDeir[3];
		_float3		vAlignDir[3];
	}OBBDESC;

	typedef	struct tagSphereDesc
	{
		_float3		vCenter;
		_float		fRadius;
	}SPHEREDESC;

private:
	explicit CColider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CColider(const CColider& rhs);
	virtual ~CColider() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eType);
	virtual HRESULT NativeConstruct(void* pArg)override;
public:
	void	Set_Colider_Center(_float3 vCenter) { m_tColiderDesc.vCenter = vCenter; }
	void	Set_Colider_Extents(_float3 vExtents) { m_tColiderDesc.vExtents = vExtents; }
	void	Set_Colider_Radius(_float fRadius) { m_tColiderDesc.fRadius = fRadius; }
public:
	void	Update(_fmatrix WolrdMatrix);
	_bool	Collision_AABB(CColider* pTarget);
	_bool	Collision_AABBXYZ(CColider* pTarget, _bool* bX, _bool* bY, _bool* bZ, _float* fDisX, _float* fDisY, _float* fDisZ);
	_bool	Collision_OBB(CColider* pTarget);
	_bool	Collision_Sphere(CColider* pTarget);
public:
	_bool	Collision_AABBOBB(CColider* pTarget);
	_bool	Collision_OBBAABB(CColider* pTarget);
	_bool	Collision_AABBSphere(CColider* pTarget);
	_bool	Collision_SphereAABB(CColider* pTarget);
public:
	_bool	Collision_SphereBoard(CColider* pTarget);
public:
	_float	Get_Radius();
public:
	void	Set_CheckCollision(_bool Check_Collision) { m_bTumCollisionCheck = Check_Collision; }
	_bool	Get_CheckCollision() { return m_bTumCollisionCheck; }
#ifdef _DEBUG
public:
	HRESULT	Render();
#endif // _DEBUG

private:
	_bool		Check_Colision(_float3* vDstMin, _float3* vSrcMin, _float3* vDstMax, _float3* vSrcMax);
private:
	BoundingBox*			m_pAABB = nullptr;
	BoundingOrientedBox*	m_pOBB = nullptr;
	BoundingSphere*			m_pSphere = nullptr;

	TYPE					m_eType = TYPE_END;
	COLIDERDESC				m_tColiderDesc;
private:
	_bool					m_bisCollision = false;
	_bool					m_bTumCollisionCheck = false;
private:
	_matrix		Remove_Rotation(_fmatrix TransformMatrix);
	OBBDESC		Compute_OBBDesc();

private:
	BasicEffect*			m_pEffect = nullptr;
	PrimitiveBatch<DirectX::VertexPositionColor>*	m_pBatch = nullptr;
	ID3D11InputLayout*		m_pInputLayOut = nullptr;
public:
	static CColider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, TYPE eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END