#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>

#include <gaGraphicsApi.h>
#include <gaConstantBuffer.h>
#include <gaTextures.h>
#include <gaShaders.h>
#include <gaVertexBuffer.h>
#include <gaIndexBuffer.h>

#include "gaPrerequisitesCore.h"

namespace gaEngineSDK {

  /*
  * @brief This class is a child of the Graphic Api parent class, 
  *        all comments on members or methods are found in their respective .h.
  */
  
  class GraphicsApiDX final : public GraphicsApi
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/

      GraphicsApiDX() = default;

      ~GraphicsApiDX();

      /***********************************************************************/
      /**
      * Inheritance methods.
      */
      /***********************************************************************/

      bool 
      initDevice(sf::WindowHandle hWnd)override;

      void 
      drawIndex(uint32 indexCount,
      		      uint32 startIndexLocation,
      		      uint32 baseVertexLocation)override;

      void 
      swapChainPresent(uint32 syncInterval,
      		             uint32 flags)override;

      Textures* 
      loadTextureFromFile(String srcFile)override;

      void 
      unbindOGL()override;

      /***********************************************************************/
      /**
      * Updates methods.
      */
      /***********************************************************************/

      void 
      updateConstantBuffer(const void* srcData,
      		                 ConstantBuffer& updateDataCB)override;

      /***********************************************************************/
      /**
      * Clear methods.
      */
      /***********************************************************************/

      void 
      clearYourRenderTargetView(Textures* renderTarget,
      		                      float r, float g, float b, float a)override;

      void 
      clearYourDepthStencilView(Textures* depthStencil)override;

      /***********************************************************************/
      /**
      * Create methods.
      */
      /***********************************************************************/

      Shaders* 
      createShadersProgram(const WString& nameVS,
      		                 const String& entryPointVS, 
      	                   const WString& namePS,
      		                 const String& entryPointPS)override;

      VertexBuffer* 
      createVertexBuffer(const void* data,
      	                 const uint32 size)override;

      IndexBuffer* 
      createIndexBuffer(const void* data,
      	                const uint32 size)override;

      ConstantBuffer* 
      createConstantBuffer(const uint32 bufferSize)override;

      Textures*
      createTexture(const uint32 width,
                    const uint32 height,
                    const uint32 bindFlags,
                    TEXTURE_FORMAT::E textureFormat,
                    const String fileName)override;

      SamplerState* 
      createSamplerState()override;

      InputLayout* 
      createInputLayout(Shaders& vertexShader)override;

      /***********************************************************************/
      /**
      * Set methods.
      */
      /***********************************************************************/

      void 
      setPixelShader(Shaders& pixelShader)override;

      void 
      setVertexShader(Shaders& vertexShader)override;

      void 
      setVertexBuffer(VertexBuffer& vertexBuffer)override;

      void 
      setIndexBuffer(IndexBuffer& indexBuffer)override;

      void 
      setConstantBuffer(bool isVertex,
      	                ConstantBuffer& constantBuffer,
      	                const uint32 startSlot,
      	                const uint32 numBuffers)override;

      void 
      setSamplerState(const uint32 startSlot,
      	              Vector<SamplerState*>& samplerState,
      	              Textures& texture)override;

      void 
      setShaderResourceView(Textures* shaderResourceView,
      	                    const uint32 startSlot,
      	                    const uint32 numViews)override;

      void 
      setRenderTarget(Textures* renderTarget,
      	              Textures* depthStencil)override;

      void 
      setDepthStencil(Textures& depthStencil,
      	              const uint32 stencilRef)override;

      void 
      setInputLayout(InputLayout& vertexLayout)override;

      void 
      setViewport(const uint32 numViewports,
      	          const uint32 width, 
      	          const uint32 heigth)override;

      void 
      setPrimitiveTopology(const uint32 topology)override;

      void 
      setYourVS(Shaders& vertexShader)override;

      void 
      setYourVSConstantBuffers(ConstantBuffer* constantBuffer,
      	                       const uint32 startSlot,
      	                       const uint32 numBuffers)override;

      void 
      setYourPS(Shaders& pixelShader)override;

      void 
      setYourPSConstantBuffers(ConstantBuffer* constantBuffer,
      	                       const uint32 startSlot,
      	                       const uint32 numBuffers)override;

      void 
      setYourPSSampler(SamplerState& sampler,
      	               const uint32 startSlot,
      	               const uint32 numSamplers)override;

      void 
      setShaders(Shaders& shaders)override;

      /***********************************************************************/
      /**
      * Gets.
      */
      /***********************************************************************/

      Textures*
      getDefaultBackBuffer()override;

      Textures*
      getDefaultDepthStencil()override;

    private:
      /***********************************************************************/
      /**
      * Members.
      */
      /***********************************************************************/

      /*
      * @brief Member with window information brought from user.
      */
      sf::WindowHandle m_hWnd;
      
      /*
      * @brief Member represents a virtual adapter.
      */
      ID3D11Device* m_pd3dDevice = nullptr;

      /*
      * @brief Member to implement one or more surfaces to store rendered data.
      */
      IDXGISwapChain* m_pSwapChain = nullptr;

      /*
      * @brief Member to represent a device context that generates render commands.
      */
      ID3D11DeviceContext* m_pImmediateContext = nullptr;

      /*
      * @brief Member to store and access a texture resource during in-depth template testing.
      */
      Textures* m_pDepthStencil = nullptr;

      /*
      * @brief Member to store the default texture when creating the device.
      */
      Textures* m_pBackBuffer = nullptr;
  };

  /***************************************************************************/
  /**
  * Export.
  */
  /***************************************************************************/

  extern "C" GA_CORE_EXPORT GraphicsApi *
  createGraphicApi() {
    GraphicsApiDX* pGraphicsApiDX = new GraphicsApiDX();
    return pGraphicsApiDX;
  }
}