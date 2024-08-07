#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CTexture;
class CRenderer;
class CColider;
class CModel;
class CVIBuffer_Cube;
class CTransform;
class CVIBuffer_Colider;
class CVIBuffer_Sphere;
END


BEGIN(Tool)


class CColliderObject_Tool final : public CGameObject
{
private:
	explicit CColliderObject_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CColliderObject_Tool(const CColliderObject_Tool& rhs);
	virtual ~CColliderObject_Tool() = default;
public:
	virtual HRESULT Native_Construct_Prototype()override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	void	Set_MyAddCount(_uint iCount) { m_iMyCount += iCount; }
	void	Set_MyMinusCount(_uint iCount);
public:
	void	Add_ChildColider(CColliderObject_Tool* pGameObject);
public:
	HRESULT Set_Pos();
	HRESULT Set_Scaile();
	HRESULT Set_ScaileXYZ();
	HRESULT Set_Rotation();
	HRESULT	Set_ModelDesc(MODELDESC* pModelDesc);
	_uint	Get_MyCount() { return m_iMyCount; }
public:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CVIBuffer_Cube*			m_pVIBufferCubeCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CColider*				m_pAABB = nullptr;
	CColider*				m_pSphere = nullptr;
public:
	vector<CColliderObject_Tool*>		m_vecChildCollider;
private:
	MODELDESC				m_tModelDesc;
	_bool					m_bIsClone = false;
	_uint					m_iMyCount;
private:
	HRESULT SetUp_Component();
	HRESULT Setup_Component_Tool();
	HRESULT SetUp_ConstantTable();


public:
	static CColliderObject_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free()override;
};

END