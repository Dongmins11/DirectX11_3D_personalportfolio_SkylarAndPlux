#pragma once

namespace Engine
{	
	typedef struct tagVertexModel
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexUV;
		XMFLOAT3			vTangent;
	}VTXMODEL;

	typedef struct tagVertexAnimModel
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexUV;
		XMFLOAT3			vTangent;

		XMUINT4				vBlendIndex;
		XMFLOAT4			vBlendWeight;
	}VTXANIMMODEL;

	typedef struct tagVertexColiderCubeTexture
	{
		XMFLOAT3			vPosition;
	}VTXCOLIDERCUBETEX;

	typedef struct tagVertexCubeTexture
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vTexUV;
	}VTXCUBETEX;

	typedef struct tagVertexTexture
	{
		XMFLOAT3			vPosition;
		XMFLOAT2			vTexUV;
	}VTXTEX;

	typedef struct tagVertexNavi
	{
		XMFLOAT3			vPosition;
	}VTXTEXNAVI;

	typedef struct tagVertexColor
	{
		XMFLOAT3			vPosition;
		XMFLOAT4			vColor;
	}VTXCOL;

	typedef struct tagVertexNormalTexture
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexUV;
	}VTXNORTEX;

	typedef struct tagCubeVertexUv3Nomal
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT3		vTexUV;

	}CUBEVTXTEXNOMALUV;


	typedef struct tagVertexMatrix
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vPosition;
	}VTXMATRIX;

	typedef struct tagVertexPSize
	{
		XMFLOAT3			vPosition;
		float				fPSize;
	}VTXPOINT;

	typedef struct tagSphereVertexUv3Nomal
	{
		XMFLOAT3		vPosition;
	}SPHEREVTXTEXNOMALUV;

	typedef struct tagFaceIndices16
	{
		unsigned short		_1, _2, _3;
	}FACEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long		_1, _2, _3;
	}FACEINDICES32;

	typedef struct tagLineIndices16
	{
		unsigned short		_1, _2;
	}LINEINDICES16;

	typedef struct tagLineIndices32
	{
		unsigned long		_1, _2;
	}LINEINDICES32;

	typedef struct tagInputDesc
	{
		HINSTANCE			hInst;
		HWND				hWnd;
	} INPUTDESC;

	typedef struct tagMtrlDesc
	{
		_float4		vDiffuse;
		_float4		vAmbient;
		_float4		vSpecular;
		_float		fPower;
	}MTRLDESC;


	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_POINT, TYPE_DIRECTIONAL, TYPE_SPOT, TYPE_END };

		_uint		iType = TYPE_END;

		_float4		vPosition;
		_float		fRange = 0.f;

		_float4		vDirection;

		_float4		vDiffuse;
		_float4		vAmbient;
		_float4		vSpecular;
	}LIGHTDESC;


	typedef struct tagMaterial
	{
		class CTexture*		pTexture[AI_TEXTURE_TYPE_MAX];
	}MESHMATERIAL;

	typedef	struct tagPassDesc
	{
		ID3D11InputLayout*	pInputLayOut = nullptr;
		ID3DX11EffectPass*	pPass = nullptr;
	}PASSDESC;


	typedef struct tagModelPath
	{
		_uint			iType;
		_uint			viLevel;
		_float			vScaile;
		_float3			vScaileXYZ;
		_float3			vRotaition;
		_float4			vPos;
		wstring			wstrPrototypeTag;
		wstring			wstrPath;
		wstring			wstrObjKey;
	}MODELDESC;

	typedef struct tagModelPathClient
	{
		_uint			iType;
		_uint			viLevel;
		_float			vScaile;
		_float3			vScaileXYZ;
		_float3			vRotaition;
		_float4			vPos;
		_tchar*			wstrPrototypeTag;
		_tchar*			wstrPath;
		_tchar*			wstrObjKey;
	}MODELDESC_CLIENT;

	typedef struct tagKeyFrame
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vPosition;
		double			Time;
	}KEYFRAME;
}

