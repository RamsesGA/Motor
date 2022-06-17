#pragma once

// Multi platform array size
#define HW_ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

//#include <gaBaseOmniConnect.h>

#include <OmniClient.h>
#include <OmniUsdLive.h>

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/mesh.h>
#include <pxr/usd/usdGeom/metrics.h>
#include <pxr/base/gf/matrix4f.h>
#include <pxr/base/gf/vec2f.h>
#include <pxr/usd/usdUtils/pipeline.h>
#include <pxr/usd/usdUtils/sparseValueWriter.h>
#include <pxr/usd/usdShade/material.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usdGeom/primvar.h>
#include <pxr/usd/usdShade/input.h>
#include <pxr/usd/usdShade/output.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdShade/materialBindingAPI.h>
#include <pxr/usd/usdLux/distantLight.h>
#include <pxr/usd/usdLux/domeLight.h>
#include <pxr/usd/usdShade/shader.h>
#include <pxr/usd/usd/modelAPI.h>

#include <gaActor.h>
#include <gaModels.h>
#include <gaLights.h>
#include <gaModule.h>
#include <gaDegrees.h>
#include <gaTransform.h>
#include <gaStaticMesh.h>
#include <gaSceneGraph.h>
#include <gaQuaternions.h>
#include <gaResourceManager.h>

#include "gaStdHeadersOmniverse.h"
#include "gaPrerequisitesCore.h"

PXR_NAMESPACE_USING_DIRECTIVE

// Private tokens for building up SdfPaths. We recommend
// constructing SdfPaths via tokens, as there is a performance
// cost to constructing them directly via strings (effectively,
// a table lookup per path element). Similarly, any API which
// takes a token as input should use a predefined token
// rather than one created on the fly from a string.
TF_DEFINE_PRIVATE_TOKENS(_tokens,
                         (box)
                         (DistantLight)
                         (DomeLight)
                         (Looks)
                         (Root)
                         (Shader)
                         (st)
                         //These tokens will be reworked or replaced by 
                         //the official MDL schema for USD.
                         (Material)
                         ((_module, "module"))
                         (name)
                         (out)
                         ((shaderId, "mdlMaterial"))
                         (mdl)
                         //Tokens used for USD Preview Surface
                         (diffuseColor)
                         (normal)
                         (file)
                         (result)
                         (varname)
                         (rgb)
                         (RAW)
                         (sRGB)
                         (surface)
                         (PrimST)
                         (UsdPreviewSurface)
                         ((UsdShaderId, "UsdPreviewSurface"))
                         ((PrimStShaderId, "UsdPrimvarReader_float2"))
                         (UsdUVTexture)
                         (model)
                         (mesh)
                         (obj));

namespace gaEngineSDK {
  using std::unique_lock;
  using std::mutex;
  using std::cout;
  using std::endl;
  using std::to_string;

  class GA_CORE_EXPORT OmniConnect : public Module<OmniConnect>
  {
  public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    OmniConnect() = default;

    ~OmniConnect() = default;

    /*************************************************************************/
    /**
    * Virtual Methods.
    */
    /*************************************************************************/

    static bool
    startOmniverse();

    static void
    shutdownOmniverse();

    static String
    createOmniverseScene(const String& destinationPath, const String& stageName);

    static void
    createGeoMeshWithModel(WeakSPtr<Actor> model);

    static bool
    loadUSDFiles(const String& rute);

    static bool
    openNewUSDFile(const String& rute);

    static void
    printConnectedUsername(const String& stageUrl);
    
    static void
    saveSceneGraphToUSD();

    static void
    saveObjectToUSD(WeakSPtr<SceneNode> child, String& name, String& parent);

    static void
    updateGaToOmniverse();

    static void
    updateOmniverseToGa();

    static void
    updateObjects(WeakSPtr<SceneNode> myNode);

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

    /*
    * @brief Check the connection status.
    */
    static void
    OmniClientConnectionStatusCallbackImpl(void* userData,
                                           const char* url,
                                           OmniClientConnectionStatus status) noexcept;

    /*
    * @brief Notify fails.
    */
    static void
    failNotify(const char* msg, const char* detail = nullptr, ...);

    /*
    * @brief Omniverse Log callback.
    */
    static void
    logCallback(const char* threadName,
                const char* component,
                OmniClientLogLevel level, const char* message);

    /*
    * @brief Check if the URL exists.
    */
    static bool
    isValidOmniURL(const String& maybeURL);

    /*
    * @brief This function will add a commented checkpoint to a file on Nucleus if:
    *        Live mode is disabled (live checkpoints are ill-supported)
    *        The Nucleus server supports checkpoints
    */
    static void
    checkpointFile(const char* stageUrl, const char* comment);
    
    /*
    * @brief Create a GeoMesh with one local model and save it.
    */
    static UsdGeomMesh
    modelToGeoMesh(WeakSPtr<Actor> model);

    /*
    * @brief .
    */
    static UsdGeomMesh
    modelToGeoMesh(WeakSPtr<Models> model, SdfPath& primitivePath);

    /*
    * @brief Load materials from Omniverse server.
    */
    static void
    uploadMaterial(const String& destinationPath);

    /*
    * @brief Create a default box and save the data and the stage in a ".usd".
    */
    static UsdGeomMesh
    createBox(int32 boxNumber = 0);

    /*
    * @brief Create a light in scene.
    */
    static void
    createLight();

    /*
    * @brief .
    */
    static UsdPrim
		createLightFromComponent(Lights* newLight, SdfPath& parentPath);

    /*
    * @brief .
    */
    static UsdPrim
    createDirectLight(Lights* newLight, SdfPath& parentPath);

    /*
    * @brief Creates a folder.
    */
    static void
    createEmptyFolder(const String & emptyFolderPath);

    /*
    * @brief Bind the material to a geometry.
    */
    static void
    createMaterial(UsdGeomMesh meshIn);

    /*
    * @brief .
    */
    static void
    setTransformComponents(UsdGeomXformable& xTransform,
                           Vector3& position,
                           Vector3& rotation,
                           Vector3& scale);

    /*
    * @brief .
    */
    static void
    setOp(UsdGeomXformable& xTransform,
          UsdGeomXformOp& op,
          UsdGeomXformOp::Type opType,
          const GfVec3d& value,
          const UsdGeomXformOp::Precision precision);

    /*
    * @brief .
    */
    static void
    getTransformComponents(UsdGeomXformable& usdXForm,
                           Vector3& position,
                           Vector3& rotation,
                           Vector3& scale);

    /*
    * @brief Check if Omniverse is initialized.
    */
    static bool m_isStartUp;

    /*
    * @brief Omniverse logging is noisy, only enable it if verbose mode (-v).
    */
    static bool m_omniverseLoggingEnabled;

    /*
    * @brief .
    */
    static bool m_isLiveSync;

    /*
    * @brief .
    */
    static String m_currentURL;

  private:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief Making the logging reasonable.
    */
    static mutex m_logMutex;

    /*
    * @brief Globals for Omniverse Connection and base Stage.
    */
    static UsdStageRefPtr m_stage;

    /*
    * @brief .
    */
    static SdfPath m_rootPrimPath;
  };
}