#pragma once

#include <gaTextures.h>

namespace gaEngineSDK {
  class TexturesOGL final : public Textures
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    TexturesOGL() = default;

    ~TexturesOGL() = default;

   protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief Member to save position information.
    */
    uint32 m_texture = 0;

    /**
    * @brief Member to save position information.
    */
    unsigned char m_data;

    /**
    * @brief It can be the render target view of DX.
    */
    uint32 m_framebuffer = 0;

    /**
    * @brief It can be the depth Stencil.
    */
    uint32 m_renderBufferObject = 0;

    friend class GraphicsApiOGL;
  };
}