#pragma once

#include "Base.h"
#include "AnimBone.h"

BEGIN(Engine)

class CHierachy_Node final :public CBase
{
private:
	CHierachy_Node();
	virtual ~CHierachy_Node() =default;

public:
	_uint	Get_Depth() const { return m_iDepth; }

	const char* Get_Name() const { return m_szNodeName; }

	_matrix	Get_CombinedTransformationMatrix() { return XMLoadFloat4x4(&m_CombinedTransformationMatrix); }

	_matrix	Get_OffsetMatrix() { return XMLoadFloat4x4(&m_OffsetMatrix); }

	const _float4x4*	Get_CombinedTransformationMatrixPtr() { return &m_CombinedTransformationMatrix; }
public:
	HRESULT NativeConstruct(const char* pNodeName, _fmatrix TransforMationMatrix, CHierachy_Node* pParent, _uint iDepth);
public:
	void	SetUp_OffsetMatrix(_fmatrix OffsetMatrix) { XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix); }

	void	Update_CombinedTransformationMatrix(_uint	iAnimIndex);

public:
	void	Add_AnimBones(_uint iAnimMationIndex, CAnimBone* pAnimBone) { m_AnimBones[iAnimMationIndex] = pAnimBone; }
	void	Resize_AnimBones(_uint iAnimMationIndex) { m_AnimBones.resize(iAnimMationIndex); }
private:
	char			m_szNodeName[MAX_PATH] = "";

	_float4x4		m_OffsetMatrix;

	_float4x4		m_TransformationMatrix;

	_float4x4		m_CombinedTransformationMatrix;

	CHierachy_Node* m_pParent = nullptr;

	_uint			m_iDepth = 0;
private:
	vector<CAnimBone*>			m_AnimBones;
public:
	static CHierachy_Node* Create(const char* pNodeName, _fmatrix TransforMationMatrix, CHierachy_Node* pParent, _uint iDepth);
	virtual void Free() override;
};

END