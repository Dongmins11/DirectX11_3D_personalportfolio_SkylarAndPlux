#pragma once

#include "Component.h"
#include "Cell.h"
BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNaviDesc
	{
		_uint		iCurrentIndex;
	}NAVIDESC;
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pNaviDataFilePath);
	virtual HRESULT NativeConstruct(void* pArg);
public:
	_vector Get_IndexPoint(_uint iIndex, CCell::POINT ePoint) { return m_Cells[iIndex]->Get_Point(ePoint); }

public:
	void Update(_fmatrix TransformMatrix);
	_bool Move_OnNavigation(_fvector vPosition);



#ifdef _DEBUG
public:
	HRESULT Render(_uint iPassIndex);
#endif // _DEBUG


private:
	vector<class CCell*>				m_Cells;
	typedef vector<class CCell*>		CELLS;

private:
	NAVIDESC		m_NaviDesc;

private:
	HRESULT SetUp_Neighbor();


public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const _tchar* pNaviDataFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END