#include "..\public\AnimBone.h"



CAnimBone::CAnimBone()
{
}

HRESULT CAnimBone::NativeConstruct()
{
	return S_OK;
}


CAnimBone * CAnimBone::Create()
{
	CAnimBone*	pInstance = new CAnimBone();

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSGBOX("Failed to Created CAnimBone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimBone::Free()
{
	for (auto& pKeyFrame : m_KeyFrames)
		Safe_Delete(pKeyFrame);

	m_KeyFrames.clear();
}
