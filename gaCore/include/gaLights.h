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

    Lights() = default;

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
    setAmbientColor(Vector4 rgba);

    void
    setDiffuseColor(float r, float g, float b, float a);

    void
    setDiffuseColor(Vector4 rgba);

    void
    setPosition(float x, float y, float z);

    void
    setPosition(Vector3 xyz);

    void
    setLookAt(float x, float y, float z);

    void
    setLookAt(Vector3 xyz);

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