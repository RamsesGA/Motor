#pragma once

#include <gaPixelShader.h>

namespace gaEngineSDK {
  class PixelShaderOGL final : public PixelShader
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    PixelShaderOGL() = default;

    ~PixelShaderOGL() = default;

   protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief Member to save position information.
    */
    uint32 m_fragmentShader;

    /**
    * @brief Member to link the program.
    */
    uint32_t m_rendererID;

    friend class GraphicsApiOGL;
  };
}