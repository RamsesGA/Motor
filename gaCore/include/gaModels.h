#pragma once

#include "gaPrerequisitesCore.h"
#include "gaResource.h"
#include "gaAnimations.h"
#include "gaMesh.h"
#include "gaMaterials.h"

namespace gaEngineSDK {
  class StaticMesh;

  class GA_CORE_EXPORT Models : public Resource
  {
   public:
    /*************************************************************************/
    /*
    * Constructor and destructor.
    */
    /*************************************************************************/
    Models() = default;

    ~Models() = default;

    /*************************************************************************/
    /*
    * Inheritance.
    */
    /*************************************************************************/

    /*
    * @brief Function to initialize class members and start with assimp.
    * @param Variable string with the address of the file.
    */
    void
    loadFromFile(const String& file) override;
    
    void
    loadFromFile(const String& file, TYPE_TEXTURES::E typeTexture) override { }

    void
    addNewMesh(Mesh newMesh);

    void
    addNewMesh(Vector<Mesh> vMeshes);

    Mesh
    createVertexData(Vertex* vertexInfo, uint32 numVertexes);

    Mesh&
    getMesh(uint32 index);

    uint32
    getSizeMeshes();

    /*
    * @brief .
    */
    Map<String, SPtr<Materials>> m_mMaterials;

   protected:
    /*
    * @brief .
    */
    void
    createDirectories(const String& file);

    /*************************************************************************/
    /*
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief Member to store the full name of the folder.
    */
    String m_modelDirectory;

    /*
    * @brief .
    */
    Animations m_animInfo;

    /*
    * @brief .
    */
    Vector<cbModelData> m_vModelData;

    /*
    * @brief Member to store a string of Meshes.
    */
    Vector<Mesh> m_vMeshes;

    /*
    * @brief .
    */
    Vector<String> m_vMeshName;

    /*
    * @brief .
    */
    Vector<SPtr<AnimationData>> m_vAnimationData;

    friend class StaticMesh;
  };
}