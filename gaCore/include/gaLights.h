#pragma once

#include <gaVector3.h>
#include <gaVector4.h>
#include <gaMatrix4x4.h>
#include <gaCamera.h>

#include "gaPrerequisitesCore.h"
#include "gaComponent.h"

namespace gaEngineSDK {
  using std::make_shared;
  using std::to_string;

  class Actor;

  class GA_CORE_EXPORT Lights : public Component
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    Lights(Vector3 position, Vector3 lookAt);

    ~Lights() = default;

    void
    update(const float& deltaTime) override;

    /*************************************************************************/
    /**
    * Sets.
    */
    /*************************************************************************/

    void
    setAmbientColor(float r, float g, float b, float a);

    void
    setDiffuseColor(float r, float g, float b, float a);

    void
    setPosition(float x, float y, float z);

    void
    setLookAt(float x, float y, float z);

    void
    setIntensity(float value);

    void
    setEmissiveIntensity(float value);

    /*************************************************************************/
    /**
    * Gets.
    */
    /*************************************************************************/

    Vector4
    getAmbientColor();

    Vector4
    getDiffuseColor();

    Vector3
    getPosition();

    Vector3
    getLookAt();

    Matrix4x4
    getViewMatrix();

    Matrix4x4
    getProjectionMatrix();

    float
    getIntensity();

    float
    getEmissiveIntensity();

   private:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/
    
    float m_intensity = 0.0f;
    float m_emissiveIntensity = 0.0f;

    Vector4 m_ambientColor;
    Vector4 m_diffuseColor;

    SPtr<Camera> m_pLightCamera;
  };
}