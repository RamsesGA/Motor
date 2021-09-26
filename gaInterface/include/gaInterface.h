#pragma once

#include <gaBaseInterface.h>

#include "gaPrerequisitesInterface.h"

namespace gaEngineSDK {
  class Interface final : public BaseInterface
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    Interface() = default;

    ~Interface() = default;

    /*
    * @brief .
    */
    void
    init(uint32 width, uint32 height, void* hwndHandle) override;

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

    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    uint32 m_width = 0;

    /*
    * @brief .
    */
    uint32 m_height = 0;

    /*
    * @brief .
    */
    void* m_hwndHandle = nullptr;
  };

  /***************************************************************************/
  /**
  * Export.
  */
  /***************************************************************************/

  extern "C" GA_INTERFACE_EXPORT BaseInterface *
  createNewInterface() {
    Interface* pInterface = new Interface();
    return pInterface;
  }
}