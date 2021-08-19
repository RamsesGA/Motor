#pragma once

#include <gaConstantBuffer.h>

namespace gaEngineSDK {
  class ConstantBufferOGL final : public ConstantBuffer
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    ConstantBufferOGL() = default;

    virtual
    ~ConstantBufferOGL() = default;

   protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief Member to store the index.
    */
    uint32 m_uniformBufferObject;

    /**
    * @brief Member to save the buffer size.
    */
    uint32 m_bufferSize;

    friend class GraphicsApiOGL;
  };
}