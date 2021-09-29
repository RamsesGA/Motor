#pragma once

#include <gaVector3.h>
#include <gaVector4.h>
#include <gaMatrix4x4.h>

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {
  class GA_CORE_EXPORT Lights
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    Lights() = default;

    ~Lights() = default;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

    void
    generateViewMatrix();

    void
    generateProjectionMatrix(float screenDepth, float screenNear);

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

    void
    getViewMatrix(Matrix4x4& mView);

    void
    getProjectionMatrix(Matrix4x4& mProjection);

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
    Vector3 m_position;
    Vector3 m_lookAt;
    Matrix4x4 m_viewMatrix;
    Matrix4x4 m_projectionMatrix;
  };
}