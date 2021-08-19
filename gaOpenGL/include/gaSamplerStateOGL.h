#pragma once

#include <gaSamplerState.h>

namespace gaEngineSDK {
  class SamplerStateOGL final : public SamplerState
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    SamplerStateOGL() = default;

    ~SamplerStateOGL() = default;
  protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief Member to store the generated sampler state.
    */
    uint32 m_samplerState = 0;

    friend class GraphicsApiOGL;
  };
}