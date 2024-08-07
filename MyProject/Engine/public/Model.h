#pragma once

#include "Component.h"
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE{	TYPE_NONANIM, TYPE_ANIM, TYPE_END	};
protected:
	explicit CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CModel(const CModel& rhs);
	virtual ~CModel() = default;
public:
	HRESULT Set_Animation(_uint iAnimationIndex);
	void	Set_PreAnimation(_uint iPreAnimIndex);
	void	Set_ChangeAnimation(_bool bCheck) { m_bIsChange = bCheck; }
public:
	_float4x4	Get_OffsetMatrix(const char* pBoneName);
	_float4x4	Get_PivotMatrix() { return m_PivotMatrix; }
	const _float4x4*	Get_PivotMatrixPtr() { return &m_PivotMatrix; }
	const _float4x4*	Get_CombinedMatrixPtr(const char* pBoneName);
public:
	void		Set_PivotMat(_fmatrix PivotMat) { XMStoreFloat4x4(&m_PivotMatrix, PivotMat); }
public:
	_bool		Get_Animation_FinishiedIndex(_int iIndex);
	_bool		Get_Animation_FinishiedCurrent();
public:
	virtual HRESULT NativeConstruct_Prototype(TYPE eMeshType,const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix);
	virtual HRESULT NativeConstruct(void* pArg);
public:
	CVIBuffer*	Get_MeshContatiner(_uint iIndex = 0) { return m_MeshContainers[iIndex]; }
	vector<CVIBuffer*>*	Get_MeshContatinerVector() { return &m_MeshContainers; }
public:
	HRESULT Set_RawValue(const char* pConstantName, void* pData, _uint iSize);
	HRESULT Set_TextureValue(const char* pConstantName, ID3D11ShaderResourceView* pSRV);
public:
	class CHierachy_Node*	Finder_HierachyNode(const char* pNodeName);
public:

	HRESULT Render(_uint iPassIndex);
	HRESULT Update(_double TimeDelta, _bool* bEndCheck = nullptr,_bool ReturnBackCheck = false,_double BlenderTime = 5.0);

private:
	const aiScene*			m_pScene = nullptr;
	Assimp::Importer		m_Importer;
	TYPE					m_eType = TYPE_END;

private: /* For.Meshes*/
	_uint								m_iNumMeshes;
	vector<CVIBuffer*>			m_MeshContainers;
	typedef vector<CVIBuffer*>	MESHCONTAINERS;
	_float4x4							m_PivotMatrix;

private: /* For.Material */
	_uint								m_iNumMaterials;
	vector<MESHMATERIAL*>				m_Materials;
	typedef vector<MESHMATERIAL*>		MATERIALS;

private: /*For. HieraChyNode*/
	vector<class CHierachy_Node*>		m_Hierachy_Node;

private: /*For. Animation */
	_uint								m_iNumAnimation = 0;
	_uint								m_iCurrentAnimation = 0;
	_uint								m_iPreAnimation = 0;
	_uint								m_iGagu = 0;
	_bool								m_bIsChange = false;
	vector<class CAnimation*>			m_Animations;

protected:
	vector<PASSDESC>					m_Passes;
	ID3DX11Effect*						m_pEffect = nullptr;

private:
	HRESULT Create_MeshContainers();
	HRESULT Create_Shader();
	HRESULT Create_Materials(const char* pModelFilePath);
	HRESULT Create_Hierachy(aiNode* pNode, CHierachy_Node* pParent = nullptr, _uint iDepth = 0);
	HRESULT Create_AniMation();
private:
	HRESULT Setup_MeshBones();
private:
	HRESULT Compile_Shader(const _tchar* pShaderFilePath, D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);


public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext,TYPE eMeshType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END