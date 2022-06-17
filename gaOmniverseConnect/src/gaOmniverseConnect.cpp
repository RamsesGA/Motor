#include <gaBaseRenderer.h>

#include "gaOmniverseConnect.h"

namespace gaEngineSDK {
  /***************************************************************************/
  /**
  * Inits.
  */
  /***************************************************************************/
  bool OmniverseConnect::m_isStartUp = false;
  bool OmniverseConnect::m_omniverseLoggingEnabled = false;
  bool OmniverseConnect::m_isLiveSync = false;

  String OmniverseConnect::m_currentURL = "";
  String OmniverseConnect::m_realPath = "";

  mutex OmniverseConnect::m_logMutex = mutex();

  UsdStageRefPtr OmniverseConnect::m_stage = nullptr;

  SdfPath OmniverseConnect::m_rootPrimPath = SdfPath();

  /***************************************************************************/
  /**
  * Local Methods.
  */
  /***************************************************************************/

  String
  checkPath(const String& folderPath, const String& stageName) {
    String realFolderPath;
    String realStageName;
    uint32 index = 0;

    bool hasPath = false;
    bool hasName = false;

    //realFolderPath
    while ('\0' != folderPath[index]) {
      realFolderPath.push_back(folderPath[index]);
      ++index;
      hasPath = true;
    }

    index = 0;

    //realStageName
    while ('\0' != stageName[index]) {
      realStageName.push_back(stageName[index]);
      ++index;
      hasName = true;
    }


    if (hasPath && hasName) {
      //In case the final part do not have a /
      if ('/' != realFolderPath.back()) {
        realFolderPath += "/";
      }

      //In case the .usd do not exist
      if (String::npos == realStageName.find(".usd")) {
        realStageName += ".usd";
      }

      return (realFolderPath + realStageName);
    }
    
    return "";
  }

  /***************************************************************************/
  /**
  * Virtual Methods.
  */
  /***************************************************************************/

  bool
  OmniverseConnect::startOmni() {
    return startOmniverse();
  }

  void
  OmniverseConnect::shutdownOmni() {
    shutdownOmniverse();
  }

  void
  OmniverseConnect::connectedUsername(const String& stageUrl) {
    printConnectedUsername(stageUrl);
  }

  String 
  OmniverseConnect::createOmniScene(const String& folderPath, const String& stageName) {
    return createOmniverseScene(folderPath, stageName);
  }

  bool 
  OmniverseConnect::openUSD(const String& folderPath, const String& stageName) {
    return openUSDFile(folderPath, stageName);
  }

  bool 
  OmniverseConnect::loadUSD() {
    return loadUSDFile();
  }

  void
  OmniverseConnect::saveSGToUSD() {
    saveSceneGraphToUSD();
  }

  void
  OmniverseConnect::updateGaToOmni() {
    updateGaToOmniverse();
  }

  void
  OmniverseConnect::updateOmniToGa() {
    updateOmniverseToGa();
  }

  /*
  * S E T
  * &
  * G E T
  *
  * Z O N E
  */

  bool 
  OmniverseConnect::getIsStartUp() {
    return m_isStartUp;
  }

  bool 
  OmniverseConnect::getOmniverseLogging() {
    return m_omniverseLoggingEnabled;
  }

  bool 
  OmniverseConnect::getIsLiveSync() {
    return m_isLiveSync;
  }

  String 
  OmniverseConnect::getCurrentURL() {
    return m_currentURL;
  }

  void 
  OmniverseConnect::setIsLiveSync(bool& isLive) {
    m_isLiveSync = isLive;

    //Enable live updates.
    omniUsdLiveSetDefaultEnabled(m_isLiveSync);
  }

  void 
  OmniverseConnect::setOmniverseLog(bool& omniverseLog) {
    m_omniverseLoggingEnabled = omniverseLog;
  }

  /***************************************************************************/
  /**
  * Methods.
  */
  /***************************************************************************/

  bool 
  OmniverseConnect::startOmniverse() {
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

    omniClientRegisterConnectionStatusCallback(nullptr, 
                                               OmniClientConnectionStatusCallbackImpl);
    m_isStartUp = true;
    return true;
  }

  void
  OmniverseConnect::shutdownOmniverse() {
    m_isStartUp = false;

    //Calling this prior to shutdown ensures that all pending live updates complete.
    omniUsdLiveWaitForPendingUpdates();

    //The stage is a sophisticated object that needs to be destroyed properly.
    //Since m_stage is a smart pointer we can just reset it.
    m_stage.Reset();

    omniClientShutdown();
  }

  String 
  OmniverseConnect::createOmniverseScene(const String& folderPath, 
                                         const String& stageName) {
    
    m_realPath = checkPath(folderPath, stageName);

    if ("" == m_realPath) {
      return "";
    }

    cout << "\nDestination path -> " << m_realPath + "\n";
    {
      //Delete the old version of this file on Omniverse 
      //and wait for the operation to complete
      unique_lock<mutex> lk(m_logMutex);
      cout << "\nWaiting for " << m_realPath << " to delete... " << endl;
    }

    omniClientWait(omniClientDelete(m_realPath.c_str(), nullptr, nullptr));
    
    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "\nFinished" << endl;
    }

    //Create this file in Omniverse cleanly
    m_stage = UsdStage::CreateNew(m_realPath);
    if (!m_stage) {
      failNotify("\nFailure to create model in Omniverse - -> ", m_realPath.c_str());
      return String();
    }

    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "\nNew stage created - -> " << m_realPath << endl;
    }

    //Always a good idea to declare your up-ness
    UsdGeomSetStageUpAxis(m_stage, UsdGeomTokens->y);

    m_stage->Save();
    omniUsdLiveProcess();

    return m_realPath;
  }

  bool 
  OmniverseConnect::openUSDFile(const String& folderPath, const String& stageName) {
    m_realPath = checkPath(folderPath, stageName);

    if ("" == m_realPath) {
      return false;
    }

    cout << "\nPath to access -> " << m_realPath + "\n";

    m_stage = UsdStage::Open(m_realPath);

    if (!(m_stage)) {
      failNotify("Error, can not open the USD stage on path: ", m_realPath.c_str());
      return false;
    }

    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "\nStage opened success on path --> " << m_realPath << endl;
    }

    if (UsdGeomTokens->y != UsdGeomGetStageUpAxis(m_stage)) {
      unique_lock<mutex> lk(m_logMutex);
      cout << "\nStage is not Y-up so live xform edits will be incorrect. Stage is "
           << UsdGeomGetStageUpAxis(m_stage) << "-up\n";
    }

    m_rootPrimPath = SdfPath::AbsoluteRootPath().AppendChild(_tokens->Root);

    return true;
  }

  bool
  OmniverseConnect::loadUSDFile() {
    if (nullptr == m_stage) {
      failNotify("\nFailure to open stage in Omniverse: ", m_realPath.c_str());
      return false;
    }

    auto mySceneGraph = SceneGraph::instancePtr();
    //mySceneGraph->clearSceneGraph();
    
    SdfPath rootPrimPath = SdfPath::AbsoluteRootPath().AppendChild(_tokens->Root);
    
    auto rootScene = mySceneGraph->m_root;
    rootScene->m_sdfPathOmni = rootPrimPath.GetString();
    
    auto range = m_stage->Traverse();
    for (const auto& node : range) {
      if (node.IsA<UsdGeomMesh>()) {
        UsdPrim parent = node.GetParent();
    
        cout << "\nParent name -> " << parent.GetName() << "\n";

        if ("Root" == parent.GetName()) {
          unique_lock<mutex> lk(m_logMutex);
    
          cout << "\nFound UsdGeoMesh -> " << node.GetName() << ".\n";

          UsdGeomMesh localGeoMesh(node);
          if (!(node.GetAllChildren().empty())) {
            for (const auto& childNode : node.GetAllChildren()) {
              if (childNode.IsA<UsdGeomSubset>()) {
                assignModelInfo(node.GetName(), localGeoMesh);
              }
              else if (childNode.IsA<UsdGeomMesh>()){
                UsdGeomMesh childGM(childNode);
                assignModelInfo(node.GetName(), childGM);
              }
            }
          }
          else {
            assignModelInfo(node.GetName(), localGeoMesh);
          }
        }
      }
    }
  }

  void
  OmniverseConnect::assignModelInfo(const String& name, const UsdGeomMesh& geomMesh) {
    VtArray<GfVec3f> points;
    VtArray<GfVec3f> normals;
    VtArray<int32> indices;

    VtVec2fArray uvs;
    
    UsdAttribute meshPointAttrib = geomMesh.GetPointsAttr();
    meshPointAttrib.Get(&points);

    UsdAttribute meshIndexAttrib = geomMesh.GetFaceVertexIndicesAttr();
    meshIndexAttrib.Get(&indices);

    UsdAttribute meshNormalAttrib = geomMesh.GetNormalsAttr();
    meshNormalAttrib.Get(&normals);

    uint32 numPoints = points.size();
    uint32 numIndices = indices.size();

    Vector<Vertex> meshVertices;
    Vector<uint32> meshIndices;

    //Assigning model information.
    for (uint32 i = 0; i < numPoints; ++i) {
      Vertex v;

      v.position = Vector4(points[i].data()[0],
        points[i].data()[1],
        points[i].data()[2]);

      v.normal = Vector3(normals[i].data()[0],
        normals[i].data()[1],
        normals[i].data()[2]);

      v.tangent = Vector3(1.0f, 1.0f, 1.0f);

      v.biTangent = Vector3(1.0f, 1.0f, 1.0f);

      if (nullptr != uvs.data()) {
        v.texCoords = Vector2(uvs[i].data()[0], uvs[i].data()[1]);
      }
      else {
        v.texCoords = Vector2(1.0f, 1.0f);
      }

      meshVertices.push_back(v);
    }

    for (uint32 i = 0; i < numIndices; ++i) {
      meshIndices.push_back(indices[i]);
    }

    //Defining
    Vector<Textures*> vTextures;

    auto myRSRCMG = ResourceManager::instancePtr();

    SPtr<Materials> myMaterial = myRSRCMG->load<Materials>
                                 ("data/textures/lost/missTexture.jpg",
                                  TYPE_TEXTURES::kAlbedo);
    if (nullptr != myMaterial) {
      vTextures.push_back(myMaterial->m_pTexture);
    }

    SPtr<Models> myModel = make_shared<Models>();
    SPtr<Mesh> localMesh = make_shared<Mesh>();

    localMesh = myModel->createVertexData(meshVertices.data(), meshVertices.size());
    localMesh->setUpMesh(meshVertices, meshIndices, vTextures);

    Vector<WeakSPtr<Mesh>> vMeshes;
    vMeshes.push_back(localMesh);

    myModel->addNewMesh(vMeshes);

    /*
    * M A T E R I A L
    * Z O N E
    * T O
    * M E S H E S
    */
    uint32 sizeMeshes = myModel->getSizeMeshes();
    uint32 sizeTextures = vTextures.size();
    for (uint32 i = 0; i < sizeMeshes; ++i) {
      for (uint32 j = 0; j < sizeTextures; ++j) {
        uint32 sizeMeshTex = myModel->getMesh(i)->m_vTextures.size();

        if (sizeTextures == sizeMeshTex) {
          myModel->getMesh(i)->m_vTextures[j] = vTextures[j];
        }
      }
    }

    SPtr<StaticMesh> myStaticMesh = make_shared<StaticMesh>();
    myStaticMesh->m_pModel = myModel;

    //Creating the component
    SPtr<Component> newComponent(myStaticMesh);

    //Creating actor
    SPtr<Actor> actor = make_shared<Actor>();
    actor->init(name);
    actor->setIsSelected(true);
    actor->setComponent(newComponent);

    auto mySceneGraph = SceneGraph::instancePtr();

    //Adding the actor to node root
    auto localNewNode = mySceneGraph->createNewActor(actor, SPtr<SceneNode>(nullptr));

    localNewNode->m_sdfPathOmni = geomMesh.GetPath().GetString();
  }

  /*
  * S A V E
  * Z O N E
  */

  void
  OmniverseConnect::saveSceneGraphToUSD() {
    //In case that is not initialize.
    if (!(m_isStartUp) || !(m_stage)) {
      return;
    }

    omniUsdLiveWaitForPendingUpdates();

    //Clean the actual root.
    m_stage->GetRootLayer()->ClearDefaultPrim();

    //Obtain the true root of the scene.
    SdfPath rootPrimitivePath = SdfPath::AbsoluteRootPath().AppendChild(_tokens->Root);

    m_rootPrimPath = rootPrimitivePath;

    //Create the transform for the root.
    UsdGeomXform::Define(m_stage, m_rootPrimPath);

    auto mySceneGraph = SceneGraph::instancePtr();
    auto actualRoot = mySceneGraph->m_root;

    //Define the SDF path to the actual root and actualize the root primitive path.
    actualRoot->m_sdfPathOmni = rootPrimitivePath.GetString();

    uint32 childNums = 0;
    for (auto child : actualRoot->getChildNodes()) {
      String localName = child->getActorNode()->getActorName();
      String childName(localName);

      //String childName("model_");
      //childName.append(to_string(++childNums));

      SdfPath modelPath = rootPrimitivePath.AppendChild(TfToken(childName));

      saveObjectToUSD(child, childName, actualRoot->m_sdfPathOmni);
    }

    m_stage->Save();
    omniUsdLiveProcess();
  }

  void
  OmniverseConnect::saveObjectToUSD(WeakSPtr<SceneNode> child, 
                                    String& name, 
                                    String& parent) {
    auto localChild = child.lock();
    if (("" == name) || ("" == parent) || (nullptr == localChild)) {
      cout << "\nError save object, name, parent or localChild are empty\n";
      return;
    }

    omniUsdLiveWaitForPendingUpdates();

    //Convert String to SdfPath.
    SdfPath parentPath = SdfPath(parent);
    SdfPath primitivePath = parentPath.AppendChild(TfToken(name));

    localChild->m_sdfPathOmni = primitivePath.GetString();

    auto getCompo = localChild->getActorNode()->getComponent<Transform>();
    auto getModel = localChild->getActorNode()->getComponent<StaticMesh>();

    if ((nullptr != getModel) || (nullptr != getModel->m_pModel)) {
      auto localUsdMesh = modelToGeoMesh(getModel->m_pModel, primitivePath);

      UsdGeomXformable xTransform(localUsdMesh);

      Vector3 tempPosition = getCompo->getPosition();
      Vector3 tempRotation = getCompo->getEulerRotation();
      Vector3 tempScale = getCompo->getScale();

      setTransformComponents(xTransform, tempPosition, tempRotation, tempScale);
    }

    uint32 numChild = 0;
    for (auto forChild : localChild->getChildNodes()) {
      //String localName = forChild->getActorNode()->getActorName();
      String childName("mesh_");

      childName.append(to_string(++numChild));

      String tempStr = primitivePath.GetString();

      saveObjectToUSD(forChild, childName, tempStr);
    }
  }

  /*
  * U P D A T E
  * Z O N E
  */

  void
  OmniverseConnect::updateGaToOmniverse() {
    //In case the stage is null.
    if (!(m_isStartUp) || !(m_stage)) {
      return;
    }

    omniUsdLiveWaitForPendingUpdates();

    auto mySceneGraph = SceneGraph::instancePtr();

    //Need to access to my local scene graph.
    auto node = mySceneGraph->m_nodeSelected;

    if (!(node)) {
      return;
    }

    //Check if the selected node is not the root.
    if (node != mySceneGraph->m_root) {

      //Check if exist in Omniverse in this moment.
      SdfPath localPath = SdfPath(node->m_sdfPathOmni);

      //If the path is not empty.
      if (!(localPath.IsEmpty())) {
        auto primitive = m_stage->GetPrimAtPath(localPath);

        //Looking for a Geo mesh model.
        if ((primitive) && (primitive.IsA<UsdGeomMesh>())) {
          UsdGeomMesh localMesh(primitive);

          auto localComponent = node->getActorNode()->getComponent<Transform>();

          if (nullptr != localComponent) {
            UsdGeomXformable usdXForm(localMesh);

            Vector3 localPosition = localComponent->getPosition();
            Vector3 localRotation = localComponent->getEulerRotation();
            Vector3 localScale = localComponent->getScale();
            
            setTransformComponents(usdXForm, localPosition, localRotation, localScale);
          }
        }
      }
    }

    //Commit the changes to the USD
    m_stage->Save();
    omniUsdLiveProcess();
  }

  void 
  OmniverseConnect::updateOmniverseToGa() {
    //In case the stage is null.
    if (!(m_isStartUp) || !(m_stage)) {
      return;
    }

    auto mySceneGraph = SceneGraph::instancePtr();
    updateObjects(mySceneGraph->m_root);

    m_stage->Save();
    omniUsdLiveProcess();
  }

  void 
  OmniverseConnect::updateObjects(WeakSPtr<SceneNode> myNode) {
    auto localNode = myNode.lock();
    
    SdfPath localPath = SdfPath(localNode->m_sdfPathOmni);
    
    if (!(localPath.IsEmpty())) {
      auto primitive = m_stage->GetPrimAtPath(localPath);
      if (primitive) {
        UsdGeomMesh localMesh(primitive);
    
        auto localTransform = localNode->getActorNode()->getComponent<Transform>();
    
        if (nullptr != localTransform) {
          Vector3 pos;
          Vector3 rot;
          Vector3 scale;

          if (primitive.IsA<UsdGeomMesh>()) {
            UsdGeomMesh geoMesh = localMesh;

            getTransformComponents(geoMesh, pos, rot, scale);
          }
    
          localTransform->setPosition(pos);
          
          localTransform->setEulerRotation(rot);

          localTransform->setScale(scale);
          
          //localTransform->update(0.0f);
        }
      }
    }
    
    for (auto childNode : localNode->getChildNodes()) {
      updateObjects(childNode);
    }
  }

  /*
  * M O R E
  * 
  * M E T H O D S
  */

  UsdGeomMesh 
  OmniverseConnect::modelToGeoMesh(WeakSPtr<Actor> model) {
    auto localModel = model.lock();

    //Set the model token
    String modelName("model_");

    modelName.append(to_string(1));

    //_tokens->box);
    SdfPath modelPrimPath = m_rootPrimPath.AppendChild(TfToken(modelName));

    //Define the path in the stage
    UsdGeomMesh localGeoMesh = UsdGeomMesh::Define(m_stage, modelPrimPath);

    if (!localGeoMesh) {
      return localGeoMesh;
    }

    //Set orientation
    localGeoMesh.CreateOrientationAttr(VtValue(UsdGeomTokens->rightHanded));

    auto myModel = localModel->getComponent<StaticMesh>()->m_pModel;

    //Get the meshes num
    uint32 numMeshes = myModel->getSizeMeshes();

    for (uint32 i = 0; i < numMeshes; ++i) {
      //Set the mesh token
      String meshName("mesh_");

      meshName.append(to_string(i));

      //_tokens->box);
      SdfPath meshPrimPath = modelPrimPath.AppendChild(TfToken(meshName));

      //Define the mesh path as a geom mesh, the path is a child of model
      UsdGeomMesh tempGeoMesh = UsdGeomMesh::Define(m_stage, meshPrimPath);

      if (!tempGeoMesh) {
        return tempGeoMesh;
      }

      //Set orientation
      tempGeoMesh.CreateOrientationAttr(VtValue(UsdGeomTokens->rightHanded));

      /*
      * Obtain all vertices
      */
      int32 numVertices = myModel->getMesh(i)->getVertices();
      auto vertexData = myModel->getMesh(i)->getVecVertex();

      VtArray<GfVec3f> points;

      points.resize(numVertices);

      for (int32 j = 0; j < numVertices; ++j) {
        auto vec = vertexData[j].position;
        points[j] = GfVec3f(vec.x, vec.y, vec.z);
      }

      //Set the vertex points to GeomMesh
      tempGeoMesh.CreatePointsAttr(VtValue(points));

      /*
      * Obtain all indices
      */
      int32 numIndices = myModel->getMesh(i)->getNumIndices();
      auto indices = myModel->getMesh(i)->getVecIndex();
      VtArray<int32> vecIndices;

      vecIndices.resize(numIndices);

      for (int32 j = 0; j < numIndices; ++j) {
        vecIndices[j] = indices[j];
      }

      //Set the index to GeomMesh
      tempGeoMesh.CreateFaceVertexIndicesAttr(VtValue(vecIndices));

      /*
      * Obtain all vertex normals
      */
      int32 numNormals = myModel->getMesh(i)->getVertices();
      VtArray<GfVec3f> meshNormals;
      meshNormals.resize(numVertices);

      for (int32 j = 0; j < numVertices; ++j) {
        auto norm = vertexData[j].normal;
        meshNormals[j] = GfVec3f(norm.x, norm.y, norm.z);
      }

      //Set the vertex normals to GeomMesh
      tempGeoMesh.CreateNormalsAttr(VtValue(meshNormals));

      //Add face vertex count
      VtArray<int32> faceVertexCounts;

      //numIndices
      faceVertexCounts.resize(numIndices / 3);

      std::fill(faceVertexCounts.begin(), faceVertexCounts.end(), 3);

      tempGeoMesh.CreateFaceVertexCountsAttr(VtValue(faceVertexCounts));

      //Set the color on the Model
      UsdPrim meshPrim = tempGeoMesh.GetPrim();
      UsdAttribute displayColorAttr = tempGeoMesh.CreateDisplayColorAttr();

      {
        VtVec3fArray valueArray;
        GfVec3f rgbFace(0.463f, 0.725f, 0.0f);

        valueArray.push_back(rgbFace);
        displayColorAttr.Set(valueArray);
      }

      //Set the UVs values for the model
      UsdGeomPrimvar attr2 = tempGeoMesh.CreatePrimvar(_tokens->st,
                                                       SdfValueTypeNames->TexCoord2fArray);
      {
        int32 uvCount = myModel->getMesh(i)->getVertices();
        VtVec2fArray valueArray;

        valueArray.resize(uvCount);

        for (int32 j = 0; j < uvCount; ++j) {
          auto uv = vertexData[j].texCoords;
          valueArray[j].Set(uv.vec);
        }

        bool status = attr2.Set(valueArray);
      }

      attr2.SetInterpolation(UsdGeomTokens->vertex);
    }

    //Commit the changes to the USD
    m_stage->Save();
    omniUsdLiveProcess();

    return localGeoMesh;
  }

  UsdGeomMesh
  OmniverseConnect::modelToGeoMesh(WeakSPtr<Models> model, SdfPath& primitivePath) {
    UsdGeomMesh geoModel = UsdGeomMesh::Define(m_stage, primitivePath);
    if (!(geoModel)) {
      cout << "\nError, model to geo mesh is null\n";
      return geoModel;
    }

    auto localModel = model.lock();
    if (nullptr == localModel) {
      cout << "\nError, localModel is null\n";
      return geoModel;
    }

    //Define the orientation.
    geoModel.CreateOrientationAttr(VtValue(UsdGeomTokens->rightHanded));

    //Get meshes size.
    uint32 numMeshes = localModel->getSizeMeshes();
    int32 numVertices = 0;
    int32 numIndices = 0;
    int32 currentIndex = 0;
    int32 maxIndexes = 0;
    int32 currentVertexIndex = 0;
    int32 currentIndexVectorIndex = 0;

    //Obtain the num vertex and indexes of all meshes.
    for (uint32 i = 0; i < numMeshes; ++i) {
      numVertices += localModel->getMesh(i)->getVertices();
      numIndices += localModel->getMesh(i)->getNumIndices();
    }

    //Create each array for all things.
    VtArray<GfVec3f> points;
    points.resize(numVertices);

    VtArray<int32> vecIndices;
    vecIndices.resize(numIndices);

    Vector<VtArray<int32>> vecIndices2;
    vecIndices2.resize(numMeshes);

    int32 uvCounter = numVertices;
    //VtVec2fArray valueArray;
    VtVec2dArray valueArray;
    valueArray.resize(uvCounter);

    int32 numNormals = numVertices;
    VtArray<GfVec3f> normalMeshes;
    normalMeshes.resize(numVertices);

    //Subsets zone
    for (uint32 i = 0; i < numMeshes; ++i) {
      auto vertexData = localModel->getMesh(i)->getVecVertex();
      auto currentNumVertex = localModel->getMesh(i)->getVertices();
      auto currentNumIndex = localModel->getMesh(i)->getNumIndices();

      for (uint32 j = 0; j < currentNumVertex; ++j) {
        //Vertices
        auto vertex = vertexData[j].position;

        points[currentVertexIndex] = GfVec3f(vertex.x, vertex.y, vertex.z);

        //Normals
        auto normals = vertexData[j].normal;
        normalMeshes[currentVertexIndex] = GfVec3f(normals.x, normals.y, normals.z);
        
        //UVs
        auto uv = vertexData[j].texCoords;
        valueArray[currentVertexIndex].Set((double*)uv.vec);

        ++currentVertexIndex;
      }

      //Calculate indices for each triangle.
      auto indices = localModel->getMesh(i)->getVecIndex();

      vecIndices2[i].resize(currentNumIndex);
      for (uint32 j = 0; j < currentNumIndex; ++j) {
        auto index = indices[j] + currentIndex;

        if (index > maxIndexes) {
          maxIndexes = index;
        }

        vecIndices[currentIndexVectorIndex] = vecIndices2[i][j] = index;

        ++currentIndexVectorIndex;
      }

      currentIndex = maxIndexes + 1;
    }

    /*
    * S E T
    * I N F O
    * Z O N E
    */

    //Positions.
    geoModel.CreatePointsAttr(VtValue(points));

    //Index.
    geoModel.CreateFaceVertexIndicesAttr(VtValue(vecIndices));

    //Normals.
    geoModel.CreateNormalsAttr(VtValue(normalMeshes));

    //Add face vertex counts.
    VtArray<int32> faceVertexCounts;

    //Faces = num index / 3 (3 points per face).
    faceVertexCounts.resize(numIndices / 3);
    std::fill(faceVertexCounts.begin(), faceVertexCounts.end(), 3);
    geoModel.CreateFaceVertexCountsAttr(VtValue(faceVertexCounts));

    //Set the color on the Model
    UsdPrim meshPrim = geoModel.GetPrim();

    UsdAttribute displayColorAttr = geoModel.CreateDisplayColorAttr();
    {
      VtVec3fArray valueArray;
      GfVec3f rgbFace(0.463f, 0.725f, 0.0f);
      valueArray.push_back(rgbFace);
      displayColorAttr.Set(valueArray);
    }

    UsdGeomPrimvar attribute = geoModel.CreatePrimvar(_tokens->st, 
                                                      SdfValueTypeNames->TexCoord2dArray);
    bool status = attribute.Set(valueArray);

    attribute.SetInterpolation(UsdGeomTokens->vertex);

    //Last face saved, change after to saved the faces of current mesh.
    uint32 last = 0;

    //Create the subsets.
    for (uint32 i = 0; i < numMeshes; ++i) {
      String meshName("mesh_");

      meshName.append(to_string(i));

      VtArray<int32> faceVertexCounts;

      //Get the num of faces of mesh.
      uint32 size = localModel->getMesh(i)->getNumIndices() / 3;

      faceVertexCounts.resize(size);

      for (uint32 j = 0; j < size; ++j) {
        faceVertexCounts[j] = j + last;
      }

      last += size;

      //Create the subset.
      UsdGeomSubset::CreateGeomSubset(geoModel, 
                                      TfToken(meshName), 
                                      UsdGeomTokens->face, faceVertexCounts);
    }

    //Commit the changes to the USD
    //m_stage->Save();
    //omniUsdLiveProcess();

    return geoModel;
  }

  void
  OmniverseConnect::OmniClientConnectionStatusCallbackImpl(void* userData, 
                                                           const char* url, 
                                                           OmniClientConnectionStatus status) noexcept {
    {
      unique_lock<mutex> lk(m_logMutex);
      
      cout << "\nConnection Status - -> " << 
               omniClientGetConnectionStatusString(status) << 
               " [" << url << "]" << endl;
    }

    //We shouldn't just exit here - 
    //we should clean up a bit, but we're going to do it anyway
    if (status == eOmniClientConnectionStatus_ConnectError) {
      cout << "\n[ERROR] Failed connection, exiting." << endl;
      exit(-1);
    }
  }

  void
  OmniverseConnect::failNotify(const char* msg, const char* detail, ...) {
    unique_lock<mutex> lk(m_logMutex);

    fprintf(stderr, "%s\n", msg);

    if (nullptr != detail) {
      fprintf(stderr, "%s\n", detail);
    }
  }

  void
  OmniverseConnect::logCallback(const char* threadName, 
                                const char* component, 
                                OmniClientLogLevel level, 
                                const char* message) {
    unique_lock<mutex> lk(m_logMutex);
    if (m_omniverseLoggingEnabled) {
      puts(message);
      cout << "\n\n";
    }
  }

  bool 
  OmniverseConnect::isValidOmniURL(const String& maybeURL) {
    bool isValidURL = false;

    OmniClientUrl* url = omniClientBreakUrl(maybeURL.c_str());

    if (url->host && url->path && 
       (String(url->scheme) == String("Omniverse") || 
        String(url->scheme) == String("Omni"))) {
      isValidURL = true;
    }

    omniClientFreeUrl(url);

    if (!isValidURL) {
      cout << "\nThis is not an Omniverse Nucleus URL: " << maybeURL.c_str() << endl;
      cout << "\nCorrect Omniverse URL format is: " << endl;
      cout << "\nomniverse://server_name/Path/To/Example/Folder" << endl;
      cout << "\nAllowing program to continue because " << endl;
      cout << "\nFile paths may be provided in the form: C:/Path/To/Stage" << endl;
    }
    return isValidURL;
  }

  void
  OmniverseConnect::checkpointFile(const char* stageUrl, const char* comment) {
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

  void 
  OmniverseConnect::uploadMaterial(const String& destinationPath) {
    String uriPath = destinationPath + "/Materials";

    omniUsdLiveWaitForPendingUpdates();

    //Delete the old version of this folder on Omniverse and wait 
    //for the operation to complete.
    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "\nWaiting for " << uriPath << " to delete...\n";
    }

    omniClientWait(omniClientDelete(uriPath.c_str(), nullptr, nullptr));

    omniUsdLiveWaitForPendingUpdates();

    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "\nFinished" << endl;
    }

    omniUsdLiveWaitForPendingUpdates();

    //Upload the material folder (MDL and textures).
    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "\nWaiting for the resources/Materials folder to upload to ";
      cout << uriPath << " ... ";
    }
    omniClientWait(omniClientCopy("resources/Materials", 
                                  uriPath.c_str(),
                                  nullptr, 
                                  nullptr));

    omniUsdLiveWaitForPendingUpdates();

    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "\nFinished" << endl;
    }
  }

  void 
  OmniverseConnect::createLight() {
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

    omniUsdLiveWaitForPendingUpdates();

    //Commit the changes to the USD
    m_stage->Save();
    omniUsdLiveProcess();
  }

  UsdPrim 
  OmniverseConnect::createLightFromComponent(Lights* newLight, SdfPath& parentPath) {
    auto myRenderer = g_baseRenderer().instancePtr();

    UsdPrim omniPrim;

    omniPrim = createDirectLight(newLight, parentPath);

    myRenderer->updateLocalLight(newLight);

    omniUsdLiveWaitForPendingUpdates();

    // Commit the changes to the USD
    m_stage->Save();
    omniUsdLiveProcess();

    return omniPrim;
  }

  UsdPrim 
  OmniverseConnect::createDirectLight(Lights* newLight, SdfPath& parentPath) {
    UsdLuxDistantLight usdNewLight = UsdLuxDistantLight::Define(m_stage, parentPath);

    auto lightColor = newLight->getDiffuseColor();

    GfVec3f color(lightColor.x, lightColor.y, lightColor.z);

    usdNewLight.CreateColorAttr(VtValue(color));
    usdNewLight.CreateIntensityAttr(VtValue(5000.0f));

    auto prim = usdNewLight.GetPrim();

    return prim;
  }

  void 
  OmniverseConnect::createMaterial(UsdGeomMesh meshIn) {
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

    omniUsdLiveWaitForPendingUpdates();

    //Commit the changes to the USD.
    m_stage->Save();
    omniUsdLiveProcess();
  }

  /*
  * S E T
  * &
  * G E T
  * 
  * Z O N E
  */

  void
  OmniverseConnect::setTransformComponents(UsdGeomXformable& xTransform,
                                           Vector3& position,
                                           Vector3& rotation,
                                           Vector3& scale) {
    /*
    * Op = operation (maybe).
    */

    //Get the xTransform.
    bool resetXTransformStack = false;
    Vector<UsdGeomXformOp> xTransformOps = xTransform.GetOrderedXformOps(&resetXTransformStack);

    //Define storage for the different xform ops that Omniverse Kit likes to use.
    UsdGeomXformOp translateOp;
    UsdGeomXformOp rotateOp;
    UsdGeomXformOp scaleOp;

    //Get the current xform op values
    for (size_t i = 0; i < xTransformOps.size(); ++i) {
      switch (xTransformOps[i].GetOpType()) {
        case UsdGeomXformOp::TypeTranslate:
          translateOp = xTransformOps[i];
          break;

        case UsdGeomXformOp::TypeRotateZYX:
          rotateOp = xTransformOps[i];
          break;

        case UsdGeomXformOp::TypeScale:
          scaleOp = xTransformOps[i];
          break;
      }
    }

    GfVec3d gfPosition = { position.x, position.y, position.z };
    GfVec3f gfRotZYX = { rotation.x, rotation.y, rotation.z };
    GfVec3f gfScale = { scale.x, scale.y, scale.z };

    setOp(xTransform,
          translateOp, 
          UsdGeomXformOp::TypeTranslate, 
          gfPosition, 
          UsdGeomXformOp::Precision::PrecisionDouble);

    setOp(xTransform,
          rotateOp,
          UsdGeomXformOp::TypeRotateZYX, 
          gfRotZYX, 
          UsdGeomXformOp::Precision::PrecisionFloat);

    setOp(xTransform,
          scaleOp, 
          UsdGeomXformOp::TypeScale, 
          gfScale, 
          UsdGeomXformOp::Precision::PrecisionFloat);

    //Make sure the xform op order is correct (translate, rotate, scale).
    Vector<UsdGeomXformOp> xFormOpsReordered;

    xFormOpsReordered.push_back(translateOp);
    xFormOpsReordered.push_back(rotateOp);
    xFormOpsReordered.push_back(scaleOp);

    xTransform.SetXformOpOrder(xFormOpsReordered);
  }

  void 
  OmniverseConnect::setOp(UsdGeomXformable& xTransform,
                          UsdGeomXformOp& op,
                          UsdGeomXformOp::Type opType, 
                          const GfVec3d& value, 
                          const UsdGeomXformOp::Precision precision) {

    omniUsdLiveWaitForPendingUpdates();

    if (!(op)) {
      op = xTransform.AddXformOp(opType, precision);
      unique_lock<mutex> lk(m_logMutex);
    }

    if (op.GetPrecision() == UsdGeomXformOp::Precision::PrecisionFloat) {
      op.Set(GfVec3f(value));
    }
    else {
      op.Set(value);
    }

    omniUsdLiveWaitForPendingUpdates();

    unique_lock<mutex> lk(m_logMutex);
  }

  void
  OmniverseConnect::getTransformComponents(UsdGeomXformable& usdXForm,
                                           Vector3& position, 
                                           Vector3& rotation, 
                                           Vector3& scale) {
    omniUsdLiveWaitForPendingUpdates();

    //Define storage for the different xform ops that Omniverse Kit likes to use.
    UsdGeomXformOp translateOp;
    UsdGeomXformOp rotateOp;
    UsdGeomXformOp scaleOp;

    GfVec3d gfPosition(0);
    GfVec3f gfRotZYX(0);
    GfVec3f gfScale(1);

    bool resetXForm = false;
    Vector<UsdGeomXformOp> usdXFormOp = usdXForm.GetOrderedXformOps(&resetXForm);

    //Get the current xform op values
    for (size_t i = 0; i < usdXFormOp.size(); ++i) {
      switch (usdXFormOp[i].GetOpType()) {
        case UsdGeomXformOp::TypeTranslate:{
          translateOp = usdXFormOp[i];
          translateOp.Get(&gfPosition);
          break;
        }
        
        case UsdGeomXformOp::TypeRotateZYX: {
          rotateOp = usdXFormOp[i];
          rotateOp.Get(&gfRotZYX);
          break;
        }
        
        case UsdGeomXformOp::TypeScale: {
          scaleOp = usdXFormOp[i];
          scaleOp.Get(&gfScale);
          break;
        }
      }
    }

    position = { (float)gfPosition.GetArray()[0], 
                 (float)gfPosition.GetArray()[1], 
                 (float)gfPosition.GetArray()[2] };

    rotation = { (float)gfRotZYX.GetArray()[0], 
                 (float)gfRotZYX.GetArray()[1], 
                 (float)gfRotZYX.GetArray()[2] };

    scale = { (float)gfScale.GetArray()[0], 
              (float)gfScale.GetArray()[1], 
              (float)gfScale.GetArray()[2] };
  }

  /***************************************************************************/
  /**
  * Methods that I do not use a lot.
  */
  /***************************************************************************/

  void
  OmniverseConnect::printConnectedUsername(const String& stageUrl) {
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
      cout << "\nConnected username - -> " << userName << endl;
    }
  }

  UsdGeomMesh 
  OmniverseConnect::createBox(int32 boxNumber) {
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

    //_tokens->box);
    SdfPath boxPrimPath = m_rootPrimPath.AppendChild(TfToken(boxName));
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
  OmniverseConnect::createGeoMeshWithModel(WeakSPtr<Actor> model) {
    auto localModel = model.lock();

    if (nullptr != localModel) {
      SPtr<Actor> newModel(localModel);

      modelToGeoMesh(newModel);
    }
    else {
      cout << "\nError to create GeoMesh with Model\n";
    }
  }

  void
  OmniverseConnect::createEmptyFolder(const String& emptyFolderPath) {
    omniUsdLiveWaitForPendingUpdates();

    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "\nWaiting to create a new folder: " << emptyFolderPath << " ... ";
    }

    OmniClientResult localResult;
    localResult = Count_eOmniClientResult;

    omniClientWait(omniClientCreateFolder(emptyFolderPath.c_str(), 
                                          &localResult, 
      [](void* userData, OmniClientResult result) noexcept {
        auto returnResult = static_cast<OmniClientResult*>(userData);
        *returnResult = result;
      }));

    omniUsdLiveWaitForPendingUpdates();

    {
      unique_lock<mutex> lk(m_logMutex);
      cout << "\nFinished [" << omniClientGetResultString(localResult) << "]" << endl;
    }
  }
}