#pragma once

#include "gaPrerequisitesCore.h"
#include "gaResource.h"
#include "gaAnimations.h"
#include "gaMesh.h"
#include "gaMaterials.h"

namespace gaEngineSDK {

  class StaticMesh;

  class GA_CORE_EXPORT Model : public Resource
  {
  public:
    /**************************************************************************/
    /*
    * Constructor and destructor.
    */
    /**************************************************************************/
    Model() = default;
    
    ~Model() = default;
    
    /**************************************************************************/
    /*
    * Inheritance.
    */
    /**************************************************************************/
    
    /*
    * @brief Function to initialize class members and start with assimp.
    * @param Variable string with the address of the file.
    */
    void
    loadFromFile(const String& file) override;

    /*
    * @brief NO.
    */
    SPtr<StaticMesh> m_pStaticMeshInfo;

  protected:
    /*
    * @brief .
    */
    void
    createDirectories(const String& file);

  private:
    /**************************************************************************/
    /*
    * Members.
    */
    /**************************************************************************/

    /*
    * @brief Member to store the full name of the folder.
    */
    String m_modelDirectory;

    /*
    * @brief Member to store a .
    */
    Vector<Materials> m_vMaterials;
    
    /*
    * @brief .
    */
    Map<String, SPtr<Materials>> m_mMaterials;
  };
}