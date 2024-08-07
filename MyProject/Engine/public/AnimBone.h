#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimBone final : public CBase
{
private:
	CAnimBone();
	virtual ~CAnimBone() =default;
public:
	HRESULT NativeConstruct();
	void	Add_KeyFrame(KEYFRAME* pKeyFrame) { m_KeyFrames.push_back(pKeyFrame); }

public:
	vector<KEYFRAME*>	Get_KeyFrames() { return m_KeyFrames; }

	_uint				Get_CurrentKeyFrameIndex() const { return m_iCurrentKeyFrame; }

	_uint				Get_PreKeyFrameIndex() const { return m_iPreKeyFrame; }

	_bool				Get_ChangeFrameCheck() const { return m_bIsChangeCheck; }

	void				Set_CurrentKeyFrameIndex(_uint iKeyFrameIndex) { m_iCurrentKeyFrame = iKeyFrameIndex; }

	void				Set_PreKeyFrameIndex(_uint iPreKeyFrameIndex) { m_iPreKeyFrame = iPreKeyFrameIndex; }

	void				Set_TransformationMatrix(_fmatrix TransformationMatrix) { XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix); }

	void				Sey_ChangeKeyFrameCheck(_bool bCheck) { m_bIsChangeCheck = bCheck; }

	_matrix				Get_TransformationMatrix() { return XMLoadFloat4x4(&m_TransformationMatrix); }

private:
	vector<KEYFRAME*>	m_KeyFrames;

	_uint				m_iCurrentKeyFrame = 0;

	_uint				m_iPreKeyFrame = 0;

	_float4x4			m_TransformationMatrix;

	_bool				m_bIsChangeCheck = false;
public:
	static CAnimBone* Create();
	virtual void Free() override;
};

END