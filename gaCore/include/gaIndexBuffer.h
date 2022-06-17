#pragma once

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {
  class IndexBuffer
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    IndexBuffer() = default;

    virtual
    ~IndexBuffer() = default;

    uint32 m_index = 0;
  };
}