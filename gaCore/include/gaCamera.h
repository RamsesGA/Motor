#pragma once
#include <d3d11.h>
#include <gaVector2.h>
#include <gaVector3.h>
#include <gaMatrix4x4.h>
#include <SFML/Window/Event.hpp>

#include "gaPrerequisitesCore.h"
#include "gaComponent.h"
#include "gaBaseInputs.h"

using sf::Event;

namespace gaEngineSDK {
  struct CameraDescriptor {
    uint32 camWidth = 0;
    uint32 camHeight = 0;

    float camNear = 0.0f;
    float camFar = 0.0f;
    float camFoV = 0.0f;

    Vector3 camEye;
    Vector3 camLookAt;
    Vector3 camUp;
  };

  class GA_CORE_EXPORT Camera : public Component
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor
    */
    /*************************************************************************/
    Camera() = default;

    ~Camera() = default;

    /**
    * @brief Function to initialize class members.
    */
    void
    startCamera();

    /**
    * @brief Function to update our view matrix.
    */
    void
    updateViewMatrix();

    /**
    * @brief Function to detect an input.
    * @param param, SFML event for input detection.
    * @param deltaTime, time elapsed since the last frame.
    */
    void
    inputDetection(KEYBOARD::E input, const float& deltaTime);

    /**
    * @brief Function to be able to apply a PITCH rotation.
    * @param param, SFML event for input detection.
    */
    void
    pitchX(KEYBOARD::E input, const float& deltaTime);

    /**
    * @brief Function to be able to apply a ROLL rotation.
    * @param param, SFML event for input detection.
    */
    void
    rollZ(KEYBOARD::E input, const float& deltaTime);

    /**
    * @brief Function to be able to apply a YAW rotation.
    * @param param, SFML event for input detection.
    */
    void
    yawY(KEYBOARD::E input, const float& deltaTime);

    /**
    * @brief Function to be able to apply a movement to the camera.
    * @param param, SFML event for input detection.
    * @param deltaTime, time elapsed since the last frame.
    */
    void
    move(KEYBOARD::E input, const float& deltaTime);

    /**
    * @brief Function to be able to apply a rotation of the view.
    */
    void
    mouseRotation(const float& deltaTime);

    /*
    * @brief .
    */
    void
    rotationAxis(const Vector2I& xy, const float& deltaTime);

    /*************************************************************************/
    /**
    * Creates
    */
    /*************************************************************************/

    /**
    * @brief Function to be able to generate a view.
    */
    void
    createView();

    /**
    * @brief Function to generate a projection matrix.
    */
    void
    createProjectionMatrix();

    /*************************************************************************/
    /**
    * Sets
    */
    /*************************************************************************/

    /**
    * @brief Function to save the X, Y value of our mouse.
    * @param x, mouse position on this coordinate.
    * @param y, mouse position on this coordinate.
    */
    void
    setOriginalMousePos(float x, float y);

    /**
    * @brief Save the values for the corresponding member.
    * @param lookAt, vector 3 to save position to member.
    */
    void
    setLookAt(Vector3 lookAt = { 0.0f, 1.0f, 0.0f });

    /**
    * @brief Save the values for the corresponding member.
    * @param eye, vector 3 to save position to member.
    */
    void
    setEye(Vector3 eye = { 0.0f, 0.0f, -500.0f });

    /**
    * @brief Save the values for the corresponding member.
    * @param up, vector 3 to save position to member.
    */
    void
    setUp(Vector3 up = { 0.0f, 1.0f, 0.0f });

    /**
    * @brief Save the values for the corresponding member.
    * @param farCam, floating with distant vision (has a default value)
    */
    void
    setFar(float farCam = 30000.0f);

    /**
    * @brief Save the values for the corresponding member.
    * @param nearCam, floating with near vision (has a default value).
    */
    void
    setNear(float nearCam = 0.01f);

    /**
    * @brief Save the values for the corresponding member.
    * @param fieldOfView, extent of the observable game world that is seen on the display.
    */
    void
    setFoV(float fieldOfView = Math::FOV);

    /**
    * @brief Save the values for the corresponding member.
    * @param height, camera position in y.
    */
    void
    setHeight(float height);

    /**
    * @brief Save the values for the corresponding member.
    * @param width, camera position in x.
    */
    void
    setWidth(float width);

    /*************************************************************************/
    /**
    * Gets
    */
    /*************************************************************************/

    /**
    * @brief Function to get the view matrix.
    * @return Matrix4x4 with the required information.
    */
    Matrix4x4
    getView();

    /**
    * @brief Function to obtain the projection matrix.
    * @return Matrix4x4 with the required information.
    */
    Matrix4x4
    getProjection();

    Matrix4x4
    getProjectionPerspective();

    /**
    * @brief Function to get the original position of the mouse.
    * @return Pointer position (x, y).
    */
    Vector2
    getOriginalMousePos();

    /*
    * @brief .
    */
    Vector3
    getCamEye();

    /*
    * @brief .
    */
    Vector3
    getLookAt();

   private:
    /*************************************************************************/
    /**
    * Members
    */
    /*************************************************************************/

    /**
    * @brief Member to save original mouse position.
    */
    Vector2 m_originalMousePos;

    /**
    * @brief Member to save the vectors above.
    */
    Vector3 m_up;

    /**
    * @brief Member to save the front vectors.
    */
    Vector3 m_front;

    /**
    * @brief Member to save the vectors of the right position.
    */
    Vector3 m_right;

    /**
    * @brief Member to store a projection matrix.
    */
    Matrix4x4 m_projection;

    /**
    * @brief Member to store a position matrix.
    */
    Matrix4x4 m_position;

    /**
    * @brief Member to store an axis matrix.
    */
    Matrix4x4 m_axis;

    /**
    * @brief Member to store a view array.
    */
    Matrix4x4 m_view;

    /**
    * @brief Member to connect to the structure and define its values.
    */
    CameraDescriptor m_cameraDesc;
  };
}