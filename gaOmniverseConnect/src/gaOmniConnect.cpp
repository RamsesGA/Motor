#include "gaOmniConnect.h"

namespace gaEngineSDK {
  bool OmniConnect::m_omniverseLoggingEnabled = false;
  bool OmniConnect::m_isStartUp = false;

  mutex OmniConnect::m_logMutex = mutex();
  UsdStageRefPtr OmniConnect::m_stage = nullptr;
  SdfPath OmniConnect::m_rootPrimPath = SdfPath();

  /***************************************************************************/
  /**
  * Omniverse virtual Methods.
  */
  /***************************************************************************/

  bool 
  OmniConnect::startOmniverse() {
    //Register a function to be called whenever 
    //the library wants to print something to a log
    omniClientSetLogCallback(logCallback);

    //The default log level is "Info", set it to "Debug" to see all messages
    omniClientSetLogLevel(eOmniClientLogLevel_Debug);

    //Initialize the library and pass it the version constant defined in OmniClient.h
    //This allows the library to verify it was built with a compatible version.
    //It will return false if there is a version mismatch.
    if (!omniClientInitialize(kOmniClientVersion)) {
      m_isStartUp = false;
      return false;
    }

    omniClientRegisterConnectionStatusCallback(nullptr, OmniClientConnectionStatusCallbackImpl);

    //Enable live updates.
    omniUsdLiveSetDefaultEnabled(true);

    m_isStartUp = true;
    return true;
  }

  String 
  OmniConnect::createOmniverseScene(const String& destinationPath,
                                    const String& sceneName) {
    String stageUrl = destinationPath + "/" + sceneName + ".usd";

    cout << "\ndestination path - - >" << stageUrl << endl;
    {
      //Delete the old version of this file on Omniverse and wait for the operation to complete
      unique_lock<mutex> lk(m_logMutex);
      cout << "\nWaiting for " << stageUrl << " to delete... " << endl;
    }
    omniClientWait(omniClientDelete(stageUrl.c_str(), nullptr, nullptr));
    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "\nFinished" << endl;
    }

    //Create this file in Omniverse cleanly
    m_stage = UsdStage::CreateNew(stageUrl);
    if (!m_stage) {
      failNotify("\nFailure to create model in Omniverse - -> ", stageUrl.c_str());
      return String();
    }

    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "\nNew stage created - -> " << stageUrl << endl;
    }

    //Always a good idea to declare your up-ness
    UsdGeomSetStageUpAxis(m_stage, UsdGeomTokens->y);
    return stageUrl;
  }

  void
  OmniConnect::printConnectedUsername(const String& stageUrl) {
    //Get the username for the connection.
    String userName("_none_");

    omniClientWait(omniClientGetServerInfo(stageUrl.c_str(), 
                                           &userName, 
      [](void* userData,
         OmniClientResult result,
         struct OmniClientServerInfo const* info) noexcept {
          String* userName = static_cast<String*>(userData);

          if (userData && userName && info && info->username) {
            userName->assign(info->username);
          }
      }));
    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "Connected username - -> " << userName << endl;
    }
  }

  /***************************************************************************/
  /**
  * Omniverse Methods.
  */
  /***************************************************************************/

  PXR_INTERNAL_NS::UsdGeomMesh 
  OmniConnect::createBox(int32 boxNumber) {
    //Create a simple box in USD with normals and UV information
    static double h = 50.0;
    static int32 gBoxVertexIndices[] = { 0, 1, 2, 1, 3, 2, 4, 5, 6, 4, 
                                         6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 
                                         14, 12, 14, 15, 16, 17, 18, 16, 18, 19,
                                         20, 21, 22, 20, 22, 23 };

    static double gBoxNormals[][3] = { {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, 
                                       {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1},
                                       {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0},
                                       {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0},
                                       {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0},
                                       {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0} };

    static double gBoxPoints[][3] = { {h, -h, -h}, {-h, -h, -h}, {h, h, -h}, {-h, h, -h}, 
                                      {h, h, h}, {-h, h, h}, {-h, -h, h}, {h, -h, h},
                                      {h, -h, h}, {-h, -h, h}, {-h, -h, -h}, {h, -h, -h},
                                      {h, h, h}, {h, -h, h}, {h, -h, -h}, {h, h, -h}, 
                                      {-h, h, h}, {h, h, h}, {h, h, -h}, {-h, h, -h},
                                      {-h, -h, h}, {-h, h, h}, {-h, h, -h}, {-h, -h, -h} };

    static float gBoxUV[][2] = { {0, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 0}, {0, 1}, {1, 1}, 
                                 {1, 0}, {0, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 0}, {0, 1},
                                 {1, 1}, {1, 0}, {0, 0}, {0, 1}, {1, 1}, {1, 0}, {0, 0}, 
                                 {0, 1}, {1, 1}, {1, 0} };

    //Create the geometry inside of "Root"
    String boxName("box_");
    boxName.append(to_string(boxNumber));

    SdfPath boxPrimPath = m_rootPrimPath.AppendChild(TfToken(boxName));//_tokens->box);
    UsdGeomMesh mesh = UsdGeomMesh::Define(m_stage, boxPrimPath);

    if (!mesh) {
      return mesh;
    }

    //Set orientation
    mesh.CreateOrientationAttr(VtValue(UsdGeomTokens->rightHanded));

    //Add all of the vertices
    int32 num_vertices = HW_ARRAY_COUNT(gBoxPoints);
    VtArray<GfVec3f> points;
    points.resize(num_vertices);

    for (int32 i = 0; i < num_vertices; ++i) {
      points[i] = GfVec3f(gBoxPoints[i][0], gBoxPoints[i][1], gBoxPoints[i][2]);
    }

    mesh.CreatePointsAttr(VtValue(points));

    //Calculate indices for each triangle
    // 2 Triangles per face * 3 Vertices per Triangle * 6 Faces
    int32 num_indices = HW_ARRAY_COUNT(gBoxVertexIndices);
    VtArray<int32> vecIndices;
    vecIndices.resize(num_indices);

    for (int32 i = 0; i < num_indices; ++i) {
      vecIndices[i] = gBoxVertexIndices[i];
    }

    mesh.CreateFaceVertexIndicesAttr(VtValue(vecIndices));

    //Add vertex normals
    int32 num_normals = HW_ARRAY_COUNT(gBoxNormals);
    VtArray<GfVec3f> meshNormals;
    meshNormals.resize(num_vertices);

    for (int32 i = 0; i < num_vertices; ++i) {
      meshNormals[i] = GfVec3f((float)gBoxNormals[i][0], 
                               (float)gBoxNormals[i][1], 
                               (float)gBoxNormals[i][2]);
    }

    mesh.CreateNormalsAttr(VtValue(meshNormals));

    //Add face vertex count
    VtArray<int32> faceVertexCounts;

    // 2 Triangles per face * 6 faces
    faceVertexCounts.resize(12);
    std::fill(faceVertexCounts.begin(), faceVertexCounts.end(), 3);
    mesh.CreateFaceVertexCountsAttr(VtValue(faceVertexCounts));

    //Set the color on the mesh
    UsdPrim meshPrim = mesh.GetPrim();
    UsdAttribute displayColorAttr = mesh.CreateDisplayColorAttr();

    {
      VtVec3fArray valueArray;
      GfVec3f rgbFace(0.463f, 0.725f, 0.0f);
      valueArray.push_back(rgbFace);
      displayColorAttr.Set(valueArray);
    }

    //Set the UV (st) values for this mesh
    UsdGeomPrimvar attr2 = mesh.CreatePrimvar(_tokens->st, 
                                              SdfValueTypeNames->TexCoord2fArray);

    {
      int32 uv_count = HW_ARRAY_COUNT(gBoxUV);
      VtVec2fArray valueArray;
      valueArray.resize(uv_count);
      for (int32 i = 0; i < uv_count; ++i)
      {
        valueArray[i].Set(gBoxUV[i]);
      }

      bool status = attr2.Set(valueArray);
    }
    attr2.SetInterpolation(UsdGeomTokens->vertex);

    //Commit the changes to the USD
    m_stage->Save();
    omniUsdLiveProcess();
    return mesh;
  }

  void 
  OmniConnect::OmniClientConnectionStatusCallbackImpl(void* userData, 
                                                      const char* url, 
                                                      OmniClientConnectionStatus status) noexcept {
    {
      unique_lock<mutex> lk(m_logMutex);
      
      cout << "Connection Status - -> " << 
               omniClientGetConnectionStatusString(status) << 
               " [" << url << "]" << endl;
    }

    //We shouldn't just exit here - 
    //we should clean up a bit, but we're going to do it anyway
    if (status == eOmniClientConnectionStatus_ConnectError) {
      cout << "[ERROR] Failed connection, exiting." << endl;
      exit(-1);
    }
  }

  void 
  OmniConnect::failNotify(const char* msg, const char* detail, ...) {
    unique_lock<mutex> lk(m_logMutex);

    fprintf(stderr, "%s\n", msg);

    if (nullptr != detail) {
      fprintf(stderr, "%s\n", detail);
    }
  }

  void
  OmniConnect::shutdownOmniverse() {
    //Calling this prior to shutdown ensures that all pending live updates complete.
    omniUsdLiveWaitForPendingUpdates();

    //The stage is a sophisticated object that needs to be destroyed properly.
    //Since m_stage is a smart pointer we can just reset it.
    m_stage.Reset();

    omniClientShutdown();

    m_isStartUp = false;
  }

  void 
  OmniConnect::logCallback(const char* threadName, 
                           const char* component, 
                           OmniClientLogLevel level, const char* message) {
    unique_lock<mutex> lk(m_logMutex);
    if (m_omniverseLoggingEnabled) {
      puts(message);
    }
  }

  void
  OmniConnect::checkpointFile(const char* stageUrl, const char* comment) {
    if (omniUsdLiveGetDefaultEnabled()) {
      return;
    }

    bool bCheckpointsSupported = false;

    omniClientWait(omniClientGetServerInfo(stageUrl, 
                                           &bCheckpointsSupported,
      [](void* UserData, 
         OmniClientResult Result,
         OmniClientServerInfo const* Info) noexcept {
        if ((Result == eOmniClientResult_Ok) && (Info && UserData)) {
          bool* bCheckpointsSupported = static_cast<bool*>(UserData);
          *bCheckpointsSupported = Info->checkpointsEnabled;
        }
      }));

    if (bCheckpointsSupported) {
      const bool bForceCheckpoint = true;
      omniClientWait(omniClientCreateCheckpoint(stageUrl, 
                                                comment, 
                                                bForceCheckpoint, 
                                                nullptr,
        [](void* userData, OmniClientResult result, char const* checkpointQuery) noexcept
        {}));
    }
  }

  bool 
  OmniConnect::isValidOmniURL(const String& maybeURL) {
    bool isValidURL = false;

    OmniClientUrl* url = omniClientBreakUrl(maybeURL.c_str());

    if (url->host && url->path && 
       (String(url->scheme) == String("Omniverse") || 
        String(url->scheme) == String("Omni"))) {
      isValidURL = true;
    }

    omniClientFreeUrl(url);

    if (!isValidURL) {
      cout << "This is not an Omniverse Nucleus URL: " << maybeURL.c_str() << endl;
      cout << "Correct Omniverse URL format is: " << endl;
      cout << "omniverse://server_name/Path/To/Example/Folder" << endl;
      cout << "Allowing program to continue because " << endl;
      cout << "file paths may be provided in the form: C:/Path/To/Stage" << endl;
    }
    return isValidURL;
  }

  void 
  OmniConnect::createLight() {
    //Construct /Root/Light path
    SdfPath lightPath = SdfPath::AbsoluteRootPath()
                                .AppendChild(_tokens->Root)
                                .AppendChild(_tokens->DistantLight);

    UsdLuxDistantLight newLight = UsdLuxDistantLight::Define(m_stage, lightPath);

    //Set the attributes
    newLight.CreateAngleAttr(VtValue(0.53f));
    GfVec3f color(1.0f, 1.0f, 0.745f);
    newLight.CreateColorAttr(VtValue(color));
    newLight.CreateIntensityAttr(VtValue(5000.0f));

    //Commit the changes to the USD
    m_stage->Save();
    omniUsdLiveProcess();
  }

  void 
  OmniConnect::uploadMaterial(const String& destinationPath) {
    String uriPath = destinationPath + "/Materials";

    //Delete the old version of this folder on Omniverse and wait 
    //for the operation to complete.
    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "Waiting for " << uriPath << " to delete...\n";
    }
    omniClientWait(omniClientDelete(uriPath.c_str(), nullptr, nullptr));
    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "finished" << endl;
    }

    //Upload the material folder (MDL and textures).
    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "Waiting for the resources/Materials folder to upload to ";
      cout << uriPath << " ... ";
    }
    omniClientWait(omniClientCopy("resources/Materials", 
                                  uriPath.c_str(),
                                  nullptr, 
                                  nullptr));
    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "finished" << endl;
    }
  }

  void 
  OmniConnect::createMaterial(UsdGeomMesh meshIn) {
    String materialName = "Fieldstone";

    //Create a material instance for this in USD.
    TfToken matNameToken(materialName);

    //Make path for "/Root/Looks/Fieldstone";
    SdfPath matPath = SdfPath::AbsoluteRootPath()
                              .AppendChild(_tokens->Root)
                              .AppendChild(_tokens->Looks)
                              .AppendChild(matNameToken);

    UsdShadeMaterial newMat = UsdShadeMaterial::Define(m_stage, matPath);

    //MDL Shader
    {
      //Create the MDL shader to bind to the material.
      SdfAssetPath mdlShaderModule = SdfAssetPath("./Materials/Fieldstone.mdl");
      SdfPath shaderPath = matPath.AppendChild(matNameToken);
      UsdShadeShader mdlShader = UsdShadeShader::Define(m_stage, shaderPath);

      mdlShader.CreateIdAttr(VtValue(_tokens->shaderId));

      //These attributes will be reworked or replaced in the official MDL schema for USD.
      mdlShader.SetSourceAsset(mdlShaderModule, _tokens->mdl);
      mdlShader.GetPrim().CreateAttribute(TfToken("info:mdl:sourceAsset:subIdentifier"), 
                                                  SdfValueTypeNames->Token, 
                                                  false, 
                                                  SdfVariabilityUniform).Set(
                                                  matNameToken);

      //Set the linkage between material and MDL shader.
      UsdShadeOutput mdlOutput = newMat.CreateSurfaceOutput(_tokens->mdl);
      mdlOutput.ConnectToSource(mdlShader, _tokens->out);
    }

    //USD Preview Surface Shaders.
    {
      //Create the "USD Primvar reader for float2" shader.
      SdfPath shaderPath = matPath.AppendChild(_tokens->PrimST);
      UsdShadeShader primStShader = UsdShadeShader::Define(m_stage, shaderPath);

      primStShader.CreateIdAttr(VtValue(_tokens->PrimStShaderId));
      primStShader.CreateOutput(_tokens->result, SdfValueTypeNames->Float2);
      primStShader.CreateInput(_tokens->varname, 
                               SdfValueTypeNames->Token).Set(_tokens->st);

      //Create the "Diffuse Color Tex" shader.
      String diffuseColorShaderName = materialName + "DiffuseColorTex";
      String diffuseFilePath = "./Materials/Fieldstone/Fieldstone_BaseColor.png";

      shaderPath = matPath.AppendChild(TfToken(diffuseColorShaderName));

      UsdShadeShader diffColorShader = UsdShadeShader::Define(m_stage, shaderPath);

      diffColorShader.CreateIdAttr(VtValue(_tokens->UsdUVTexture));

      UsdShadeInput texInput = diffColorShader.CreateInput(_tokens->file, 
                                                           SdfValueTypeNames->Asset);

      texInput.Set(SdfAssetPath(diffuseFilePath));
      texInput.GetAttr().SetColorSpace(_tokens->sRGB);
      diffColorShader.CreateInput(_tokens->st, 
                                  SdfValueTypeNames->Float2).ConnectToSource(
                                                             primStShader.CreateOutput(
                                                             _tokens->result, 
                                                             SdfValueTypeNames->Float2));

      UsdShadeOutput diffColorShaderOut = diffColorShader.CreateOutput(_tokens->rgb, 
                                                                       SdfValueTypeNames->Float3);

      //Create the "Normal Tex" shader.
      String normalShaderName = materialName + "NormalTex";
      String normalFilePath = "./Materials/Fieldstone/Fieldstone_N.png";

      shaderPath = matPath.AppendChild(TfToken(normalShaderName));

      UsdShadeShader normalShader = UsdShadeShader::Define(m_stage, shaderPath);

      normalShader.CreateIdAttr(VtValue(_tokens->UsdUVTexture));

      UsdShadeInput normalTexInput = normalShader.CreateInput(_tokens->file, 
                                                              SdfValueTypeNames->Asset);

      normalTexInput.Set(SdfAssetPath(normalFilePath));
      normalTexInput.GetAttr().SetColorSpace(_tokens->RAW);
      normalShader.CreateInput(_tokens->st, 
                               SdfValueTypeNames->Float2).ConnectToSource(
                                                          primStShader.CreateOutput(
                                                          _tokens->result, 
                                                          SdfValueTypeNames->Float2));

      UsdShadeOutput normShaderOut = normalShader.CreateOutput(_tokens->rgb, 
                                                               SdfValueTypeNames->Float3);

      //Create the USD Preview Surface shader.
      String usdPreviewSurfaceShaderName = materialName + "PreviewSurface";

      shaderPath = matPath.AppendChild(TfToken(usdPreviewSurfaceShaderName));

      UsdShadeShader usdPrevSurfaceShader = UsdShadeShader::Define(m_stage, 
                                                                      shaderPath);

      usdPrevSurfaceShader.CreateIdAttr(VtValue(_tokens->UsdPreviewSurface));

      UsdShadeInput diffColorInput = usdPrevSurfaceShader.CreateInput(_tokens->diffuseColor, 
                                                                      SdfValueTypeNames->Color3f);

      diffColorInput.ConnectToSource(diffColorShaderOut);

      UsdShadeInput normInput = usdPrevSurfaceShader.CreateInput(_tokens->normal, 
                                                                 SdfValueTypeNames->Normal3f);

      normInput.ConnectToSource(normShaderOut);

      //Set the linkage between material and USD Preview surface shader.
      UsdShadeOutput usdPreviewSurfaceOutput = newMat.CreateSurfaceOutput();
      usdPreviewSurfaceOutput.ConnectToSource(usdPrevSurfaceShader, _tokens->surface);
    }

    //Final step, associate the material with the face.
    UsdShadeMaterialBindingAPI usdMaterialBinding(meshIn);

    usdMaterialBinding.Bind(newMat);

    //Commit the changes to the USD.
    m_stage->Save();
    omniUsdLiveProcess();
  }

  void
  OmniConnect::createEmptyFolder(const String& emptyFolderPath) {
    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "Waiting to create a new folder: " << emptyFolderPath << " ... ";
    }

    OmniClientResult localResult;
    localResult = Count_eOmniClientResult;

    omniClientWait(omniClientCreateFolder(emptyFolderPath.c_str(), 
                                          &localResult, 
      [](void* userData, OmniClientResult result) noexcept {
        auto returnResult = static_cast<OmniClientResult*>(userData);
        *returnResult = result;
      }));

    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "finished [" << omniClientGetResultString(localResult) << "]" << endl;
    }
  }

}