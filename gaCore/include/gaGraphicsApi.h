#pragma once

#include <windows.h>
#include <gaVector3.h>
#include <gaVector2.h>
#include <gaTextures.h>
#include <gaModule.h>
#include <SFML/Window.hpp>

#include "gaPrerequisitesCore.h"
#include "gaStructures.h"
#include "gaCamera.h"

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
  class VertexShader;
  class PixelShader;
  
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

      virtual 
      ~GraphicsApi() = default;
      
      /***********************************************************************/
      /**
      * Methods.
      */
      /***********************************************************************/

      /**
      * @brief Save the class itself in an instance.
      * @param api pointer.
      * @return true or false.
      */
      void
      setObject(GraphicsApi* api) {
        GraphicsApi::_instance() = api;
      }
      
      /**
      * @brief Function to initialize the device and generate 
      *    the back buffer and depth stencil for DX by default.
      * @param handle to a window.
      * @return true or false.
      */
      virtual bool 
      initDevice(sf::WindowHandle hWnd) { return false; };
      
      /**
      * @brief Function to send to draw the indices of a declared object.
      * @param .
      * @param .
      * @param .
      */
      virtual void 
      drawIndex(uint32 indexCount, uint32 startIndexLocation, uint32 baseVertexLocation) { };
      
      /**
      * @brief Function to exchange buffers and update your information.
      * @param An integer that specifies how to synchronize presentation 
      *        of a frame with the vertical blank.
      * @param An integer value that contains swap-chain presentation options.
      */
      virtual void 
      swapChainPresent(uint32 syncInterval = 0, uint32 flags = 0) { };
      
      /**
      * @brief Function to load textures in file.
      * @param String with the address of the file.
      * @return Textures pointer.
      */
      virtual Textures* 
      loadTextureFromFile(String srcFile) { return nullptr; };
      
      /**
      * @brief OGL function to separate a program.
      */
      virtual void 
      unbindOGL() { };

      /**
      * @brief .
      */
      virtual Matrix4x4
      matrixPolicy(const Matrix4x4& matContext) { return Matrix4x4(); };
      
      /***********************************************************************/
      /**
      * Updates methods.
      */
      /***********************************************************************/
      
      /**
      * @brief Function to update constant buffers.
      * @param The distance (in bytes) from the 
      *        beginning of one line of a texture to the next line.
      * @param Constant buffers contain shader constant data.
      */
      virtual void 
      updateConstantBuffer(const void* srcData, WeakSPtr<ConstantBuffer> updateDataCB) { };
      
      /***********************************************************************/
      /**
      * Clear methods.
      */
      /***********************************************************************/
      
      /**
      * @brief Function to clean our render target view.
      * @param .
      * @param .
      */
      virtual void 
      clearYourRenderTargetView(WeakSPtr<Textures> renderTarget, Vector4 rgba) { };
      
      /**
      * @brief Function to clean our depth stencil view.
      * @param .
      */
      virtual void 
      clearYourDepthStencilView(WeakSPtr<Textures> depthStencil) { };
      
      /***********************************************************************/
      /**
      * Create methods.
      */
      /***********************************************************************/
      
      /**
      * @brief Function to generate the vertex shader and vertex resource.
      *         Function to generate the pixel shader or fragment shader.
      * @param name vertex shader.
      * @param entry point vertex shader.
      * @param name pixel shader.
      * @param entry point pixel shader.
      * @return Shaders points.
      */
      virtual Shaders* 
      createShadersProgram(const WString& nameVS, const String& entryPointVS,
                           const WString& namePS,
                           const String& entryPointPS) { return nullptr; };

      /**
      * @brief Function to load and save vertex shader from file.
      * @param 
      * @return A shared ptr with the vertex shader created.
      */
      virtual SPtr<VertexShader>
      loadVertexShaderFromFile(const char* vertexFilePath, const char* vertexMainFuntion,
                               const char* shaderVersion) { return nullptr; };

      /**
       * @brief Function to load and save pixel shader from file.
       * @param 
       * @return A shared ptr with the pixel shader created.
       */
      virtual SPtr<PixelShader>
      loadPixelShaderFromFile(const char* pixelFilePath, const char* pixelMainFuntion,
                              const char* shaderVersion) { return nullptr; };
      
      /**
      * @brief Function to generate the vertex buffer.
      * @param .
      * @param .
      * @return VertexBuffer pointer.
      */
      virtual VertexBuffer* 
      createVertexBuffer(const void* data, const uint32 size) { return nullptr; };
      
      /**
      * @brief Function to generate the index buffer.
      * @param .
      * @param .
      * @return IndexBuffer pointer.
      */
      virtual IndexBuffer*
      createIndexBuffer(const void* data, const uint32 size) { return nullptr; };
      
      /**
      * @brief Function to generate constant buffers.
      * @param .
      * @return ConstantBuffer pointer.
      */
      virtual ConstantBuffer* 
      createConstantBuffer(const uint32 bufferSize) { return nullptr; };
      
      /**
      * @brief Function to generate the following: 
      *        ° ShaderResourceView 
      *        ° DepthStencilView 
      *        ° RenderTargetView.
      * @param width of texture.
      * @param height of texture.
      * @param bindFlags, identifies how to bind a resource to the pipeline.
      * @param textureFormat, resource data formats.
      * @param fileName, where is it supposed to be.
      * @return Textures pointer.
      */
      virtual Textures* 
      createTexture(const uint32 width, const uint32 height, const uint32 bindFlags,
                    TEXTURE_FORMAT::E textureFormat, const String fileName) {return nullptr;};
      
      /**
      * @brief Function to generate the sampler state.
      * @return SamplerState
      */
      virtual SamplerState* 
      createSamplerState() { return nullptr; };
      
      /**
      * @brief Function to generate the input layout.
      * @param .
      * @return InputLayout pointer.
      */
      virtual InputLayout* 
      createInputLayout(WeakSPtr<Shaders> vertexShader) { return nullptr; };
      
      /***********************************************************************/
      /**
      * Set methods.
      */
      /***********************************************************************/
      
      /**
      * @brief Function to save the information of the pixel shader.
      * @param .
      */
      virtual void 
      setPixelShader(WeakSPtr<Shaders> pixelShader) { };
      
      /**
      * @brief Function to save vertex shader information.
      * @param .
      */
      virtual void 
      setVertexShader(WeakSPtr<Shaders> vertexShader) { };
      
      /**
      * @brief Function to save vertex buffer information.
      * @param .
      */
      virtual void 
      setVertexBuffer(WeakSPtr<VertexBuffer> vertexBuffer) { };
      
      /**
      * @brief Function to save the index buffer information.
      * @param .
      */
      virtual void 
      setIndexBuffer(WeakSPtr<IndexBuffer> indexBuffer) { };
      
      /**
      * @brief Function to save the information of the constant buffers.
      * @param .
      * @param .
      * @param .
      * @param .
      */
      virtual void 
      setConstantBuffer(bool isVertex, WeakSPtr<ConstantBuffer> constantBuffer,
                        const uint32 startSlot,
                        const uint32 numBuffers) { };
      
      /**
      * @brief Function to save the information of the sampler state.
      * @param .
      * @param .
      * @param .
      */
      virtual void 
      setSamplerState(const uint32 startSlot, Vector<SamplerState*>& samplerState,
                      WeakSPtr<Textures> texture) { };
      
      /**
      * @brief Function to save the information of the shader resource view.
      * @param .
      * @param .
      * @param .
      */
      virtual void 
      setShaderResourceView(WeakSPtr<Textures> shaderResourceView, const uint32 startSlot,
                            const uint32 numViews) { };
      
      /**
      * @brief Function to save the information of the render target.
      * @param .
      * @param .
      */
      virtual void 
      setRenderTarget(WeakSPtr<Textures> renderTarget, WeakSPtr<Textures> depthStencil) { };
      
      /**
      * @brief Function to save the depth stencil information.
      * @param .
      * @param .
      */
      virtual void 
      setDepthStencil(WeakSPtr<Textures> depthStencil, const uint32 stencilRef) { };
      
      /**
      * @brief Function to save the information of the input layout.
      * @param .
      */
      virtual void 
      setInputLayout(WeakSPtr<InputLayout> vertexLayout) { };
      
      /**
      * @brief Function to save the viewport information.
      * @param .
      * @param .
      * @param .
      */
      virtual void 
      setViewports(const uint32 width, const uint32 heigth, const uint32 numViewports = 1) { };
      
      /**
      * @brief Function to save the topology information.
      * @param .
      */
      virtual void 
      setPrimitiveTopology(const uint32 topology = PRIMITIVE_TOPOLOGY::kTriangleList) { };
      
      /**
      * @brief Function to call VSSetShader.
      * @param .
      */
      virtual void 
      setYourVS(WeakSPtr<Shaders> vertexShader) { };
      
      /**
      * @brief Function to call VSSetConstantBuffers.
      * @param .
      * @param .
      * @param .
      */
      virtual void 
      setYourVSConstantBuffers(WeakSPtr<ConstantBuffer> constantBuffer, 
                               const uint32 startSlot,
                               const uint32 numBuffers = 1) { };
      
      /**
      * @brief Function to call PSSetShader.
      * @param .
      */
      virtual void 
      setYourPS(WeakSPtr<Shaders> pixelShader) { };
      
      /**
      * @brief Function to call PSSetConstantBuffers.
      * @param .
      * @param .
      * @param .
      */
      virtual void 
      setYourPSConstantBuffers(WeakSPtr<ConstantBuffer> constantBuffer, const uint32 startSlot,
                               const uint32 numBuffers = 1) { };
      
      /**
      * @brief Function to call PSSetSamplers.
      * @param .
      * @param .
      * @param .
      */
      virtual void 
      setYourPSSampler(WeakSPtr<SamplerState> sampler, const uint32 startSlot,
                       const uint32 numSamplers) { };
      
      /**
      * @brief Function to call program and link for OGL.
      * @param .
      */
      virtual void 
      setShaders(WeakSPtr<Shaders> shaders) { };
      
      /***********************************************************************/
      /**
      * Gets.
      */
      /***********************************************************************/
      
      /**
      * @brief Function to obtain the automatically generated back buffer.
      * @return Textures pointer.
      */
      virtual Textures*
      getDefaultBackBuffer() { return nullptr; };
      
      /**
      * @brief Function to obtain the automatically generated depth stencil.
      * @return Textures pointer.
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
       uint32 m_width = 0;
       
       /**
       * @brief Member to save the screen height.
       */
       uint32 m_height = 0;
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