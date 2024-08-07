#include "..\public\Hierachy_Node.h"



CHierachy_Node::CHierachy_Node()
{
}

HRESULT CHierachy_Node::NativeConstruct(const char * pNodeName, _fmatrix TransforMationMatrix, CHierachy_Node * pParent, _uint iDepth)
{

	m_iDepth = iDepth;

	strcpy_s(m_szNodeName, pNodeName);

	XMStoreFloat4x4(&m_OffsetMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_TransformationMatrix, TransforMationMatrix);

	m_pParent = pParent;
	Safe_AddRef(m_pParent);

	return S_OK;
}

void CHierachy_Node::Update_CombinedTransformationMatrix(_uint iAnimIndex)
{
	CAnimBone*	pAnimBone = m_AnimBones[iAnimIndex];

	_matrix		TransformationMatrix;

	if (nullptr == pAnimBone)
		TransformationMatrix = XMLoadFloat4x4(&m_TransformationMatrix);
	else
		TransformationMatrix = pAnimBone->Get_TransformationMatrix();



	if (nullptr != m_pParent)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, TransformationMatrix * m_pParent->Get_CombinedTransformationMatrix());
	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, TransformationMatrix * XMMatrixIdentity());



}

CHierachy_Node * CHierachy_Node::Create(const char * pNodeName, _fmatrix TransforMationMatrix, CHierachy_Node * pParent, _uint iDepth)
{
	CHierachy_Node* pInstance = new CHierachy_Node;

	if (FAILED(pInstance->NativeConstruct(pNodeName, TransforMationMatrix, pParent, iDepth)))
	{
		MSGBOX("Failed to Create CHierachy_Node");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHierachy_Node::Free()
{
	Safe_Release(m_pParent);
}

