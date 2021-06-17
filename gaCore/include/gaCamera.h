#pragma once
#include <d3d11.h>
#include <gaVector2.h>
#include <gaVector3.h>
#include <gaMatrix4x4.h>
#include <SFML/Window/Event.hpp>

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {

  struct CameraDescriptor {
    uint32 camWidth = 0;
    uint32 camHeight = 0;

    float camNear = 0.0f;
    float camFar = 0.0f;
    float camFoV = 0.0f;

    Vector3 camEye = { 0.0f, 0.0f, 0.0f };
    Vector3 camLookAt = { 0.0f, 0.0f, 0.0f };
    Vector3 camUp = { 0.0f, 0.0f, 0.0f };
  };
  
  class GA_CORE_EXPORT Camera
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor
      */
      /***********************************************************************/
      Camera() = default;

      ~Camera() = default;

      /**
      * @brief Function to initialize class members.
      * @param Variable to obtain the information of the camera description.
      */
      void 
      init();
      
      /**
      * @brief Function to update our view matrix.
      */
      void 
      updateViewMatrix();

      /**
      * @brief Function to detect an input.
      * @param SFML event for input detection.
      */
      void 
      inputDetection(sf::Event param);

      /**
      * @brief Function to be able to apply a PITCH rotation.
      * @param SFML event for input detection.
      */
      void 
      pitchX(sf::Event param);

      /**
      * @brief Function to be able to apply a ROLL rotation.
      * @param SFML event for input detection.
      */
      void 
      rollZ(sf::Event param);
      
      /**
      * @brief Function to be able to apply a YAW rotation.
      * @param SFML event for input detection.
      */
      void 
      yawY(sf::Event param);

      /**
      * @brief Function to be able to apply a movement to the camera.
      * @param SFML event for input detection.
      */
      void 
      move(sf::Event param);
      
      /**
      * @brief Function to be able to apply a rotation of the view.
      */
      void 
      mouseRotation();

      /***********************************************************************/
      /**
      * Creates
      */
      /***********************************************************************/
      
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

      /***********************************************************************/
      /**
      * Sets
      */
      /***********************************************************************/

      /**
      * @brief Function to save the X, Y value of our mouse.
      */
      void 
      setOriginalMousePos(float x, float y);

      /**
      * @brief Function to save the bool value on a click or not.
      * @param Bool to know if you are pressing or not on the screen.
      */
      void 
      setClickPressed(bool isClicked);

      /**
      * @brief .
      */
      void
      setLookAt(Vector3 lookAt);

      /**
      * @brief .
      */
      void
      setEye(Vector3 eye);

      /**
      * @brief .
      */
      void
      setUp(Vector3 up);

      /**
      * @brief .
      */
      void
      setFar(float farCam = 30000.0f);

      /**
      * @brief .
      */
      void
      setNear(float nearCam = 0.01f);

      /**
      * @brief .
      */
      void
      setFoV(float fieldOfView = Math::FOV);
      
      /**
      * @brief .
      */
      void
      setHeight(float height);

      /**
      * @brief .
      */
      void
      setWidth(float width);

      /***********************************************************************/
      /**
      * Gets
      */
      /***********************************************************************/

      /**
      * @brief Function to get the view matrix.
      */
      Matrix4x4 
      getView();
      
      /**
      * @brief Function to obtain the projection matrix.
      */
      Matrix4x4 
      getProjection();
      
      /**
      * @brief Function to get the original position of the mouse.
      */
      Vector2 
      getOriginalMousePos();
     
      /**
      * @brief Function to obtain the boolean of detection of a click.
      */
      bool 
      getClickPressed();

    private:
      /***********************************************************************/
      /**
      * Members
      */
      /***********************************************************************/

      /**
      * @brief Member to detect and notify if user clicked.
      */
      bool m_clickPressed = false;

      /**
      * @brief Member to save angle.
      */
      float m_angule = 0.0f;

      /**
      * @brief Member to define a maximum angle that the user can rotate.
      */
      float m_maxAngule = 2160.0f;
     
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
      * @brief Member to save vectors.
      */
      Vector3 m_locketFront;

      /**
      * @brief Member to save vectors in motion X Y.
      */
      Vector3 m_onlyXnY;

      /**
      * @brief Member to save the X Y vectors and lock the camera.
      */
      Vector3 m_locketonlyXnY;

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