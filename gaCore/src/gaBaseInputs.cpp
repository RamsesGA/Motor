#include "gaBaseInputs.h"

namespace gaEngineSDK {
  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/
  BaseInputs&
  g_baseInputs() {
    return BaseInputs::instance();
  }
}