#pragma once

#include "Model.h"
#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CAnimMeshContainer final : public CVIBuffer
{
private:
	explicit CAnimMeshContainer(ID3D11Device* pDevice,ID3D11DeviceContext* pDeviceContext);
	explicit CAnimMeshContainer(const CAnimMeshContainer& rhs);
	virtual ~CAnimMeshContainer() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(aiMesh* pAIMesh);
	virtual HRESULT NativeConstruct(void* pArg)override;
	virtual HRESULT Render(_uint iPassIndex) override;

public:
	_uint	Get_MaterialIndex() { return m_iMaterialIndex;}
	void	Get_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix	fPivotMatrix);
public:
	HRESULT SetUp_MeshBones(class CModel* pModel, aiMesh* pAiMesh);
private:
	char	m_szName[MAX_PATH] = ""; //하이어라키에있는 본들의 이름을찾으려고
	_uint	m_iMaterialIndex = 0;
	_uint	m_iNumBone = 0;

	vector<class CHierachy_Node*>		m_BoneNode;
public:
	static CAnimMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,aiMesh* pAIMesh);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END