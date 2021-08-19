#pragma once

#include <gaIndexBuffer.h>

namespace gaEngineSDK {
  class IndexBufferOGL final : public IndexBuffer
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    IndexBufferOGL() = default;

    virtual 
    ~IndexBufferOGL() = default;

   protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief Member to save the index.
    */
    uint32 m_indexBufferObject;

    /**
    * @brief Member to store size.
    */
    uint32 m_indexBufferSize;

    friend class GraphicsApiOGL;
  };
}