#pragma once

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {
  class Models;

  class GA_CORE_EXPORT Plane
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    Plane() = default;
    
    ~Plane() = default;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

    /*
    * @brief Screen Aligned Quad.
    */
    void
    createSAQ();

    /*
    * @brief Screen Aligned Quad.
    */
    void
    setSAQ();

   private:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    SPtr<Models> m_SAQ;
  };
}