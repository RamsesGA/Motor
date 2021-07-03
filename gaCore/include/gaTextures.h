#pragma once

#include "gaResource.h"

namespace gaEngineSDK {

  /**
  * @brief Enum with all numbers with the type of bind flag for DX.
  */
  namespace TEXTURE_BIND_FLAG {
    enum class E {
      kShaderResource = 0x8L,
      kRenderTarget = 0x20L,
      kDepthStencil = 0x40L,
      kUnorderedAccess = 0x80L,
    };
  }

  /**
  * @brief Enum with all numbers with format type for DX.
  */
  namespace TEXTURE_FORMAT {
    enum class E {
      kUnknown = 0,
      kR32G32B32A32Typeless = 1,
      kR32G32B32A32Float = 2,
      kR32G32B32A32UInt = 3,
      kR32G32B32A32SInt = 4,
      kR32G32B32Typeless = 5,
      kR32G32B32Float = 6,
      kR32G32B32UInt = 7,
      kR32G32B32SInt = 8,
      kR16G16B16A16Typeless = 9,
      kR16G16B16A16Float = 10,
      kR16G16B16A16UNorm = 11,
      kR16G16B16A16UInt = 12,
      kR16G16B16A16SNorm = 13,
      kR16G16B16A16SInt = 14,
      kR32G32Typeless = 15,
      kR32G32Float = 16,
      kR32G32UInt = 17,
      kR32G32SInt = 18,
      kR32G8X24Typeless = 19,
      kD32FloatS8X24UInt = 20,
      kR32FloatX8X24Typeless = 21,
      kX32TypelessG8X24UInt = 22,
      kR10G10B10A2Typeless = 23,
      kR10G10B10A2UNorm = 24,
      kR10G10B10A2UInt = 25,
      kR11G11B10Float = 26,
      kR8G8B8A8Typeless = 27,
      kR8G8B8A8UNorm = 28,
      kR8G8B8A8UNorm_SRGB = 29,
      kR8G8B8A8UInt = 30,
      kR8G8B8A8SNorm = 31,
      kR8G8B8A8SInt = 32,
      kR16G16Typeless = 33,
      kR16G16Float = 34,
      kR16G16UNorm = 35,
      kR16G16UInt = 36,
      kR16G16SNorm = 37,
      kR16G16SInt = 38,
      kR32Typeless = 39,
      kD32Float = 40,
      kR32Float = 41,
      kR32UInt = 42,
      kR32SInt = 43,
      kR24G8Typeless = 44,
      kD24UNormS8UInt = 45,
      kR24UNormX8Typeless = 46,
      kX24TypelessG8UInt = 47,
      kR8G8Typeless = 48,
      kR8G8UNorm = 49,
      kR8G8UInt = 50,
      kR8G8SNorm = 51,
      kR8G8SInt = 52,
      kR16Typeless = 53,
      kR16Float = 54,
      kD16UNorm = 55,
      kR16UNorm = 56,
      kR16UInt = 57,
      kR16SNorm = 58,
      kR16SInt = 59,
      kR8Typeless = 60,
      kR8UNorm = 61,
      kR8UInt = 62,
      kR8SNorm = 63,
      kR8SInt = 64,
      kA8UNorm = 65,
      kR1UNorm = 66,
      kR9G9B9E5SHAREDEXP = 67,
      kR8G8B8G8UNorm = 68,
      kG8R8G8B8UNorm = 69,
      kBC1Typeless = 70,
      kBC1UNorm = 71,
      kBC1UNorm_SRGB = 72,
      kBC2Typeless = 73,
      kBC2UNorm = 74,
      kBC2UNorm_SRGB = 75,
      kBC3Typeless = 76,
      kBC3UNorm = 77,
      kBC3UNormSRGB = 78,
      kBC4Typeless = 79,
      kBC4UNorm = 80,
      kBC4SNorm = 81,
      kBC5Typeless = 82,
      kBC5UNorm = 83,
      kBC5SNorm = 84,
      kB5G6R5UNorm = 85,
      kB5G5R5A1UNorm = 86,
      kB8G8R8A8UNorm = 87,
      kB8G8R8X8UNorm = 88,
      kR10G10B10XRBiasA2UNorm = 89,
      kB8G8R8A8Typeless = 90,
      kB8G8R8A8UNormSRGB = 91,
      kB8G8R8X8Typeless = 92,
      kB8G8R8X8UNormSRGB = 93,
      kBC6HTypeless = 94,
      kBC6HUF16 = 95,
      kBC6HSF16 = 96,
      kBC7Typeless = 97,
      kBC7UNorm = 98,
      kBC7UNormSRGB = 99,
    };
  }

	class Textures : public Resource
	{
	  public:
	  	/***********************************************************************/
	  	/**
	  	* Constructor and destructor.
	  	*/
	  	/***********************************************************************/
			Textures() = default;

      ~Textures() = default;

      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/

      virtual void*
      getTexture() { return nullptr; };
	};
}