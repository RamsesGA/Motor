#pragma once

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {
  class RenderTarget
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    RenderTarget() = default;

    virtual
    ~RenderTarget() = default;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    virtual void*
    getRenderTexture(uint32 index = 0) { return nullptr; };

    /*
    * @brief .
    */
    virtual void*
    getDepthStencilView() { return nullptr; };

    /*
    * @brief .
    */
    uint32
    getID() {
      return m_id;
    };

   private:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    uint32 m_id = 0;

    /*
    * @brief .
    */
    uint32 m_mipLevel = 1;

    /*
    * @brief .
    */
    float m_scale = 0.0f;

    friend class GraphicsApiDX;
  };
}