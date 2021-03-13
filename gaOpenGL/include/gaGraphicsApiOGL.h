#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include <Glad/glad.h>
#include <Glm/glm.hpp>

#include <gaGraphicsApi.h>
#include <gaConstantBuffer.h>
#include <gaTextures.h>
#include <gaShaders.h>
#include <gaVertexBuffer.h>
#include <gaIndexBuffer.h>

#include <gaPrerequisitesUtilities.h>

namespace gaEngineSDK {
  
  class GraphicsApiOGL final : public GraphicsApi
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/

      GraphicsApiOGL() = default;

      ~GraphicsApiOGL();

      /***********************************************************************/
      /**
      * Inheritance methods.
      */
      /***********************************************************************/

      bool 
      initDevice(HWND hWnd)override;

      void 
      drawIndex(uint32 indexCount,
      		      uint32 startIndexLocation,
      		      uint32 baseVertexLocation)override;

      void 
      swapChainPresent(uint32 syncInterval,
      		             uint32 flags)override;

      Textures* 
      loadTextureFromFile(std::string srcFile)override;

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
      createShadersProgram(const std::wstring& nameVS,
      		                 const std::string& entryPointVS, 
      	                   const std::wstring& namePS,
      		                 const std::string& entryPointPS)override;

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
                    const std::string fileName)override;

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

      /// <summary>
      /// Miembro con la información
      /// de la ventana traida del usuario
      /// </summary>
      HWND m_hWnd;

      /// <summary>
      /// 
      /// </summary>
      HDC m_HandleToDC;

      /// <summary>
      /// 
      /// </summary>
      HGLRC m_renderingContext;

      /// <summary>
      /// Miembro para almacenar
      /// la topología y que la api lo conozca
      /// </summary>
      uint32 m_topology;
  };
}