#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "AnimMeshContainer.h"
#include "GameInstance.h"
#include "Hierachy_Node.h"
#include "Texture.h"
#include "Animation.h"
#include "AnimBone.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_pEffect(rhs.m_pEffect)
	, m_Passes(rhs.m_Passes)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_Materials(rhs.m_Materials)
	, m_Hierachy_Node(rhs.m_Hierachy_Node)
	, m_iNumAnimation(rhs.m_iNumAnimation)
	, m_Animations(rhs.m_Animations)
	, m_eType(rhs.m_eType)
	, m_pScene(rhs.m_pScene)
	, m_iCurrentAnimation(rhs.m_iCurrentAnimation)
{
	for (auto& pMeshContainer_Prototype : rhs.m_MeshContainers)
	{
		m_MeshContainers.push_back((CVIBuffer*)pMeshContainer_Prototype->Clone(nullptr));
	}
	Safe_AddRef(m_pEffect);

	for (auto& pPass : m_Passes)
	{
		Safe_AddRef(pPass.pInputLayOut);
		Safe_AddRef(pPass.pPass);
	}
}

HRESULT CModel::Set_Animation(_uint iAnimationIndex)
{
	m_iCurrentAnimation = iAnimationIndex;

	return S_OK;
}

void CModel::Set_PreAnimation(_uint iPreAnimIndex)
{
	m_iPreAnimation = iPreAnimIndex;
}

_float4x4 CModel::Get_OffsetMatrix(const char * pBoneName)
{
	_float4x4	OffsetMatrix;

	CHierachy_Node*		pNode = Finder_HierachyNode(pBoneName);

	if(nullptr == pNode)
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());
	else
		XMStoreFloat4x4(&OffsetMatrix, pNode->Get_OffsetMatrix());



	return OffsetMatrix;
}

const _float4x4 * CModel::Get_CombinedMatrixPtr(const char * pBoneName)
{
	_float4x4	CombinedMatrix;

	CHierachy_Node*		pNode = Finder_HierachyNode(pBoneName);
	if (nullptr == pNode)
		return nullptr;

	return pNode->Get_CombinedTransformationMatrixPtr();
}

_bool CModel::Get_Animation_FinishiedIndex(_int iIndex)
{
	return m_Animations[iIndex]->Get_isFinished();
}

_bool CModel::Get_Animation_FinishiedCurrent()
{
	return m_Animations[m_iCurrentAnimation]->Get_isFinished();
}

HRESULT CModel::NativeConstruct_Prototype(TYPE eMeshType,const char * pModelFilePath, const char * pModelFileName, _fmatrix PivotMatrix)
{
	char		szFullPath[MAX_PATH] = "";
	_uint		iFlag = 0;
	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	m_eType = eMeshType;
	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	if (TYPE::TYPE_NONANIM == m_eType)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_Triangulate;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_Triangulate;

	/* Importer : 메시파일로부터 데이터를 읽어서 assimp의 구조대로 정리하여 보관한다.  */
	m_pScene = m_Importer.ReadFile(szFullPath,iFlag);
	
	if (nullptr == m_pScene)
		return E_FAIL;

	if (FAILED(Create_MeshContainers()))
		return E_FAIL;

	if (FAILED(Create_Shader()))
		return E_FAIL;

	if (FAILED(Create_Materials(pModelFilePath)))
		return E_FAIL;



	return S_OK;
}

HRESULT CModel::NativeConstruct(void * pArg)
{
	if (TYPE::TYPE_ANIM == m_eType)
	{
		if (FAILED(Create_Hierachy(m_pScene->mRootNode,0)))
			return E_FAIL;

		sort(m_Hierachy_Node.begin(), m_Hierachy_Node.end(), [](CHierachy_Node* pSour, CHierachy_Node* pDest)
		{
			return pSour->Get_Depth() < pDest->Get_Depth();
		});

		if (FAILED(Setup_MeshBones()))
			return E_FAIL;

		if (FAILED(Create_AniMation()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Set_RawValue(const char * pConstantName, void * pData, _uint iSize)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(pData, 0, iSize);
}

HRESULT CModel::Set_TextureValue(const char * pConstantName, ID3D11ShaderResourceView * pSRV)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName)->AsShaderResource();
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetResource(pSRV);
}

CHierachy_Node * CModel::Finder_HierachyNode(const char * pNodeName)
{
	auto iter = find_if(m_Hierachy_Node.begin(), m_Hierachy_Node.end(), [&](CHierachy_Node* pNode)
	{
		return !strcmp(pNodeName, pNode->Get_Name());
	});

	if (iter == m_Hierachy_Node.end())
		return nullptr;

	return *iter;
}

HRESULT CModel::Create_MeshContainers()
{
	m_iNumMeshes = m_pScene->mNumMeshes;

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		aiMesh*		pAIMesh = m_pScene->mMeshes[i];
		if (nullptr == pAIMesh)
			return E_FAIL;

		CVIBuffer*	pMeshContainer = nullptr;

		if (TYPE_NONANIM == m_eType)
			pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDeviceContext, pAIMesh, pAIMesh->mMaterialIndex, XMLoadFloat4x4(&m_PivotMatrix));
		else
			pMeshContainer = CAnimMeshContainer::Create(m_pDevice, m_pDeviceContext,pAIMesh);


		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_MeshContainers.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Create_Shader()
{


	if (TYPE::TYPE_NONANIM == m_eType)
	{
		D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		if (FAILED(Compile_Shader(TEXT("../Bin/ShaderFiles/Shader_Model.hlsl"), Elements, 4)))
			return E_FAIL;
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		if (FAILED(Compile_Shader(TEXT("../Bin/ShaderFiles/Shader_AnimModel.hlsl"), Elements, 6)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Create_Materials(const char* pModelFilePath)
{
	m_iNumMaterials = m_pScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		aiMaterial*		pAIMaterial = m_pScene->mMaterials[i];
		MESHMATERIAL*	pMeshMaterial = new MESHMATERIAL;
		ZeroMemory(pMeshMaterial, sizeof(MESHMATERIAL));

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{


			aiString	strTextureFilePath;

			char		szFileName[MAX_PATH] = "";
			char		szEXT[MAX_PATH] = "";
			char		szFullPath[MAX_PATH] = "";

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strTextureFilePath)))
				continue;

			_splitpath_s(strTextureFilePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szEXT);

			_tchar		szTextureFullPath[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szTextureFullPath, MAX_PATH);

			pMeshMaterial->pTexture[j] = CTexture::Create(m_pDevice, m_pDeviceContext, szTextureFullPath);
			if (nullptr == pMeshMaterial->pTexture[j])
				return E_FAIL;
		}

		m_Materials.push_back(pMeshMaterial);
	}


	return S_OK;
}

HRESULT CModel::Create_Hierachy(aiNode * pNode, CHierachy_Node * pParent, _uint iDepth)
{
	_matrix			TransformationMatrix;
	memcpy(&TransformationMatrix, &pNode->mTransformation, sizeof(_matrix));

	CHierachy_Node*	pHierachyNode = CHierachy_Node::Create(pNode->mName.data, XMMatrixTranspose(TransformationMatrix), pParent, iDepth);
	if (nullptr == pHierachyNode)
		return E_FAIL;

	pHierachyNode->Resize_AnimBones(m_pScene->mNumAnimations);

	m_Hierachy_Node.push_back(pHierachyNode);

	_uint iNumChild = pNode->mNumChildren;

	for (_uint i = 0; i < iNumChild; ++i)
	{
		if (FAILED(Create_Hierachy(pNode->mChildren[i], pHierachyNode, iDepth + 1)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Create_AniMation()
{
	m_iNumAnimation = m_pScene->mNumAnimations;

	for (_uint i = 0; i < m_iNumAnimation; ++i)
	{
		aiAnimation* pAIAnimation = m_pScene->mAnimations[i];
		CAnimation* pAnimation = CAnimation::Create(pAIAnimation->mTicksPerSecond, pAIAnimation->mDuration);
		if (nullptr == pAnimation)
			return E_FAIL;

		for (_uint j = 0; j < pAIAnimation->mNumChannels; ++j)
		{
			aiNodeAnim*	pAIAnimBone = pAIAnimation->mChannels[j];
			CAnimBone*	pAnimBone = CAnimBone::Create();
			if (nullptr == pAnimBone)
				return E_FAIL;

			CHierachy_Node*	pHierachyNode = Finder_HierachyNode(pAIAnimBone->mNodeName.data);
			if (nullptr == pHierachyNode)
				return E_FAIL;

			pHierachyNode->Add_AnimBones(i, pAnimBone);

			_uint iNumKeyFrame = max(pAIAnimBone->mNumScalingKeys, pAIAnimBone->mNumRotationKeys);
			iNumKeyFrame = max(iNumKeyFrame, pAIAnimBone->mNumPositionKeys);

			_float3		vScale;
			_float4		vRotation;
			_float3		vPostion;

			for (_uint k = 0; k < iNumKeyFrame; ++k)
			{
				KEYFRAME*		pKeyFrame = new KEYFRAME;
				ZeroMemory(pKeyFrame, sizeof(KEYFRAME));

				if (pAIAnimBone->mNumScalingKeys > k)
				{
					memcpy(&vScale, &pAIAnimBone->mScalingKeys[k].mValue, sizeof(_float3));
					pKeyFrame->Time = pAIAnimBone->mScalingKeys[k].mTime;
				}
				if (pAIAnimBone->mNumRotationKeys > k)
				{
					vRotation.x = pAIAnimBone->mRotationKeys[k].mValue.x;
					vRotation.y = pAIAnimBone->mRotationKeys[k].mValue.y;
					vRotation.z = pAIAnimBone->mRotationKeys[k].mValue.z;
					vRotation.w = pAIAnimBone->mRotationKeys[k].mValue.w;
					pKeyFrame->Time = pAIAnimBone->mRotationKeys[k].mTime;
				}
				if (pAIAnimBone->mNumPositionKeys > k)
				{
					memcpy(&vPostion, &pAIAnimBone->mPositionKeys[k].mValue, sizeof(_float3));
					pKeyFrame->Time = pAIAnimBone->mPositionKeys[k].mTime;
				}

				pKeyFrame->vScale = vScale;
				pKeyFrame->vRotation = vRotation;
				pKeyFrame->vPosition = vPostion;

				pAnimBone->Add_KeyFrame(pKeyFrame);
			}
			pAnimation->Add_AnimBone(pAnimBone);
		}
		m_Animations.push_back(pAnimation);
	}

	return S_OK;
}

HRESULT CModel::Setup_MeshBones()
{
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		if (FAILED(dynamic_cast<CAnimMeshContainer*>(m_MeshContainers[i])->SetUp_MeshBones(this, m_pScene->mMeshes[i])))
			return E_FAIL;
	}
	//본을한번에모아서 한번에삭 정리해줄려고하는것이다
	return S_OK;
}

HRESULT CModel::Compile_Shader(const _tchar * pShaderFilePath, D3D11_INPUT_ELEMENT_DESC * pElements, _uint iNumElements)
{
	_uint			iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;

#endif // _DEBUG

	ID3DBlob*		pCodes = nullptr;

	/* 셰이더파일을 컴파일하여 메모리에 올리고. */
	if (FAILED(D3DCompileFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, nullptr, "fx_5_0", iFlag, 0, &pCodes, nullptr)))
		return E_FAIL;

	/* 올라온 메모리로부터 이펙트 객체를 생성한다. */
	if (FAILED(D3DX11CreateEffectFromMemory(pCodes->GetBufferPointer(), pCodes->GetBufferSize(), 0, m_pDevice, &m_pEffect)))
		return E_FAIL;

	ID3DX11EffectTechnique*		pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC		TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));

	if (FAILED(pTechnique->GetDesc(&TechniqueDesc)))
		return E_FAIL;

	for (_uint i = 0; i < TechniqueDesc.Passes; ++i)
	{
		PASSDESC			PassDesc;
		ZeroMemory(&PassDesc, sizeof(PASSDESC));

		PassDesc.pPass = pTechnique->GetPassByIndex(i);

		D3DX11_PASS_DESC		Pass;
		ZeroMemory(&Pass, sizeof(D3DX11_PASS_DESC));

		PassDesc.pPass->GetDesc(&Pass);

		if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElements, Pass.pIAInputSignature, Pass.IAInputSignatureSize, &PassDesc.pInputLayOut)))
			return E_FAIL;

		m_Passes.push_back(PassDesc);
	}

	return S_OK;
}

HRESULT CModel::Render(_uint iPassIndex)
{
	m_pDeviceContext->IASetInputLayout(m_Passes[iPassIndex].pInputLayOut);

	for (auto& pMeshContainer : m_MeshContainers)
	{
		_uint iMaterialIndex;

		if (TYPE_ANIM == m_eType)
		{
			_float4x4	BoneMatrices[266];
			ZeroMemory(BoneMatrices, sizeof(_float4x4) * 266);

			((CAnimMeshContainer*)pMeshContainer)->Get_BoneMatrices(BoneMatrices, XMLoadFloat4x4(&m_PivotMatrix));

			iMaterialIndex = ((CAnimMeshContainer*)pMeshContainer)->Get_MaterialIndex();

			if (FAILED(Set_RawValue("g_BoneMatrices", BoneMatrices, sizeof(_float4x4) * 266)))
				return E_FAIL;
		}
		else
			iMaterialIndex = ((CMeshContainer*)pMeshContainer)->Get_MaterialIndex();


		if (nullptr != m_Materials[iMaterialIndex]->pTexture[aiTextureType_DIFFUSE])
		{
			if (FAILED(Set_TextureValue("g_DiffuseTexture", m_Materials[iMaterialIndex]->pTexture[aiTextureType_DIFFUSE]->Get_ResourceView())))
				return E_FAIL;
		}
		if (nullptr != m_Materials[iMaterialIndex]->pTexture[aiTextureType_NORMALS])
		{
			if (FAILED(Set_TextureValue("g_NomalTexture", m_Materials[iMaterialIndex]->pTexture[aiTextureType_NORMALS]->Get_ResourceView())))
				return E_FAIL;
		}

		if (nullptr != m_Materials[iMaterialIndex]->pTexture[aiTextureType_EMISSIVE])
		{
			if (FAILED(Set_TextureValue("g_vMtrlEmissiveTexture", m_Materials[iMaterialIndex]->pTexture[aiTextureType_EMISSIVE]->Get_ResourceView())))
				return E_FAIL;
		}
		


		m_Passes[iPassIndex].pPass->Apply(0, m_pDeviceContext);

		pMeshContainer->Render(iPassIndex);
	}


	return S_OK;
}

HRESULT CModel::Update(_double TimeDelta, _bool* bEndCheck,_bool ReturnBackCheck,_double BlenderTime)
{
	if (m_iPreAnimation != m_iCurrentAnimation)
	{
		m_iGagu = m_iPreAnimation;
		m_bIsChange = true;
	}


	if (true == m_bIsChange)
		m_Animations[m_iCurrentAnimation]->Update_NextAnimBoneTransformationMatrix(TimeDelta, m_Animations[m_iGagu],this, BlenderTime);
	else
		m_Animations[m_iCurrentAnimation]->Update_AnimBoneTransformationMatrix(TimeDelta, ReturnBackCheck,bEndCheck);

	for (auto& pHierachyNode : m_Hierachy_Node)
		pHierachyNode->Update_CombinedTransformationMatrix(m_iCurrentAnimation);
	
	
	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, TYPE eMeshType, const char * pModelFilePath, const char * pModelFileName, _fmatrix PivotMatrix)
{
	CModel*	pInstance = new CModel(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype(eMeshType,pModelFilePath, pModelFileName, PivotMatrix)))
	{
		MSGBOX("Failed to Created CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*	pInstance = new CModel(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSGBOX("Failed to Created CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	if (false == m_bIsClone)
	{
		for (auto& pMaterial : m_Materials)
		{
			for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
				Safe_Release(pMaterial->pTexture[i]);

			Safe_Delete(pMaterial);
		}
		
		m_Materials.clear();
	}

	m_Materials.clear();
	vector<MESHMATERIAL*>().swap(m_Materials);

	for (auto& pMeshContainer : m_MeshContainers)
		Safe_Release(pMeshContainer);

	m_MeshContainers.clear();

	vector<class CVIBuffer*>().swap(m_MeshContainers);

	for (auto& pPass : m_Passes)
	{
		Safe_Release(pPass.pInputLayOut);
		Safe_Release(pPass.pPass);
	}
	m_Passes.clear();
	vector<PASSDESC>().swap(m_Passes);

	Safe_Release(m_pEffect);
	
	for (auto& Anmation : m_Animations)
	{
		Safe_Release(Anmation);
	}
	m_Animations.clear();
	vector<CAnimation*>().swap(m_Animations);

	for (auto& iter : m_Hierachy_Node)
	{
		Safe_Release(iter);
	}
	m_Hierachy_Node.clear();
	vector<CHierachy_Node*>().swap(m_Hierachy_Node);


	m_Importer.FreeScene();
	m_pScene = nullptr;

	__super::Free();
}
