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
  Resource::setRSRCName(const String& name) {
    m_RSRCName.resize(name.size());
    std::memcpy(m_RSRCName.data(), name.data(), name.size());
  }

  String
  Resource::getRSRCName() {
    return m_RSRCName;
  }
}