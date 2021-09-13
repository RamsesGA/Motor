#include "gaBaseInterface.h"

namespace gaEngineSDK {
  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/
  BaseInterface&
  g_baseInterface() {
    return BaseInterface::instance();
  }
}