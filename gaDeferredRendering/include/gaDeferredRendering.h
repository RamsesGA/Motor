#pragma once

#include <gaBaseRenderer.h>

#include "gaPrerequisitesDefRend.h"

namespace gaEngineSDK {
  class DeferredRendering final : public BaseRenderer
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    DeferredRendering() = default;

    ~DeferredRendering() = default;

    /*************************************************************************/
    /**
    * Inheritance.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    init(uint32 width, uint32 height) override;

    /*
    * @brief .
    */
    void
    update(const float& deltaTime) override;

    /*
    * @brief .
    */
    void
    render() override;

    /*
    * @brief .
    */
    void
    resize() override;

    /*************************************************************************/
    /*
    * Passes.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    void
    RTGbufferPass() override;

    /*
    * @brief .
    */
    void
    RT_SSAO_Pass() override;

    /*
    * @brief .
    */
    void
    BlurH_Pass(void* texture) override;

    /*
    * @brief .
    */
    void
    BlurV_Pass(void* texture) override;

    /*
    * @brief .
    */
    void
    AdditionPass(void* texture1, void* texture2) override;

    /*
    * @brief .
    */
    void
    LightningPass() override;
  };

  /***************************************************************************/
  /**
  * Export.
  */
  /***************************************************************************/

  extern "C" GA_DEF_REND_EXPORT BaseRenderer *
  createBaseRenderer() {
    DeferredRendering* pDefRender = new DeferredRendering();
    return pDefRender;
  }
}