#pragma once
#include <gaVector2.h>
#include <gaVector3.h>
#include <gaVector4.h>
#include <gaMatrix4x4.h>

#include "gaPrerequisitesCore.h"
#include "gaTextures.h"

using namespace gaEngineSDK;

/*****************************************************************************/
/**
* Structs
* 
* @brief The creation of structs necessary for the API is done.
*/
/*****************************************************************************/

const int32 MaxBones = 200;

struct ConstantBuffer1 {
	Matrix4x4 mView;
	Matrix4x4 mProjection;
};

struct ConstantBuffer2 {
	Matrix4x4 mWorld;
	Vector4 vMeshColor;
};

struct Matrices {
	Matrix4x4 World;
	Matrix4x4 Projection;
	Vector4   Color;
};

struct ViewCB {
	Matrix4x4 View;
};

struct Vertex {
	Vector4 position = { 0.0f, 0.0f, 0.0f, 1.0f };
	Vector4 normal = { 0.0f, 0.0f, 0.0f, 0.0f };
	Vector4 tangent = { 0.0f, 0.0f, 0.0f, 0.0f };
	Vector2 texCoords = { 0.0f, 0.0f };
	//Vector4 color = { 255.0f, 255.0f, 255.0f, 255.0f };
	float boneWeights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	uint32 boneIds[4] = { 0, 0, 0, 0 };
};

struct Texture {
	String type;
	String path;
	SPtr<Textures> texture;
};

struct ConstBuffBonesTransform {
	Matrix4x4 bonesTransform[MaxBones];
};

namespace FILTER {
	enum E {
		FILTER_MIN_MAG_MIP_POINT                          = 0,
		FILTER_MIN_MAG_POINT_MIP_LINEAR                   = 0x1,
		FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT             = 0x4,
		FILTER_MIN_POINT_MAG_MIP_LINEAR                   = 0x5,
		FILTER_MIN_LINEAR_MAG_MIP_POINT                   = 0x10,
		FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR            = 0x11,
		FILTER_MIN_MAG_LINEAR_MIP_POINT                   = 0x14,
		FILTER_MIN_MAG_MIP_LINEAR                         = 0x15,
		FILTER_ANISOTROPIC                                = 0x55,
		FILTER_COMPARISON_MIN_MAG_MIP_POINT               = 0x80,
		FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR        = 0x81,
		FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT  = 0x84,
		FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR        = 0x85,
		FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT        = 0x90,
		FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
		FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT        = 0x94,
		FILTER_COMPARISON_MIN_MAG_MIP_LINEAR              = 0x95,
		FILTER_COMPARISON_ANISOTROPIC                     = 0xd5
	};
}

namespace TEXTURE_ADDRESS_MODE {
	enum E {
		TEXTURE_ADDRESS_WRAP        = 1,
		TEXTURE_ADDRESS_MIRROR      = 2,
		TEXTURE_ADDRESS_CLAMP       = 3,
		TEXTURE_ADDRESS_BORDER      = 4,
		TEXTURE_ADDRESS_MIRROR_ONCE = 5
	};
}

namespace COMPARISON_FUNC {
	enum E {
		COMPARISON_NEVER         = 1,
		COMPARISON_LESS          = 2,
		COMPARISON_EQUAL         = 3,
		COMPARISON_LESS_EQUAL    = 4,
		COMPARISON_GREATER       = 5,
		COMPARISON_NOT_EQUAL     = 6,
		COMPARISON_GREATER_EQUAL = 7,
		COMPARISON_ALWAYS        = 8
	};
}

struct SAMPLER_DESC {
	FILTER::E               myFilter;
	TEXTURE_ADDRESS_MODE::E myAddressU;
	TEXTURE_ADDRESS_MODE::E myAddressV;
	TEXTURE_ADDRESS_MODE::E myAddressW;
	COMPARISON_FUNC::E      myComparisonFunc;
	uint32                  myMaxAnisotropy;
	float                   myMipLODBias;
	float                   myBorderColor[4];
	float                   myMinLOD;
	float                   myMaxLOD;
};