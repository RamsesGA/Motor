#pragma once

#include <gaVector3.h>
#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {

  struct CameraDescriptor {
    float width;
    float height;
    float near;
    float far;
    float foV;

    Vector3 eye;
    Vector3 lookAt;
    Vector3 up;
  };

  class Camera
  {
    public:
      /***********************************************************************/
      /**
      * Construcotr and destructor
      */
      /***********************************************************************/
      Camera() = default;

      virtual ~Camera() = default;

      /**
      * @brief Function to initialize class members.
      * @param .
      */
      void 
      init(CameraDescriptor cameraDesc,
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
      inputDetection(WPARAM param);

      /**
      * @brief Function to be able to apply a PITCH rotation.
      * @param .
      */
      void 
      pitchX(WPARAM param);

      /**
      * @brief Function to be able to apply a ROLL rotation.
      * @param .
      */
      void 
      rollZ(WPARAM param);
      
      /**
      * @brief Function to be able to apply a YAW rotation.
      * @param .
      */
      void 
      yawY(WPARAM param);

      /**
      * @brief Function to be able to apply a movement to the camera.
      * @param .
      */
      void 
      move(WPARAM param);
      
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
      * @brief Function to save WIDTH value.
      * @param .
      */
      void 
      setWidht(float width);

      /// <summary>
      /// 
      /// </summary>
      
      /**
      * @brief Function to save the HEIGHT value.
      * @param .
      */
      void 
      setHeight(float height);

      /// <summary>
      /// Función para guardar el valor NEAR
      /// </summary>
      void 
      setNear(float near);

      /// <summary>
      /// Función para guardar el valor FAR
      /// </summary>
      void 
      setFar(float far);

      /// <summary>
      /// Función para guardar el valor 
      /// Field Of View
      /// </summary>
      void 
      setFoV(float foV);

      /// <summary>
      /// Función para guardar el valor EYE
      /// </summary>
      void
      setEye(glm::vec3 vec3);

      /// <summary>
      /// Función para guardar el valor LOOK AT
      /// </summary>
      void 
      setLookAt(glm::vec3 vec3);

      /// <summary>
      /// Función para guardar el valor UP
      /// </summary>
      void 
      setUp(glm::vec3 vec3);

      /// <summary>
      /// Función para guardar el valor X, Y
      /// de nuestro mouse
      /// </summary>
      void 
      setOriginalMousePos(float x, float y);

      /// <summary>
      /// Función para guardar el valor bool
      /// sobre un click o no
      /// </summary>
      void 
      setClickPressed(bool bool);

      /***********************************************************************/
      /**
      * Gets
      */
      /***********************************************************************/

      /// <summary>
      /// Función para obtener la matríz de vista
      /// </summary>
      glm::mat4x4 GetView();
      /// <summary>
      /// Función para obtener la matríz de proyección
      /// </summary>
      glm::mat4x4 GetProjection();
      /// <summary>
      /// Función para obtener la posición original del mouse
      /// </summary>
      glm::vec2 GetOriginalMousePos();
      /// <summary>
      /// Función para obtener el booleano de detección de un click
      /// </summary>
      bool GetClickPressed();
  };
}