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

/*
* C O N S T A N T
* B U F F E R S
*/
struct ConstantBuffer1 {
  Matrix4x4 mView;
  Matrix4x4 mProjection;
};

struct ConstantBuffer2 {
  Matrix4x4 mWorld;
  Vector4 objectPosition;
};

struct cbSSAO {
  Vector2 mViewportDimensions;
  Vector2 mNothing;

  float mSample_radius;
  float mIntensity;
  float mScale;
  float mBias;
};

struct cbViewportDimension {
  Vector2 viewportDimensions;
  Vector2 mNothing;
};

struct cbLightning {
  float emissiveIntensity;
  float lightIntensity0;
  Vector3 lightPos0;
  Vector3 vViewPosition;
};

struct cbMipLevels {
  int32 mipLevel0;
  int32 mipLevel1;
  int32 mipLevel2;
  int32 mipLevel3;
};

struct ConstBuffBonesTransform {
  Matrix4x4 bonesTransform[maxBones];
};

//Shadow map
struct cbViewMatrixes {
  Matrix4x4 mView;
  Matrix4x4 mViewInv;
};

struct cbProjectionMatrixes {
  Matrix4x4 mProjection;
  Matrix4x4 mProjectionInv;
  Matrix4x4 mViewProjection;
  Matrix4x4 mViewProjectionInv;
};

struct cbWorldInfo {
  Matrix4x4 mWorld;
  Matrix4x4 mWorldInv;
  Matrix4x4 mWorldView;
  Matrix4x4 mWorldViewInv;
  Matrix4x4 mWorldProjection;
  Matrix4x4 mWorldProjectionInv;
  Matrix4x4 mWorldViewProjection;
  Matrix4x4 mWorldViewProjectionInv;

  Vector4 viewPosition;
};

struct myLight {
  Vector4 lDirection;
  Vector4 lColor;
  Vector3 lPosition;

  uint32 numberLights;
};

struct cbLight {
  myLight currentLight[5];
};

//-----------------------------------------------------------------------------
struct Matrices {
  Matrix4x4 World;
  Matrix4x4 Projection;
  Vector4   Color;
};

/*
* Structs for shaders
*/

struct ViewCB {
  Matrix4x4 View;
};

struct Vertex {
  Vector4 position     = { 0.0f, 0.0f, 0.0f, 1.0f };
  Vector3 normal       = { 0.0f, 0.0f, 0.0f };
  Vector3 tangent      = { 0.0f, 0.0f, 0.0f };
  Vector3 biTangent    = { 0.0f, 0.0f, 0.0f };
  Vector2 texCoords    = { 0.0f, 0.0f };
  float boneWeights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
  uint32 boneIds[4]    = { 0, 0, 0, 0 };
};

struct ssoaInput {
  Vector2 texCoord;
};

//Shadow map


//-----------------------------------------------------------------------------
namespace FILTER {
  enum E {
    kFilterMinMagMipPoint = 0,
    kFilterMinMagPointMipLinear = 0x1,
    kFilterMinPointMagLinearMipPoint = 0x4,
    kFilterMinPointMagMipLinear = 0x5,
    kFilterMinLinearMagMipPoint = 0x10,
    kFilterMinLinearMagPointMipLinear = 0x11,
    kFilterMinMagLinearMipPoint = 0x14,
    kFilterMinMagMipLinear = 0x15,
    kFilterAnisotropic = 0x55,
    kFilterComparisionMinMagMipPoint = 0x80,
    kFilterComparisionMinMagPointMipLinear = 0x81,
    kFilterComparisionMinPointMagLinearMipPoint = 0x84,
    kFilterComparisionMinPointMagMipLinear = 0x85,
    kFilterComparisionMinLinearMagMipPoint = 0x90,
    kFilterComparisionMinLinearMagPointMipLinear = 0x91,
    kFilterComparisionMinMagLinearMipPoint = 0x94,
    kFilterComparisionMinMagMipLinear = 0x95,
    kFilterComparisionAnisotropic = 0xd5
  };
}

namespace TEXTURE_ADDRESS_MODE {
  enum E {
    kTextureAddressWrap = 1,
    kTextureAddressMirror = 2,
    kTextureAddressClamp = 3,
    kTextureAddressBorder = 4,
    kTextureAddressMirrorOnce = 5
  };
}

namespace COMPARISON_FUNC {
  enum E {
    kComparisonNever = 1,
    kComparisonLess = 2,
    kComparisonEqual = 3,
    kComparisonLellEqual = 4,
    kComparisonGreater = 5,
    kComparisonNotEqual = 6,
    kComparisonGreaterEqual = 7,
    kComparisonAlways = 8
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