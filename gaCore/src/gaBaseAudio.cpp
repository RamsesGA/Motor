#include "gaBaseAudio.h"

namespace gaEngineSDK {
  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/
  BaseAudio&
  g_baseAudio() {
    return BaseAudio::instance();
  }
}