#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <gaQuaternions.h>

#include "gaPrerequisitesCore.h"
#include "gaSamplerState.h"

namespace gaEngineSDK {

  class Mesh;

  class GA_CORE_EXPORT Model : public Resource
  {
    public:
      /***********************************************************************/
      /*
      * Constructor and destructor.
      */
      /***********************************************************************/
      Model() = default;

      ~Model() = default;

      /***********************************************************************/
      /*
      * Gets.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      SPtr<SamplerState>
      getSampler();

      /*
      * @brief .
      */
      Vector<SPtr<Mesh>>
      getMeshesInfo();

      /***********************************************************************/
      /*
      * Sets.
      */
      /***********************************************************************/

      /*
      * @brief .
      */
      void
      setSampler(WeakSPtr<SamplerState> sampler);

      /*
      * @brief .
      */
      void
      setMeshesInfo(SPtr<Mesh> meshInfo);

      /*
      * @brief .
      */
      //Matrix4x4 m_globalInverseTransform;

    private:
      /***********************************************************************/
      /*
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief Member to store a string of samplers.
      */
      SPtr<SamplerState> m_sampler;

      /*
      * @brief .
      */
      Vector<SPtr<Mesh>> m_vMeshes;
  };  
}