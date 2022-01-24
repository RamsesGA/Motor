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
const uint32 totalPixels = 256;

/*
* C O N S T A N T
* B U F F E R S
*/
struct cbCamera {
  Matrix4x4 mView;
  Matrix4x4 mProjection;
};

struct cbWorld {
  Matrix4x4 mWorld;
  Vector4 objectPosition;
};

struct cbSSAO {
  Vector2 mViewportDimensions;
  Vector2 mNothing;
};

struct cbSSAO2 {
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
  float lightIntensity;

  float lightPosX;
  float lightPosY;
  float lightPosZ;

  float vViewPositionX;
  float vViewPositionY;
  float vViewPositionZ;
};

struct cbMipLevels {
  int32 mipLevel0;
  int32 mipLevel1;
  int32 mipLevel2;
  int32 mipLevel3;
};

struct cbModelData {
  Matrix4x4 modelMatrix;
  Matrix4x4 bonesTransform[maxBones];
};

//Depth
struct cbMatrixBuffer {
  Matrix4x4 mWorld;
  Matrix4x4 mView;
  Matrix4x4 mProjection;
};

//shadow maps
struct cbShadows {
  Matrix4x4 mWorld;
  Matrix4x4 mView;
  Matrix4x4 mProjection;
  Matrix4x4 mLightView;
  Matrix4x4 mLightProjection;
};

struct cbInverse {
  Matrix4x4 mInverseViewCam;
  Matrix4x4 mInverseProjectionCam;
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
};

namespace TEXTURE_ADDRESS {
  enum E {
    kTextureAddressWrap = 1,
    kTextureAddressMirror = 2,
    kTextureAddressClamp = 3,
    kTextureAddressBorder = 4,
    kTextureAddressMirrorOnce = 5
  };
};

namespace COMPARISON {
  enum E {
    kComparisonNever = 1,
    kComparisonLess = 2,
    kComparisonEqual = 3,
    kComparisonLessEqual = 4,
    kComparisonGreater = 5,
    kComparisonNotEqual = 6,
    kComparisonGreaterEqual = 7,
    kComparisonAlways = 8
  };
};

namespace TEXTURE_BIND_FLAGS {
  enum E {
    kDefault = 0,
    kBindShaderResource  = 0x8L,
    kBindRenderTarget    = 0x20L,
    kBindDepthStencil    = 0x40L,
    kBindUnorderedAccess = 0x80L
  };
};

struct SAMPLER_DESC {
  FILTER::E          myFilter;
  TEXTURE_ADDRESS::E myAddressU;
  TEXTURE_ADDRESS::E myAddressV;
  TEXTURE_ADDRESS::E myAddressW;
  COMPARISON::E      myComparisonFunc;
  uint32             myMaxAnisotropy;
  float              myMipLODBias;
  float              myBorderColor[4];
  float              myMinLOD;
  float              myMaxLOD;
};

namespace CPU_ACCESS {
  enum E {
    kCpuAccessRead = 1,
    kCpuAccessWrite = 2
  };
};

namespace USAGE {
  enum E {
    kUsageDefault = 0,
    kUsageDynamic = 1,
    kUsageImmutable = 2,
    kUsageStaging = 3
  };
};