#pragma once

#include <cstdint>
#include "gaPlatformDefines.h"

namespace gaEngineSDK {
  using std::uint8_t;
  using std::uint16_t;
  using std::uint32_t;
  using std::uint64_t;
  using std::int8_t;
  using std::int16_t;
  using std::int32_t;
  using std::int64_t;
  
  /**
  * Basic unsigned types
  */
  using uint8 = uint8_t;  //8-bit unsigned
  using uint16 = uint16_t;  //16-bit unsigned
  using uint32 = uint32_t;  //32-bit unsigned
  using uint64 = uint64_t;  //64-bit unsigned
  
  /**
  * Basic signed types
  */
  using int8 = int8_t;  //8-bit signed
  using int16 = int16_t;  //16-bit signed
  using int32 = int32_t;  //32-bit signed
  using int64 = int64_t;  //64-bit signed
}