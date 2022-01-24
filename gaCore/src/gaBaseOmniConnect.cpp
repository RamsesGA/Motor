#include "gaBaseOmniConnect.h"

namespace gaEngineSDK {
  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/
  BaseOmniConnect&
  g_baseOmniConnect() {
    return BaseOmniConnect::instance();
  }
}