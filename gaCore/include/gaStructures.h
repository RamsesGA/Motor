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

const int32 maxBones = 200;

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
	float boneWeights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	uint32 boneIds[4] = { 0, 0, 0, 0 };
};

struct ConstBuffBonesTransform {
	Matrix4x4 bonesTransform[maxBones];
};

namespace FILTER {
	enum E {
		kFilterMinMagMipPoint                        = 0,
		kFilterMinMagPointMipLinear                  = 0x1,
		kFilterMinPointMagLinearMipPoint             = 0x4,
		kFilterMinPointMagMipLinear                  = 0x5,
		kFilterMinLinearMagMipPoint                  = 0x10,
		kFilterMinLinearMagPointMipLinear            = 0x11,
		kFilterMinMagLinearMipPoint                  = 0x14,
		kFilterMinMagMipLinear                       = 0x15,
		kFilterAnisotropic                           = 0x55,
		kFilterComparisionMinMagMipPoint             = 0x80,
		kFilterComparisionMinMagPointMipLinear       = 0x81,
		kFilterComparisionMinPointMagLinearMipPoint  = 0x84,
		kFilterComparisionMinPointMagMipLinear       = 0x85,
		kFilterComparisionMinLinearMagMipPoint       = 0x90,
		kFilterComparisionMinLinearMagPointMipLinear = 0x91,
		kFilterComparisionMinMagLinearMipPoint       = 0x94,
		kFilterComparisionMinMagMipLinear            = 0x95,
		kFilterComparisionAnisotropic                = 0xd5
	};
}

namespace TEXTURE_ADDRESS_MODE {
	enum E {
		kTextureAddressWrap       = 1,
		kTextureAddressMirror     = 2,
		kTextureAddressClamp      = 3,
		kTextureAddressBorder     = 4,
		kTextureAddressMirrorOnce = 5
	};
}

namespace COMPARISON_FUNC {
	enum E {
		kComparisonNever        = 1,
		kComparisonLess         = 2,
		kComparisonEqual        = 3,
		kComparisonLellEqual    = 4,
		kComparisonGreater      = 5,
		kComparisonNotEqual     = 6,
		kComparisonGreaterEqual = 7,
		kComparisonAlways       = 8
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