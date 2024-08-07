#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class CMeshContainer final : public CVIBuffer
{
private:
	CMeshContainer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	CMeshContainer(const CMeshContainer& rhs);
	virtual ~CMeshContainer() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}
public:
	virtual HRESULT NativeConstruct_Prototype(aiMesh* pAIMesh, _uint iMaterialIndex, _fmatrix PivotMatrix);
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render(_uint iPassIndex) override;

private:
	char				m_szName[MAX_PATH] = "";
	_uint				m_iMaterialIndex = 0;
	_uint				m_iMaxIndex = 0;
	_float				m_fPos = 0.f;
	

public:
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, aiMesh* pAIMesh, _uint iMaterialIndex, _fmatrix PivotMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END