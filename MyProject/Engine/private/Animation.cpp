#include "..\public\Animation.h"
#include "AnimBone.h"
#include "Model.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::NativeConstruct(_double TickPerSecond, _double Duration)
{
	m_TickPerSecond = TickPerSecond;
	m_Duration = Duration;

	return S_OK;
}

void CAnimation::Update_AnimBoneTransformationMatrix(_double TimeDelta, _bool ReturnBackCheck, _bool* pEndCheck)
{
	m_TimeAcc += m_TickPerSecond * TimeDelta;

	if (m_TimeAcc >= m_Duration)
	{
		m_isFinished = true;
		m_TimeAcc = 0.0;
		if (nullptr != pEndCheck)
			*pEndCheck = m_isFinished;
		if (true == ReturnBackCheck)
			return;
	}
	else
	{
		m_isFinished = false;
		if (nullptr != pEndCheck)
			*pEndCheck = m_isFinished;
	}




	for (auto& pAnimBone : m_vecAnimBone)
	{
		if (true == m_isFinished)
			pAnimBone->Set_CurrentKeyFrameIndex(0);

		vector<KEYFRAME*>	KeyFrames = pAnimBone->Get_KeyFrames();

		_vector		vSourScale, vDestScale, vScale;
		_vector		vSourRotation, vDestRotation, vRotation;
		_vector		vSourPosition, vDestPosition, vPosition;

		_uint iCurrentKeyFrame = pAnimBone->Get_CurrentKeyFrameIndex();

		if (m_TimeAcc >= KeyFrames.back()->Time)
		{
			KEYFRAME*		pKeyFrame = KeyFrames.back();

			vScale = XMLoadFloat3(&pKeyFrame->vScale);
			vRotation = XMLoadFloat4(&pKeyFrame->vRotation);
			vPosition = XMLoadFloat3(&pKeyFrame->vPosition);

			iCurrentKeyFrame = KeyFrames.size() - 1;
			pAnimBone->Set_CurrentKeyFrameIndex(iCurrentKeyFrame);
		}
		else
		{
			while (m_TimeAcc >= KeyFrames[iCurrentKeyFrame + 1]->Time)
			{
				iCurrentKeyFrame = iCurrentKeyFrame + 1;
				pAnimBone->Set_CurrentKeyFrameIndex(iCurrentKeyFrame);
			}
			m_iCurrentKeyFrame = iCurrentKeyFrame;

			_float		fRatio = (m_TimeAcc - KeyFrames[iCurrentKeyFrame]->Time)
				/ (KeyFrames[iCurrentKeyFrame + 1]->Time - KeyFrames[iCurrentKeyFrame]->Time);

			vSourScale = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame]->vScale);
			vSourRotation = XMLoadFloat4(&KeyFrames[iCurrentKeyFrame]->vRotation);
			vSourPosition = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame]->vPosition);

			vDestScale = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame + 1]->vScale);
			vDestRotation = XMLoadFloat4(&KeyFrames[iCurrentKeyFrame + 1]->vRotation);
			vDestPosition = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame + 1]->vPosition);

			vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
			vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
			vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
		}

		_matrix			TransformationMatrix;

		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

		pAnimBone->Set_TransformationMatrix(TransformationMatrix);
	}


}

void CAnimation::Update_NextAnimBoneTransformationMatrix(_double TimeDelta, CAnimation * pNextAnimation, CModel * pModel,_double BlenderTime)
{
 	auto Nextiter = m_vecAnimBone.begin();

 	m_TimeDelta += TimeDelta*BlenderTime;

	//현재
	for (auto& pBone : pNextAnimation->m_vecAnimBone)
	{	//과거
		vector<KEYFRAME*>	KeyFrames = pBone->Get_KeyFrames();//과거
		_uint iCurrentKeyFrame = pBone->Get_CurrentKeyFrameIndex();//과거

		vector<KEYFRAME*>	NextKeyFrames = (*Nextiter)->Get_KeyFrames();//현재
		_uint iNextKeyFrame = 0;//현재

		_vector		vSourScale, vDestScale, vScale;
		_vector		vSourRotation, vDestRotation, vRotation;
		_vector		vSourPosition, vDestPosition, vPosition;

		_float		fRatio = (m_TimeDelta);
		//소스 과거
		//데스트 현재

		if (1.f < m_TimeDelta)
		{
			m_TimeDelta = 0;
			pModel->Set_ChangeAnimation(false);

			vScale = XMLoadFloat3(&NextKeyFrames[iNextKeyFrame]->vScale);//현재
			vRotation = XMLoadFloat4(&NextKeyFrames[iNextKeyFrame]->vRotation);
			vPosition = XMLoadFloat3(&NextKeyFrames[iNextKeyFrame]->vPosition);

			_matrix			TransformationMatrix;

			TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

			(*Nextiter)->Set_TransformationMatrix(TransformationMatrix);//현재
			
			pNextAnimation->m_isFinished = true;
			for (auto& pBone : pNextAnimation->m_vecAnimBone)
				pBone->Set_CurrentKeyFrameIndex(0);

			m_isFinished = true;
			for (auto&pBone2 : m_vecAnimBone)
				pBone2->Set_CurrentKeyFrameIndex(0);
			m_TimeAcc = 0;
			
			return;
		}

		vSourScale = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame]->vScale);//과거
		vSourRotation = XMLoadFloat4(&KeyFrames[iCurrentKeyFrame]->vRotation);
		vSourPosition = XMLoadFloat3(&KeyFrames[iCurrentKeyFrame]->vPosition);

		vDestScale = XMLoadFloat3(&NextKeyFrames[iNextKeyFrame]->vScale);//현재
		vDestRotation = XMLoadFloat4(&NextKeyFrames[iNextKeyFrame]->vRotation);
		vDestPosition = XMLoadFloat3(&NextKeyFrames[iNextKeyFrame]->vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);

		_matrix			TransformationMatrix;

		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

		(*Nextiter)->Set_TransformationMatrix(TransformationMatrix);//현재

		++Nextiter;
	}
}


CAnimation * CAnimation::Create(_double TickPerSecond, _double Duration)
{
	CAnimation* pInstance = new CAnimation;

	if (FAILED(pInstance->NativeConstruct(TickPerSecond, Duration)))
	{
		MSGBOX("Failed to Create Animation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	for (auto& pAnimBone : m_vecAnimBone)
		Safe_Release(pAnimBone);

	m_vecAnimBone.clear();
	vector<class CAnimBone*>().swap(m_vecAnimBone);

}
