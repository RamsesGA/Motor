#include "gaResource.h"

namespace gaEngineSDK {

  void 
  Resource::setId(int32 id) {
    m_id = id;
  }

  int32 
  Resource::getId() {
    return m_id;
  }

  void 
  Resource::setRSRCName(String name) {
    m_RSRCName = name;
  }

  String
  Resource::getRSRCName() {
    return m_RSRCName;
  }

}