#pragma once

#include <gaModule.h>
#include <gaSceneGraph.h>
#include <gaGraphicsApi.h>
#include <gaResourceManager.h>

#include "gaPrerequisitesRender.h"

namespace gaEngineSDK {
  class GA_RENDER_EXPORT Renderer : public Module<Renderer>
  {
   public:
    /***********************************************************************/
    /**
    * Constructor and destructor.
    */
    /***********************************************************************/

    Renderer() = default;

    virtual
    ~Renderer() = default;

    /***********************************************************************/
    /**
    * Methods.
    */
    /***********************************************************************/

    /*
    * @brief .
    */
    void
    setObject(Renderer* api) {
      Renderer::_instance() = api;
    }

    /*
    * @brief .
    */
    virtual void
    init(void* wndHandle) { };

    /*
    * @brief .
    */
    virtual void
    update(const float& deltaTime) { };

    /*
    * @brief .
    */
    virtual void
    render() { };

    /*
    * @brief .
    */
    virtual void
    resize() { };
  };

  /***************************************************************************/
  /**
  * Methods.
  */
  /***************************************************************************/

  GA_RENDER_EXPORT Renderer&
  g_renderer();
}
