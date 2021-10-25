#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <gaPrerequisitesCore.h>

#include "gaComputeShader.h"

namespace gaEngineSDK {
  class ComputeShaderDX final : public ComputeShader
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    ComputeShaderDX() = default;

    ~ComputeShaderDX() = default;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    bool
    compileComputeShaderFromFile(const WString& fileName,
                                 const String& entryPoint,
                                 const String& versionCS);

   protected:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief .
    */
    ID3D11ComputeShader* m_pComputeShaderDX = nullptr;

    /*
    * @brief .
    */
    ID3DBlob* m_pBlob = nullptr;

    friend class GraphicsApiDX;
  };
}