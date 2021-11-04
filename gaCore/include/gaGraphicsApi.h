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
  class RenderTarget;
  class ComputeBuffer;

  /**
  * @brief Parent class where we can add
  *      new functions to generalize a graphical api.
  */
  class GA_CORE_EXPORT GraphicsApi : public Module<GraphicsApi>
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/
    GraphicsApi() = default;

    virtual
    ~GraphicsApi() = default;

    /*************************************************************************/
    /**
    * Methods.
    */
    /*************************************************************************/

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
    initDevice(sf::WindowHandle hWnd) { GA_UNREFERENCED_PARAMETER(hWnd);  return false; };

    /**
    * @brief Function to send to draw the indices of a declared object.
    * @param .
    * @param .
    * @param .
    */
    virtual void
    drawIndex(uint32 indexCount, 
              uint32 startIndexLocation = 0, 
              uint32 baseVertexLocation = 0) {
      GA_UNREFERENCED_PARAMETER(indexCount);
      GA_UNREFERENCED_PARAMETER(startIndexLocation);
      GA_UNREFERENCED_PARAMETER(baseVertexLocation);
    };

    /**
    * @brief Function to exchange buffers and update your information.
    * @param An integer that specifies how to synchronize presentation
    *      of a frame with the vertical blank.
    * @param An integer value that contains swap-chain presentation options.
    */
    virtual void
    swapChainPresent(uint32 syncInterval = 0, uint32 flags = 0) {
      GA_UNREFERENCED_PARAMETER(syncInterval); GA_UNREFERENCED_PARAMETER(flags);
    };

    /**
    * @brief Function to load textures in file.
    * @param String with the address of the file.
    * @return Textures pointer.
    */
    virtual Textures*
    loadTextureFromFile(String srcFile) {
      GA_UNREFERENCED_PARAMETER(srcFile);
      return nullptr;
    };

    /**
    * @brief OGL function to separate a program.
    */
    virtual void
    unbindOGL() { };

    /**
    * @brief .
    */
    virtual Matrix4x4
    matrixPolicy(const Matrix4x4& matContext) {
      GA_UNREFERENCED_PARAMETER(matContext);
      return Matrix4x4();
    };

    virtual void
    dispatch(uint32 threadGroupCountX, uint32 threadGroupCountY, uint32 threadGroupCountZ) {
      GA_UNREFERENCED_PARAMETER(threadGroupCountX);
      GA_UNREFERENCED_PARAMETER(threadGroupCountY);
      GA_UNREFERENCED_PARAMETER(threadGroupCountZ);
    };

    virtual void
    desbindUAV(uint32 startSlot, uint32 numUAV = 1) {
      GA_UNREFERENCED_PARAMETER(startSlot);
      GA_UNREFERENCED_PARAMETER(numUAV);
    };

    virtual void
    desbindRT() {};

    virtual void
    desbindSRV(uint32 startSlot = 0) {};

    /*************************************************************************/
    /**
    * Updates methods.
    */
    /*************************************************************************/

    /**
    * @brief Function to update constant buffers.
    * @param The distance (in bytes) from the
    *      beginning of one line of a texture to the next line.
    * @param Constant buffers contain shader constant data.
    */
    virtual void
    updateConstantBuffer(const void* srcData, WeakSPtr<ConstantBuffer> updateDataCB) {
      GA_UNREFERENCED_PARAMETER(srcData); GA_UNREFERENCED_PARAMETER(updateDataCB);
    };

    virtual void
    updateComputeBuffer(const void* srcData, WeakSPtr<ComputeBuffer> computeBuffer) {
      GA_UNREFERENCED_PARAMETER(srcData); GA_UNREFERENCED_PARAMETER(computeBuffer);
    };

    /*************************************************************************/
    /**
    * Clear methods.
    */
    /*************************************************************************/

    /**
    * @brief Function to clean our render target view.
    * @param .
    * @param .
    */
    virtual void
    clearYourRenderTargetView(WeakSPtr<Textures> renderTarget, Vector4 rgba) {
      GA_UNREFERENCED_PARAMETER(renderTarget); GA_UNREFERENCED_PARAMETER(rgba);
    };

    virtual void
    clearYourRenderTarget(WeakSPtr<RenderTarget> renderTarget, Vector4 rgba) {
      GA_UNREFERENCED_PARAMETER(renderTarget); GA_UNREFERENCED_PARAMETER(rgba);
    };

    /**
    * @brief Function to clean our depth stencil view.
    * @param .
    */
    virtual void
    clearYourDepthStencilView(WeakSPtr<Textures> depthStencil) {
      GA_UNREFERENCED_PARAMETER(depthStencil);
    };

    /*
    * @brief .
    */
    virtual void
    clearYourDepthStencilView(WeakSPtr<RenderTarget> renderTarg) {
      GA_UNREFERENCED_PARAMETER(renderTarg);
    };

    /*************************************************************************/
    /**
    * Create methods.
    */
    /*************************************************************************/

    /**
    * @brief Function to generate the vertex shader and vertex resource.
    *       Function to generate the pixel shader or fragment shader.
    * @param name vertex shader.
    * @param entry point vertex shader.
    * @param name pixel shader.
    * @param entry point pixel shader.
    * @return Shaders points.
    */
    virtual Shaders*
    createShadersProgram(const WString& nameVS, 
                         const String& entryPointVS,
                         const WString& namePS, 
                         const String& entryPointPS) {
      GA_UNREFERENCED_PARAMETER(nameVS); 
      GA_UNREFERENCED_PARAMETER(entryPointVS);
      GA_UNREFERENCED_PARAMETER(namePS); 
      GA_UNREFERENCED_PARAMETER(entryPointPS);
      return nullptr;
    };

    virtual Shaders*
    createComputeShaderProgram(const WString& fileName,
                               const String& entryPoint,
                               const String& versionCS = "cs_5_0") {
      GA_UNREFERENCED_PARAMETER(fileName);
      GA_UNREFERENCED_PARAMETER(entryPoint);
      GA_UNREFERENCED_PARAMETER(versionCS);
      return nullptr;
    };

    /**
    * @brief Function to load and save vertex shader from file.
    * @param
    * @return A shared ptr with the vertex shader created.
    */
    virtual SPtr<VertexShader>
    loadVertexShaderFromFile(const char* vertexFilePath, 
                             const char* vertexMainFuntion,
                             const char* shaderVersion) {
      GA_UNREFERENCED_PARAMETER(vertexFilePath);
      GA_UNREFERENCED_PARAMETER(vertexMainFuntion);
      GA_UNREFERENCED_PARAMETER(shaderVersion); return nullptr;
    };

    /**
     * @brief Function to load and save pixel shader from file.
     * @param
     * @return A shared ptr with the pixel shader created.
     */
    virtual SPtr<PixelShader>
    loadPixelShaderFromFile(const char* pixelFilePath, 
                            const char* pixelMainFuntion,
                            const char* shaderVersion) {
      GA_UNREFERENCED_PARAMETER(pixelFilePath);
      GA_UNREFERENCED_PARAMETER(pixelMainFuntion);
      GA_UNREFERENCED_PARAMETER(shaderVersion); 
      return nullptr;
    };

    /**
    * @brief Function to generate the vertex buffer.
    * @param .
    * @param .
    * @return VertexBuffer pointer.
    */
    virtual VertexBuffer*
    createVertexBuffer(const void* data, const uint32 size) {
      GA_UNREFERENCED_PARAMETER(data); GA_UNREFERENCED_PARAMETER(size); return nullptr;
    };

    /**
    * @brief Function to generate the index buffer.
    * @param .
    * @param .
    * @return IndexBuffer pointer.
    */
    virtual IndexBuffer*
    createIndexBuffer(const void* data, const uint32 size) {
      GA_UNREFERENCED_PARAMETER(data); GA_UNREFERENCED_PARAMETER(size);
      return nullptr;
    };

    /**
    * @brief Function to generate constant buffers.
    * @param .
    * @return ConstantBuffer pointer.
    */
    virtual ConstantBuffer*
    createConstantBuffer(const uint32 bufferSize) {
      GA_UNREFERENCED_PARAMETER(bufferSize);
      return nullptr;
    };

    /*
    * @brief .
    */
    virtual ConstantBuffer*
    createConstantBuffer(const uint32 bufferSize, CPU_ACCESS::E typeCpu, USAGE::E typeUsage) {
      GA_UNREFERENCED_PARAMETER(bufferSize);
      GA_UNREFERENCED_PARAMETER(typeCpu);
      GA_UNREFERENCED_PARAMETER(typeUsage);
      return nullptr;
    };

    /*
    * @brief .
    */
    virtual SPtr<ComputeBuffer>
    createComputeBuffer(const uint32 bufferSize,
                        const uint32 numElements,
                        TEXTURE_BIND_FLAGS::E typeBindFlag = 
                        TEXTURE_BIND_FLAGS::kBindUnorderedAccess, 
                        USAGE::E typeUsage = USAGE::kUsageDefault) {
      GA_UNREFERENCED_PARAMETER(bufferSize);
      GA_UNREFERENCED_PARAMETER(typeBindFlag);
      GA_UNREFERENCED_PARAMETER(typeUsage);
      return nullptr;
    };

    /**
    * @brief Function to generate the following:
    *      ° ShaderResourceView
    *      ° DepthStencilView
    *      ° RenderTargetView.
    * @param width of texture.
    * @param height of texture.
    * @param bindFlags, identifies how to bind a resource to the pipeline.
    * @param textureFormat, resource data formats.
    * @param fileName, where is it supposed to be.
    * @return Textures pointer.
    */
    virtual Textures*
    createTexture(const uint32 width, 
                  const uint32 height, 
                  const uint32 bindFlags,
                  TEXTURE_FORMAT::E textureFormat) {
      GA_UNREFERENCED_PARAMETER(width);
      GA_UNREFERENCED_PARAMETER(height);
      GA_UNREFERENCED_PARAMETER(bindFlags);
      GA_UNREFERENCED_PARAMETER(textureFormat);
      return nullptr;
    };

    /**
    * @brief Function to generate the sampler state.
    * @return SamplerState
    */
    virtual SPtr<SamplerState>
    createSamplerState(FILTER::E typeFilter = 
                       FILTER::kFilterMinMagMipLinear,
                       TEXTURE_ADDRESS::E textureAddress = 
                       TEXTURE_ADDRESS::kTextureAddressWrap,
                       COMPARISON::E typeComparison = 
                       COMPARISON::kComparisonNever) { 
      GA_UNREFERENCED_PARAMETER(typeFilter);
      GA_UNREFERENCED_PARAMETER(textureAddress);
      GA_UNREFERENCED_PARAMETER(typeComparison);
      return nullptr; };

    /**
    * @brief Function to generate the input layout.
    * @param .
    * @return InputLayout pointer.
    */
    virtual InputLayout*
    createInputLayout(WeakSPtr<Shaders> vertexShader) {
      GA_UNREFERENCED_PARAMETER(vertexShader); 
      return nullptr;
    };

    virtual SPtr<RenderTarget>
    createRenderTarget(uint32 width, 
                       uint32 height, 
                       uint32 mipLevels = 1,
                       uint32 numRenderTargets = 1, 
                       float scale = 1.0f,
                       bool depth = false,
                       TEXTURE_FORMAT::E typeTexture = TEXTURE_FORMAT::E::kR16G16B16A16Float,
                       TEXTURE_BIND_FLAGS::E typeBindFlag = TEXTURE_BIND_FLAGS::kDefault) {
      GA_UNREFERENCED_PARAMETER(width);     
      GA_UNREFERENCED_PARAMETER(height);
      GA_UNREFERENCED_PARAMETER(mipLevels); 
      GA_UNREFERENCED_PARAMETER(numRenderTargets);
      GA_UNREFERENCED_PARAMETER(scale);     
      GA_UNREFERENCED_PARAMETER(depth);
      GA_UNREFERENCED_PARAMETER(typeTexture);
      return nullptr;
    };

    /*
    * @brief .
    */
    virtual void
    createMipMaps(WeakSPtr<RenderTarget> renderTarg){ GA_UNREFERENCED_PARAMETER(renderTarg); };

    /*************************************************************************/
    /**
    * Set methods.
    */
    /*************************************************************************/

    /**
    * @brief Function to save the information of the pixel shader.
    * @param .
    */
    virtual void
    setPixelShader(WeakSPtr<Shaders> pixelShader) {
      GA_UNREFERENCED_PARAMETER(pixelShader);
    };

    /**
    * @brief Function to save vertex shader information.
    * @param .
    */
    virtual void
    setVertexShader(WeakSPtr<Shaders> vertexShader) {
      GA_UNREFERENCED_PARAMETER(vertexShader);
    };

    /**
    * @brief Function to save vertex buffer information.
    * @param .
    */
    virtual void
    setVertexBuffer(WeakSPtr<VertexBuffer> vertexBuffer) {
      GA_UNREFERENCED_PARAMETER(vertexBuffer);
    };

    /**
    * @brief Function to save the index buffer information.
    * @param .
    */
    virtual void
    setIndexBuffer(WeakSPtr<IndexBuffer> indexBuffer) {
      GA_UNREFERENCED_PARAMETER(indexBuffer);
    };

    /**
    * @brief Function to save the information of the constant buffers.
    * @param .
    * @param .
    * @param .
    * @param .
    */
    virtual void
    setConstantBuffer(bool isVertex,
                      WeakSPtr<ConstantBuffer> constantBuffer,
                      const uint32 startSlot,
                      const uint32 numBuffers) {
      GA_UNREFERENCED_PARAMETER(isVertex);
      GA_UNREFERENCED_PARAMETER(constantBuffer);
      GA_UNREFERENCED_PARAMETER(startSlot);
      GA_UNREFERENCED_PARAMETER(numBuffers);
    };

    /**
    * @brief Function to save the information of the sampler state.
    * @param .
    * @param .
    * @param .
    * @param .
    */
    virtual void
    setSamplerState(WeakSPtr<SamplerState> sampler, 
                    uint32 startSlot,
                    uint32 numSamplers = 1) {
      GA_UNREFERENCED_PARAMETER(sampler);
      GA_UNREFERENCED_PARAMETER(startSlot);
      GA_UNREFERENCED_PARAMETER(numSamplers);
    };

    /**
    * @brief Function to save the information of the vertex sampler state.
    * @param .
    * @param .
    * @param .
    * @param .
    */
    virtual void
    setSamplerVertexShader(WeakSPtr<SamplerState> sampler,
                           uint32 startSlot,
                           uint32 numSamplers = 1) {
      GA_UNREFERENCED_PARAMETER(sampler);
      GA_UNREFERENCED_PARAMETER(startSlot);
      GA_UNREFERENCED_PARAMETER(numSamplers);
    };

    /**
    * @brief Function to save the information of the vertex sampler state.
    * @param .
    * @param .
    * @param .
    * @param .
    */
    virtual void
    setSamplerPixelShader(WeakSPtr<SamplerState> sampler, 
                          uint32 startSlot,
                          uint32 numSamplers = 1) {
      GA_UNREFERENCED_PARAMETER(sampler);
      GA_UNREFERENCED_PARAMETER(startSlot);
      GA_UNREFERENCED_PARAMETER(numSamplers);
    };

    /**
    * @brief Function to save the information of the shader resource view.
    * @param .
    * @param .
    * @param .
    */
    virtual void
    setShaderResourceView(const Vector<Textures*>& texture,
                          const uint32 startSlot,
                          const uint32 numViews) {
      GA_UNREFERENCED_PARAMETER(texture);
      GA_UNREFERENCED_PARAMETER(startSlot);
      GA_UNREFERENCED_PARAMETER(numViews);
    };

    virtual void
    setShaderResourceView(void* renderTexture,
                          const uint32 startSlot,
                          const uint32 numViews = 1) {
      GA_UNREFERENCED_PARAMETER(renderTexture);
      GA_UNREFERENCED_PARAMETER(startSlot);
      GA_UNREFERENCED_PARAMETER(numViews);
    };

    virtual void
    csSetShaderResource(void* renderTexture,
                        const uint32 startSlot,
                        const uint32 numViews = 1) {
      GA_UNREFERENCED_PARAMETER(renderTexture);
      GA_UNREFERENCED_PARAMETER(startSlot);
      GA_UNREFERENCED_PARAMETER(numViews);
    };

    /**
    * @brief Function to save the information of the render target.
    * @param .
    * @param .
    */
    virtual void
    setRenderTarget(WeakSPtr<Textures> renderTarget, WeakSPtr<Textures> depthStencil) {
      GA_UNREFERENCED_PARAMETER(renderTarget); GA_UNREFERENCED_PARAMETER(depthStencil);
    };

    virtual void
    setRenderTarget(WeakSPtr<Textures> renderTarget) {
      GA_UNREFERENCED_PARAMETER(renderTarget);
    };

    /**
    * @brief Function to save the information of the render target.
    * @param .
    * @param .
    */
    virtual void
    setRenderTarget(WeakSPtr<RenderTarget> renderTarget, WeakSPtr<Textures> depthStencil) {
      GA_UNREFERENCED_PARAMETER(renderTarget); 
      GA_UNREFERENCED_PARAMETER(depthStencil);
    };

    virtual void
    setRenderTarget(WeakSPtr<RenderTarget> renderTarget) {
      GA_UNREFERENCED_PARAMETER(renderTarget);
    };

    /**
    * @brief Function to save the depth stencil information.
    * @param .
    * @param .
    */
    virtual void
    setDepthStencil(WeakSPtr<Textures> depthStencil, const uint32 stencilRef) {
      GA_UNREFERENCED_PARAMETER(depthStencil); GA_UNREFERENCED_PARAMETER(stencilRef);
    };

    /**
    * @brief Function to save the information of the input layout.
    * @param .
    */
    virtual void
    setInputLayout(WeakSPtr<InputLayout> vertexLayout) {
      GA_UNREFERENCED_PARAMETER(vertexLayout);
    };

    /**
    * @brief Function to save the viewport information.
    * @param .
    * @param .
    * @param .
    */
    virtual void
    setViewports(const uint32 width, const uint32 heigth, const uint32 numViewports = 1) {
      GA_UNREFERENCED_PARAMETER(width);
      GA_UNREFERENCED_PARAMETER(heigth);
      GA_UNREFERENCED_PARAMETER(numViewports);
    };

    /**
    * @brief Function to save the topology information.
    * @param .
    */
    virtual void
    setPrimitiveTopology(PRIMITIVE_TOPOLOGY::E topology = PRIMITIVE_TOPOLOGY::kTriangleList) {
      GA_UNREFERENCED_PARAMETER(topology);
    };

    /**
    * @brief Function to call VSSetShader.
    * @param .
    */
    virtual void
    setYourVS(WeakSPtr<Shaders> vertexShader) { GA_UNREFERENCED_PARAMETER(vertexShader); };

    /**
    * @brief Function to call VSSetConstantBuffers.
    * @param .
    * @param .
    * @param .
    */
    virtual void
    setYourVSConstantBuffers(WeakSPtr<ConstantBuffer> constantBuffer, 
                             const uint32 startSlot,
                             const uint32 numBuffers = 1) {
      GA_UNREFERENCED_PARAMETER(constantBuffer);
      GA_UNREFERENCED_PARAMETER(startSlot);
      GA_UNREFERENCED_PARAMETER(numBuffers);
    };

    /**
    * @brief Function to call PSSetShader.
    * @param .
    */
    virtual void
    setYourPS(WeakSPtr<Shaders> pixelShader) { GA_UNREFERENCED_PARAMETER(pixelShader); };

    /**
    * @brief Function to call PSSetConstantBuffers.
    * @param .
    * @param .
    * @param .
    */
    virtual void
    setYourPSConstantBuffers(WeakSPtr<ConstantBuffer> constantBuffer, 
                             const uint32 startSlot,
                             const uint32 numBuffers = 1) {
      GA_UNREFERENCED_PARAMETER(constantBuffer);
      GA_UNREFERENCED_PARAMETER(startSlot);
      GA_UNREFERENCED_PARAMETER(numBuffers);
    };

    /**
    * @brief Function to call PSSetSamplers.
    * @param .
    * @param .
    * @param .
    */
    virtual void
    setYourPSSampler(WeakSPtr<SamplerState> sampler, const uint32 startSlot,
      const uint32 numSamplers) {
      GA_UNREFERENCED_PARAMETER(sampler);
      GA_UNREFERENCED_PARAMETER(startSlot);
      GA_UNREFERENCED_PARAMETER(numSamplers);
    };

    /**
    * @brief Function to call program and link for OGL.
    * @param .
    */
    virtual void
    setShaders(WeakSPtr<Shaders> shaders) { GA_UNREFERENCED_PARAMETER(shaders); };

    /**
    * @brief Function to set constant buffer for bones.
    * @param
    */
    virtual void
    setConstBufferBones(WeakSPtr<ConstantBuffer> cbBones) {
      GA_UNREFERENCED_PARAMETER(cbBones);
    };

    virtual void
    setComputeUAV(WeakSPtr<ComputeBuffer> pComBuff,
                  uint32 startSlot = 0,
                  uint32 numUAV = 1) {
      GA_UNREFERENCED_PARAMETER(pComBuff);
      GA_UNREFERENCED_PARAMETER(startSlot);
      GA_UNREFERENCED_PARAMETER(numUAV);
    };

    virtual void
    setRtUAV(WeakSPtr<RenderTarget> pCompBuffRT,
             uint32 startSlot = 0,
             uint32 numUAV = 1) {
      GA_UNREFERENCED_PARAMETER(pCompBuffRT);
      GA_UNREFERENCED_PARAMETER(startSlot);
      GA_UNREFERENCED_PARAMETER(numUAV);
    };

    virtual void
    setComputeShader(WeakSPtr<Shaders> shader) {
      GA_UNREFERENCED_PARAMETER(shader);
    };

    virtual void
    setCSConstantBuffer(WeakSPtr<ConstantBuffer> constBuffer,
                        const uint32 startSlot,
                        const uint32 numBuffers = 1) {
      GA_UNREFERENCED_PARAMETER(constBuffer);
      GA_UNREFERENCED_PARAMETER(startSlot);
      GA_UNREFERENCED_PARAMETER(numBuffers);
    };

    virtual void
    setCSSampler(WeakSPtr<SamplerState> sampler,
                 uint32 startSlot,
                 uint32 numSamplers = 1) {
      GA_UNREFERENCED_PARAMETER(sampler);
      GA_UNREFERENCED_PARAMETER(startSlot);
      GA_UNREFERENCED_PARAMETER(numSamplers);
    };

    /*************************************************************************/
    /**
    * Gets.
    */
    /*************************************************************************/

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

    /**
    * @brief Function to get constant buffer for bones.
    * @return Smart pointer with CB info.
    */
    virtual SPtr<ConstantBuffer>
    getConstBufferBones() { return nullptr; };

    /**
    * @brief get ptr to device
    * @return share ptr to device
    */
    virtual void*
    getDevice() { return nullptr; };

    /**
    * @brief get ptr to device context
    * @return share ptr to device context
    */
    virtual void*
    getDeviceContext() { return nullptr; };

    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /**
    * @brief Member to save screen width.
    */
    uint32 m_width = 0;

    /**
    * @brief Member to save the screen height.
    */
    uint32 m_height = 0;

    /**
    * @brief Member to save the bones.
    */
    SPtr<ConstantBuffer> m_bonesBuffer = nullptr;
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