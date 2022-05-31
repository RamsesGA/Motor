#pragma once

#include <gaModule.h>

#include "gaPrerequisitesCore.h"
#include "gaSceneGraph.h"
#include "gaGraphicsApi.h"
#include "gaResourceManager.h"

using sf::Event;
using sf::Vector2i;
using sf::Mouse;

namespace gaEngineSDK {
  class Shaders;
  class SamplerState;
  class RenderTarget;
  class ConstantBuffer;
  class Plane;
  class Lights;

  class GA_CORE_EXPORT BaseRenderer : public Module<BaseRenderer>
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    BaseRenderer() = default;

    virtual
    ~BaseRenderer() = default;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    setObject(BaseRenderer* api) {
      BaseRenderer::_instance() = api;
    }

    /*
    * @brief .
    */
    virtual void
    init(uint32 width, uint32 height) {};

    /*
    * @brief .
    */
    virtual void
    update(const float& deltaTime) {};

    /*
    * @brief .
    */
    virtual void
    render() {};

    /*
    * @brief .
    */
    virtual void
    resize() {};

    /*
    * @brief Indicates if the left mouse button is pressed.
    */
    virtual void
    onLeftMouseBtnDown() {};

    /*
    * @brief .
    */
    virtual void
    defaultCamera() {};

    /*
    * @brief .
    */
    virtual void
    updateLocalLight(Lights* newLight) {};
  };

  /***************************************************************************/
  /**
  * INSTANCE.
  */
  /***************************************************************************/

  GA_CORE_EXPORT BaseRenderer&
  g_baseRenderer();
}