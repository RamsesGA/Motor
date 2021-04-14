#pragma once
#include <d3d11.h>
#include <gaVector2.h>
#include <gaVector3.h>
#include <gaMatrix4x4.h>
#include <SFML/Window/Event.hpp>

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {

  namespace CameraDescriptor {
    struct GA_CORE_EXPORT E {
      float camWidth;
      float camHeight;
      float camNear;
      float camFar;
      float camFoV;

      Vector3 camEye;
      Vector3 camLookAt;
      Vector3 camUp;
    };
  }
  
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
      * @param .
      */
      void 
      init(CameraDescriptor::E cameraDesc,
           bool isOGL);
      
      /**
      * @brief Function to update our view matrix.
      */
      void 
      updateViewMatrix();

      /**
      * @brief Function to detect an input.
      * @param .
      */
      void 
      inputDetection(sf::Event param);

      /**
      * @brief Function to be able to apply a PITCH rotation.
      * @param .
      */
      void 
      pitchX(sf::Event param);

      /**
      * @brief Function to be able to apply a ROLL rotation.
      * @param .
      */
      void 
      rollZ(sf::Event param);
      
      /**
      * @brief Function to be able to apply a YAW rotation.
      * @param .
      */
      void 
      yawY(sf::Event param);

      /**
      * @brief Function to be able to apply a movement to the camera.
      * @param .
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
      * @param .
      */
      void 
      setOriginalMousePos(float x, float y);

      /**
      * @brief Function to save the bool value on a click or not.
      * @param .
      */
      void 
      setClickPressed(bool isClicked);

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
      * @brief Member to detect if it is OGL or DX.
      */
      bool m_isOGL;

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
      CameraDescriptor::E m_cameraDesc;
  };
}