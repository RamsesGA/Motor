#pragma once

#include <gaVector3.h>
#include <gaMatrix4x4.h>
#include <gaQuaternions.h>

#include "gaPrerequisitesCore.h"
#include "gaComponent.h"

namespace gaEngineSDK {
  class GA_CORE_EXPORT Transform final : public Component
  {
    public:
      /*************************************************************************/
      /**
      * Constructor and destructor.
      */
      /*************************************************************************/
      Transform();

      Transform(const Quaternions& inRotation, const Vector3& inTranslation,
                const Vector3& inScale3D);
    
      ~Transform() = default;
    
      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/

      Transform&
      operator=(const Transform& other);

      Transform
      operator+(const Transform& atom) const;

      Transform&
      operator+=(const Transform& atom);

      Transform
      operator*(float mult) const;

      Transform&
      operator*=(float mult);

      Transform
      operator*(const Transform& other) const;

      void
      operator*=(const Transform& other);

      Transform
      operator*(const Quaternions& other) const;

      void
      operator*=(const Quaternions& other);

      /*bool
      operator==(const Transform& other) const;

      bool
      operator!=(const Transform& other) const;*/


    private:
      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      Vector3 m_position;

      /*
      * @brief .
      */
      Quaternions m_rotation;

      /*
      * @brief .
      */
      Vector3 m_scale;

      friend class Actor;
  };
}