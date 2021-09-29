#include "gaBasePhysics.h"

namespace gaEngineSDK {
  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/
  BasePhysics& 
  g_basePhysics() {
    return BasePhysics::instance();
  }
}