#pragma once

#include <windows.h>
#include <gaVector3.h>
#include <gaVector2.h>
#include <gaTextures.h>
#include <gaModule.h>

#include "gaPrerequisitesCore.h"
#include "gaStructures.h"

namespace gaEngineSDK {

  /**
  * @brief Enum with all numbers with topology type for DX.
  */
  namespace PRIMITIVE_TOPOLOGY {
    enum E {
      kPointList = 1,
      kLineList = 2,
      kLineStrip = 3,
      kTriangleList = 4,
      kTriangleStrip = 5,
    };
  }
  
  /***************************************************************************/
  /**
  * Instances of the abstracted classes.
  */
  /***************************************************************************/
  class ConstantBuffer;
  class Textures;
  class SamplerState;
  class InputLayout;
  class Shaders;
  class VertexBuffer;
  class IndexBuffer;
  
  /**
  * @brief Parent class where we can add 
  *        new functions to generalize a graphical api.
  */
  class GA_CORE_EXPORT GraphicsApi : public Module<GraphicsApi>
  {
    public:
      /***********************************************************************/
      /**
      * Constructor and destructor.
      */
      /***********************************************************************/
      GraphicsApi() = default;

      virtual ~GraphicsApi() = default;
      
      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/

      void
      setObject(GraphicsApi* api) {
        GraphicsApi::_instance() = api;
      }
      
      /**
      * @brief Function to initialize the device and generate 
      *    the back buffer and depth stencil for DX by default
      * @param hWnd handle to a window.
      * @return true or false.
      */
      virtual bool 
      initDevice(HWND hWnd) { return false; };
      
      /**
      * @brief Function to send to draw the indices of a declared object.
      * @param indexCount 
      *        startIndexLocation 
      *        baseVertexLocation 
      */
      virtual void 
      drawIndex(uint32 indexCount,
                uint32 startIndexLocation,
                uint32 baseVertexLocation) { };
      
      /**
      * @brief Function to exchange buffers and update your information.
      * @param syncInterval 
      *        flags 
      */
      virtual void 
      swapChainPresent(uint32 syncInterval,
                       uint32 flags) { };
      
      /**
      * @brief Function to load textures in file.
      * @param srcFile
      * @return Textures.
      */
      virtual Textures* 
      loadTextureFromFile(std::string srcFile) { return nullptr; };
      
      /**
      * @brief OGL function to separate a program.
      */
      virtual void 
      unbindOGL() { };
      
      /***********************************************************************/
      /**
      * Updates methods.
      */
      /***********************************************************************/
      
      /**
      * @brief Function to update constant buffers.
      * @param srcData
      *        updateDataCB
      */
      virtual void 
      updateConstantBuffer(const void* srcData,
                           ConstantBuffer& updateDataCB) { };
      
      /***********************************************************************/
      /**
      * Clear methods.
      */
      /***********************************************************************/
      
      /**
      * @brief Function to clean our render target view.
      * @param renderTarget
      *        r
      *        g
      *        b
      *        a
      */
      virtual void 
      clearYourRenderTargetView(Textures* renderTarget,
                                float r, float g, float b, float a) { };
      
      /**
      * @brief Function to clean our depth stencil view.
      * @param depthStencil
      */
      virtual void 
      clearYourDepthStencilView(Textures* depthStencil) { };
      
      /***********************************************************************/
      /**
      * Create methods.
      */
      /***********************************************************************/
      
      /**
      * @brief Function to generate the vertex shader and vertex resource.
      *         Function to generate the pixel shader or fragment shader.
      * @param nameVS
      *        entryPointVS
      *        namePS
      *        entryPointPS
      * @return Shaders
      */
      virtual Shaders* 
      createShadersProgram(const std::wstring& nameVS,
                           const std::string& entryPointVS, 
                           const std::wstring& namePS,
                           const std::string& entryPointPS) { return nullptr; };
      
      /**
      * @brief Function to generate the vertex buffer.
      * @param data
      *        size
      * @return gaVertexBuffer
      */
      virtual VertexBuffer* 
      createVertexBuffer(const void* data,
                         const uint32 size) { return nullptr; };
      
      /**
      * @brief Function to generate the index buffer.
      * @param data
      *        size
      * @return gaIndexBuffer
      */
      virtual IndexBuffer*
      createIndexBuffer(const void* data,
                        const uint32 size) { return nullptr; };
      
      /**
      * @brief Function to generate constant buffers.
      * @param bufferSize
      * @return ConstantBuffer
      */
      virtual ConstantBuffer* 
      createConstantBuffer(const uint32 bufferSize) { return nullptr; };
      
      /**
      * @brief Function to generate the following: 
      *        ° ShaderResourceView 
      *        ° DepthStencilView 
      *        ° RenderTargetView.
      * @param width
      *        height
      *        bindFlags
      *        textureFormat
      *        fileName
      * @return Textures
      */
      virtual Textures* 
      createTexture(const uint32 width,
                    const uint32 height,
                    const uint32 bindFlags,
                    TEXTURE_FORMAT::E textureFormat,
                    const std::string fileName) { return nullptr; };
      
      /**
      * @brief Function to generate the sampler state.
      * @return SamplerState
      */
      virtual SamplerState* 
      createSamplerState() { return nullptr; };
      
      /**
      * @brief Function to generate the input layout.
      * @param vertexShader
      * @return InputLayout
      */
      virtual InputLayout* 
      createInputLayout(Shaders& vertexShader) { return nullptr; };
      
      /***********************************************************************/
      /**
      * Set methods.
      */
      /***********************************************************************/
      
      /**
      * @brief Function to save the information of the pixel shader.
      * @param pixelShader
      */
      virtual void 
      setPixelShader(Shaders& pixelShader) { };
      
      /**
      * @brief Function to save vertex shader information.
      * @param vertexShader
      */
      virtual void 
      setVertexShader(Shaders& vertexShader) { };
      
      /**
      * @brief Function to save vertex buffer information.
      * @param vertexBuffer
      */
      virtual void 
      setVertexBuffer(VertexBuffer& vertexBuffer) { };
      
      /**
      * @brief Function to save the index buffer information.
      * @param indexBuffer
      */
      virtual void 
      setIndexBuffer(IndexBuffer& indexBuffer) { };
      
      /**
      * @brief Function to save the information of the constant buffers.
      * @param isVertex
      *        constantBuffer
      *        startSlot
      *        numBuffers
      */
      virtual void 
      setConstantBuffer(bool isVertex,
                        ConstantBuffer& constantBuffer,
                        const uint32 startSlot,
                        const uint32 numBuffers) { };
      
      /**
      * @brief Function to save the information of the sampler state.
      * @param startSlot
      *        samplerState
      *        texture
      */
      virtual void 
      setSamplerState(const uint32 startSlot,
                      std::vector<SamplerState*>& samplerState,
                      Textures& texture) { };
      
      /**
      * @brief Function to save the information of the shader resource view.
      * @param shaderResourceView
      *        startSlot
      *        numViews
      */
      virtual void 
      setShaderResourceView(Textures* shaderResourceView,
                            const uint32 startSlot,
                            const uint32 numViews) { };
      
      /**
      * @brief Function to save the information of the render target.
      * @param renderTarget
      *        depthStencil
      */
      virtual void 
      setRenderTarget(Textures* renderTarget,
                      Textures* depthStencil) { };
      
      /**
      * @brief Function to save the depth stencil information.
      * @param depthStencil
      *        stencilRef
      */
      virtual void 
      setDepthStencil(Textures& depthStencil,
                      const uint32 stencilRef) { };
      
      /**
      * @brief Function to save the information of the input layout.
      * @param vertexLayout
      */
      virtual void 
      setInputLayout(InputLayout& vertexLayout) { };
      
      /**
      * @brief Function to save the viewport information.
      * @param numViewports
      *        width
      *        heigth
      */
      virtual void 
      setViewport(const uint32 numViewports,
                  const uint32 width, 
                  const uint32 heigth) { };
      
      /**
      * @brief Function to save the topology information.
      * @param topology
      */
      virtual void 
      setPrimitiveTopology(const uint32 topology) { };
      
      /**
      * @brief Function to call VSSetShader.
      * @param vertexShader
      */
      virtual void 
      setYourVS(Shaders& vertexShader) { };
      
      /**
      * @brief Function to call VSSetConstantBuffers.
      * @param constantBuffer
      *        startSlot
      *        numBuffers
      */
      virtual void 
      setYourVSConstantBuffers(ConstantBuffer* constantBuffer,
                               const uint32 startSlot,
                               const uint32 numBuffers) { };
      
      /**
      * @brief Function to call PSSetShader.
      * @param pixelShader
      */
      virtual void 
      setYourPS(Shaders& pixelShader) { };
      
      /**
      * @brief Function to call PSSetConstantBuffers.
      * @param constantBuffer
      *        startSlot
      *        numBuffers
      */
      virtual void 
      setYourPSConstantBuffers(ConstantBuffer* constantBuffer,
                               const uint32 startSlot,
                               const uint32 numBuffers) { };
      
      /**
      * @brief Function to call PSSetSamplers.
      * @param sampler
      *        startSlot
      *        numSamplers
      */
      virtual void 
      setYourPSSampler(SamplerState& sampler,
                       const uint32 startSlot,
                       const uint32 numSamplers) { };
      
      /**
      * @brief Function to call program and link for OGL.
      * @param shaders
      */
      virtual void 
      setShaders(Shaders& shaders) { };
      
      /***********************************************************************/
      /**
      * Gets.
      */
      /***********************************************************************/
      
      /**
      * @brief Function to obtain the automatically generated back buffer.
      * @return Textures.
      */
      virtual Textures*
      getDefaultBackBuffer() { return nullptr; };
      
      /**
      * @brief Function to obtain the automatically generated depth stencil.
      * @return Textures.
      */
      virtual Textures*
      getDefaultDepthStencil() { return nullptr; };

     protected:
       /**********************************************************************/
       /**
       * Members.
       */
       /**********************************************************************/
       
       /**
       * @brief Member to save screen width.
       */
       uint32 m_width;
       
       /**
       * @brief Member to save the screen height.
       */
       uint32 m_height;
  };

  /***************************************************************************/
  /**
  * Export.
  */
  /***************************************************************************/
  
  using funcGraphicsApiProto = GraphicsApi * (*)();

  GA_CORE_EXPORT GraphicsApi&
  g_graphicApi();
}