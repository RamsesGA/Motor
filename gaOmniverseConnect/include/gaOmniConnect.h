#pragma once

// Multi platform array size
#define HW_ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

#include <gaBaseOmniConnect.h>

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
#include <gaStaticMesh.h>

#include "gaPrerequisitesOmniConnect.h"

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

using std::unique_lock;
using std::mutex;
using std::cout;
using std::endl;
using std::to_string;

namespace gaEngineSDK {
  class OmniConnect final : public BaseOmniConnect
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
    * Omniverse virtual Methods.
    */
    /*************************************************************************/

    bool
    startOmniverse() override;

    String
    createOmniverseScene(const String& destinationPath, const String& stageName) override;

    void
    printConnectedUsername(const String& stageUrl) override;

    void
    createGeoMeshWithModel(WeakSPtr<Actor> model) override;

    bool
    openUSDFiles(String rute) override;

    /*************************************************************************/
    /**
    * Omniverse Methods.
    */
    /*************************************************************************/

    /*
    * @brief Create a GeoMesh with one local model and save it.
    */
    static UsdGeomMesh
    modelToGeoMesh(WeakSPtr<Actor> model);

    /*
    * @brief Create a default box and save the data and the stage in a ".usd".
    */
    static UsdGeomMesh
    createBox(int32 boxNumber = 0);

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
    * @brief Shut down Omniverse connection.
    */
    static void shutdownOmniverse();

    /*
    * @brief Omniverse Log callback.
    */
    static void 
    logCallback(const char* threadName,
                const char* component,
                OmniClientLogLevel level, const char* message);

    

    /*
    * @brief This function will add a commented checkpoint to a file on Nucleus if:
    *        Live mode is disabled (live checkpoints are ill-supported)
    *        The Nucleus server supports checkpoints
    */
    static void
    checkpointFile(const char* stageUrl, const char* comment);

    /*
    * @brief Check if the URL exists.
    */
    static bool
    isValidOmniURL(const String& maybeURL);

    /*
    * @brief Create a light in scene.
    */
    static void
    createLight();

    /*
    * @brief Load materials from Omniverse server.
    */
    static void
    uploadMaterial(const String& destinationPath);

    /*
    * @brief Bind the material to a geometry.
    */
    static void
    createMaterial(UsdGeomMesh meshIn);

    /*
    * @brief Creates a folder.
    */
    static void
    createEmptyFolder(const String & emptyFolderPath);

    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/
  private:

    /*
    * @brief Omniverse logging is noisy, only enable it if verbose mode (-v).
    */
    static bool m_omniverseLoggingEnabled;

    /*
    * @brief Check if Omniverse is initialized.
    */
    static bool m_isStartUp;

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

  /***************************************************************************/
  /**
  * Export.
  */
  /***************************************************************************/

  extern "C" GA_OMNICONNECT_EXPORT BaseOmniConnect *
  createOmniConnect() {
    OmniConnect* pOmniverse = new OmniConnect();
    return pOmniverse;
  }
}