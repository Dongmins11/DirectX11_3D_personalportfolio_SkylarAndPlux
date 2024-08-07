#pragma once

#include "Tool_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CRenderer;
class CVIBuffer_Navigation;
class CVIBuffer_Cube;
class CTransform;
END

BEGIN(Tool)
class CNavigationCube;
class CNavigationTool final : public CGameObject
{
public:
	enum PROPERTY { PROPERTY_GROUND, PROPERTY_FALL, PROPERTY_JUMP, PROPERTY_ROTATION, PROPERTY_END };
private:
	CNavigationTool(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CNavigationTool(const CNavigationTool& rhs);
	virtual ~CNavigationTool() = default;

public:
	virtual HRESULT Native_Construct_Prototype() override;
	virtual HRESULT Native_Construct(void* pArg,_uint iMyLevelIndex) override;
	virtual _int Tick(_double TimeDelta) override;;
	virtual _int LateTick(_double TimeDelta) override;;
	virtual HRESULT Render();
public:
	HRESULT Finder_MapCube(wstring	MapTag);
	void	Set_ColorRender(_bool bCheck) { m_bIsColorCheck = bCheck; }
	HRESULT	Release_ComponentLayer(_uint iIdex);
public:
	vector<CVIBuffer_Navigation*>&			Get_BufferVector() { return m_vVIBufferCom; }
	map<wstring, vector<CNavigationCube*>>&	Get_MapCube() { return m_MapCube; }
public:
	vector<CVIBuffer_Navigation*>			m_vVIBufferCom;
	map<wstring, vector<CNavigationCube*>>	m_MapCube;
	vector<CNavigationCube*>				m_vecCube;
public:
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
public:
	_uint					m_iType = PROPERTY_END;
private:
	_bool					m_bIsRenderCube = false;
	_bool					m_bIsColorCheck = false;
public:
	HRESULT SetUp_Component_Tool(_uint iLevel,const _tchar* PrototypeTag,const _tchar* LayerTag,void* pArg,const _tchar* CubeTag,wstring MapTag,_uint iPropertyIndex);
private:
	HRESULT SetUp_ConstantTable();
	HRESULT SetUp_Component();
public:
	static CNavigationTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg,_uint iMyLevelIndex) override;
	virtual void Free() override;
};

END