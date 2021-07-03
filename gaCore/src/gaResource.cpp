#include "gaResource.h"

namespace gaEngineSDK {

  void 
  Resource::setId(int32 id) {
    m_id = id;
  }

  void 
  Resource::setRSRCName(const String& name) {
    m_RSRCName.resize(name.size());
    std::memcpy(m_RSRCName.data(), name.data(), name.size());
  }

  int32
  Resource::getId() {
    return m_id;
  }

  String
  Resource::getRSRCName() {
    return m_RSRCName;
  }
}