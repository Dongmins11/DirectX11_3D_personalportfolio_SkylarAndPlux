#include "..\public\PipeLine.h"


IMPLEMENT_SINGLETON(CPipeLine)
CPipeLine::CPipeLine()
{
}

void CPipeLine::Update_Matrix()
{
	_matrix ViewMatrix = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_HoldMatrix[D3DTS_VIEW]));
	XMStoreFloat3(&m_vCameraPosition, ViewMatrix.r[3]);
}

void CPipeLine::Free()
{
}
