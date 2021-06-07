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
      * @param hWnd, handle to a window.
      * @return true or false.
      */
      virtual bool 
      initDevice(sf::WindowHandle ) { return false; };
      
      /**
      * @brief Function to send to draw the indices of a declared object.
      * @param indexCount.
      * @param startIndexLocation.
      * @param baseVertexLocation.
      */
      virtual void 
      drawIndex(uint32 ,
                uint32 ,
                uint32 ) { };
      
      /**
      * @brief Function to exchange buffers and update your information.
      * @param syncInterval, An integer that specifies how to synchronize presentation 
      *        of a frame with the vertical blank.
      * @param flags, An integer value that contains swap-chain presentation options.
      */
      virtual void 
      swapChainPresent(uint32 ,
                       uint32 ) { };
      
      /**
      * @brief Function to load textures in file.
      * @param srcFile, String with the address of the file.
      * @return Textures pointer.
      */
      virtual Textures* 
      loadTextureFromFile(String ) { return nullptr; };
      
      /**
      * @brief OGL function to separate a program.
      */
      virtual void 
      unbindOGL() { };

      virtual Matrix4x4
      matrix4x4Context(const Matrix4x4&) { return Matrix4x4(); };
      
      /***********************************************************************/
      /**
      * Updates methods.
      */
      /***********************************************************************/
      
      /**
      * @brief Function to update constant buffers.
      * @param srcData, The distance (in bytes) from the 
      *        beginning of one line of a texture to the next line.
      * @param updateDataCB, Constant buffers contain shader constant data.
      */
      virtual void 
      updateConstantBuffer(const void* ,
                           ConstantBuffer& ) { };
      
      /***********************************************************************/
      /**
      * Clear methods.
      */
      /***********************************************************************/
      
      /**
      * @brief Function to clean our render target view.
      * @param renderTarget.
      * @param r, g, b, a.
      */
      virtual void 
      clearYourRenderTargetView(Textures* ,
                                float , float , float , float ) { };
      
      /**
      * @brief Function to clean our depth stencil view.
      * @param depthStencil.
      */
      virtual void 
      clearYourDepthStencilView(Textures* ) { };
      
      /***********************************************************************/
      /**
      * Create methods.
      */
      /***********************************************************************/
      
      /**
      * @brief Function to generate the vertex shader and vertex resource.
      *         Function to generate the pixel shader or fragment shader.
      * @param nameVS of vertex shader.
      * @param entryPointVS of vertex shader.
      * @param namePS of pixel shader.
      * @param entryPointPS of pixel shader.
      * @return Shaders points.
      */
      virtual Shaders* 
      createShadersProgram(const WString& ,
                           const String& , 
                           const WString& ,
                           const String& ) { return nullptr; };
      
      /**
      * @brief Function to generate the vertex buffer.
      * @param data.
      * @param size.
      * @return VertexBuffer pointer.
      */
      virtual VertexBuffer* 
      createVertexBuffer(const void* ,
                         const uint32 ) { return nullptr; };
      
      /**
      * @brief Function to generate the index buffer.
      * @param data.
      * @param size.
      * @return IndexBuffer pointer.
      */
      virtual IndexBuffer*
      createIndexBuffer(const void* ,
                        const uint32 ) { return nullptr; };
      
      /**
      * @brief Function to generate constant buffers.
      * @param bufferSize.
      * @return ConstantBuffer pointer.
      */
      virtual ConstantBuffer* 
      createConstantBuffer(const uint32 ) { return nullptr; };
      
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
      createTexture(const uint32 ,
                    const uint32 ,
                    const uint32 ,
                    TEXTURE_FORMAT::E ,
                    const String ) { return nullptr; };
      
      /**
      * @brief Function to generate the sampler state.
      * @return SamplerState
      */
      virtual SamplerState* 
      createSamplerState() { return nullptr; };
      
      /**
      * @brief Function to generate the input layout.
      * @param vertexShader.
      * @return InputLayout pointer.
      */
      virtual InputLayout* 
      createInputLayout(Shaders& ) { return nullptr; };
      
      /***********************************************************************/
      /**
      * Set methods.
      */
      /***********************************************************************/
      
      /**
      * @brief Function to save the information of the pixel shader.
      * @param pixelShader.
      */
      virtual void 
      setPixelShader(Shaders& ) { };
      
      /**
      * @brief Function to save vertex shader information.
      * @param vertexShader.
      */
      virtual void 
      setVertexShader(Shaders& ) { };
      
      /**
      * @brief Function to save vertex buffer information.
      * @param vertexBuffer.
      */
      virtual void 
      setVertexBuffer(VertexBuffer& ) { };
      
      /**
      * @brief Function to save the index buffer information.
      * @param indexBuffer.
      */
      virtual void 
      setIndexBuffer(IndexBuffer& ) { };
      
      /**
      * @brief Function to save the information of the constant buffers.
      * @param isVertex.
      * @param constantBuffer.
      * @param startSlot.
      * @param numBuffers.
      */
      virtual void 
      setConstantBuffer(bool ,
                        ConstantBuffer& ,
                        const uint32 ,
                        const uint32 ) { };
      
      /**
      * @brief Function to save the information of the sampler state.
      * @param startSlot.
      * @param samplerState.
      * @param texture.
      */
      virtual void 
      setSamplerState(const uint32 ,
                      Vector<SamplerState*>& ,
                      Textures* ) { };
      
      /**
      * @brief Function to save the information of the shader resource view.
      * @param shaderResourceView.
      * @param startSlot.
      * @param numViews.
      */
      virtual void 
      setShaderResourceView(Textures* ,
                            const uint32 ,
                            const uint32 ) { };
      
      /**
      * @brief Function to save the information of the render target.
      * @param renderTarget.
      * @param depthStencil.
      */
      virtual void 
      setRenderTarget(Textures* ,
                      Textures* ) { };
      
      /**
      * @brief Function to save the depth stencil information.
      * @param depthStencil.
      * @param stencilRef.
      */
      virtual void 
      setDepthStencil(Textures& ,
                      const uint32 ) { };
      
      /**
      * @brief Function to save the information of the input layout.
      * @param vertexLayout.
      */
      virtual void 
      setInputLayout(InputLayout& ) { };
      
      /**
      * @brief Function to save the viewport information.
      * @param numViewports.
      * @param width.
      * @param heigth.
      */
      virtual void 
      setViewport(const uint32 ,
                  const uint32 , 
                  const uint32 ) { };
      
      /**
      * @brief Function to save the topology information.
      * @param topology.
      */
      virtual void 
      setPrimitiveTopology(const uint32 ) { };
      
      /**
      * @brief Function to call VSSetShader.
      * @param vertexShader.
      */
      virtual void 
      setYourVS(Shaders& ) { };
      
      /**
      * @brief Function to call VSSetConstantBuffers.
      * @param constantBuffer.
      * @param startSlot.
      * @param numBuffers.
      */
      virtual void 
      setYourVSConstantBuffers(ConstantBuffer* ,
                               const uint32 ,
                               const uint32 ) { };
      
      /**
      * @brief Function to call PSSetShader.
      * @param pixelShader.
      */
      virtual void 
      setYourPS(Shaders& ) { };
      
      /**
      * @brief Function to call PSSetConstantBuffers.
      * @param constantBuffer.
      * @param startSlot.
      * @param numBuffers.
      */
      virtual void 
      setYourPSConstantBuffers(ConstantBuffer* ,
                               const uint32 ,
                               const uint32 ) { };
      
      /**
      * @brief Function to call PSSetSamplers.
      * @param sampler.
      * @param startSlot.
      * @param numSamplers.
      */
      virtual void 
      setYourPSSampler(SamplerState& ,
                       const uint32 ,
                       const uint32 ) { };
      
      /**
      * @brief Function to call program and link for OGL.
      * @param shaders.
      */
      virtual void 
      setShaders(Shaders& ) { };
      
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