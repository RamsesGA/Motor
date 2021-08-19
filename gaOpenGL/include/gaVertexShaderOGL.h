#pragma once

#include <gaVertexShader.h>

namespace gaEngineSDK {
  class VertexShaderOGL final : public VertexShader
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    VertexShaderOGL() = default;
    
    ~VertexShaderOGL() = default;

   protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief Member to save position information
    */
    uint32 m_vertexShader;
    
    /**
    * @brief Member to link the program.
    */
    uint32_t m_rendererID;
    
    friend class GraphicsApiOGL;
  };
}