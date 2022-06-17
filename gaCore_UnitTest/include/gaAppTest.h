#pragma once

#include <gaMesh.h>
#include <gaCamera.h>
#include <gaBaseApp.h>
#include <gaVector4.h>
#include <gaMatrix4x4.h>
#include <gaStructures.h>
#include <gaGraphicsApi.h>
#include <gaSamplerState.h>
#include <gaRenderTarget.h>
#include <gaPrerequisitesCore.h>

namespace gaEngineSDK {
  class ResourceManager;
  class StaticMesh;
  class Plane;
}

class AppTest : public BaseApp
{
 public:
  /***************************************************************************/
  /*
  * Constructor and destructor.
  */
  /***************************************************************************/
  AppTest() = default;

  ~AppTest() = default;

  /***************************************************************************/
  /*
  * Inheritance.
  */
  /***************************************************************************/

  /*
  * @brief .
  */
  void
  onUpdate(float deltaTime) override;

  /*
  * @brief .
  */
  void
  onRender() override;

  /*
  * @brief .
  */
  void
  onCreate() override;
};