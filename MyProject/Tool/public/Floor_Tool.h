#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CColider;
class CModel;
class CTransform;
class CVIBuffer_Colider;
class CVIBuffer_Cube;
class CVIBuffer_Sphere;
END

BEGIN(Tool)

class CFloor_Tool final : public CGameObject
{
public:
	enum TYPE
	{
		TYPE_NONANIM,TYPE_ANIM,TYPE_ROTATION,TYPE_FILTER,TYPE_END
	};
private:
	CFloor_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CFloor_Tool(const CFloor_Tool& rhs);
	virtual ~CFloor_Tool() = default;

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
	HRESULT Set_Pos();
	HRESULT Set_Scaile();
	HRESULT Set_ScaileXYZ();
	HRESULT Set_Rotation();
	HRESULT	Set_ModelDesc(MODELDESC* pModelDesc);
	_uint	Get_MyCount() { return m_iMyCount; }
public:
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pVIBufferCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTexture*				m_pTextureComFilter = nullptr;
	CTexture*				m_pNomalTextureCom = nullptr;
	CVIBuffer_Cube*			m_pVIBufferCubeCom = nullptr;
public:
	CVIBuffer_Sphere*		m_pColiderCom = nullptr;
	//CVIBuffer_Sphere*		
	CTransform*				m_pColiderTransformCom = nullptr;

	CColider*				m_pColliderCom = nullptr;

private:
	MODELDESC				m_tModelDesc;
	_bool					m_bIsClone = false;
	_float3					m_vVtxPos;
	_uint					m_iMyCount;
	TYPE					m_eType = TYPE_END;
private:
	_float					m_fTimeDelta = 0.f;
private:
	HRESULT SetUp_Component();
	HRESULT Setup_Component_Tool();
	HRESULT SetUp_ConstantTable();


public:
	static CFloor_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END