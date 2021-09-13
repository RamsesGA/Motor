#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>

#include <gaGraphicsApi.h>
#include <gaConstantBuffer.h>
#include <gaTextures.h>
#include <gaShaders.h>
#include <gaVertexShader.h>
#include <gaPixelShader.h>
#include <gaVertexBuffer.h>
#include <gaIndexBuffer.h>
#include <gaRenderTarget.h>
#include <SFML/Window.hpp>

#include "gaPrerequisitesCore.h"

using sf::WindowHandle;

namespace gaEngineSDK {
  /*
  * @brief This class is a child of the Graphic Api parent class,
  *      all comments on members or methods are found in their respective .h.
  */

  class GraphicsApiDX final : public GraphicsApi
  {
   public:
    /*************************************************************************/
    /**
    * Constructor and destructor.
    */
    /*************************************************************************/

    GraphicsApiDX() = default;

    ~GraphicsApiDX();

    /*************************************************************************/
    /**
    * Inheritance methods.
    */
    /*************************************************************************/

    bool
    initDevice(sf::WindowHandle hWnd) override;

    void
    drawIndex(uint32 indexCount, uint32 startIndexLocation = 0, 
              uint32 baseVertexLocation = 0) override;

    void
    swapChainPresent(uint32 syncInterval = 0, uint32 flags = 0) override;

    Textures*
    loadTextureFromFile(String srcFile) override;

    void
    unbindOGL() override;

    Matrix4x4
    matrixPolicy(const Matrix4x4& mat4x4) override;

    /*************************************************************************/
    /**
    * Updates methods.
    */
    /*************************************************************************/

    void
    updateConstantBuffer(const void* srcData, WeakSPtr<ConstantBuffer> updateDataCB) override;

    /*************************************************************************/
    /**
    * Clear methods.
    */
    /*************************************************************************/

    void
    clearYourRenderTargetView(WeakSPtr<Textures> renderTarget, Vector4 rgba) override;

    void
    clearYourRenderTarget(WeakSPtr<RenderTarget> renderTarget, Vector4 rgba) override;

    void
    clearYourDepthStencilView(WeakSPtr<Textures> depthStencil) override;

    /*************************************************************************/
    /**
    * Create methods.
    */
    /*************************************************************************/

    Shaders*
    createShadersProgram(const WString& nameVS, 
                         const String& entryPointVS,
                         const WString& namePS, 
                         const String& entryPointPS) override;

    SPtr<VertexShader>
    loadVertexShaderFromFile(const char* vertexFilePath,
                             const char* vertexMainFuntion,
                             const char* shaderVersion) override;

    SPtr<PixelShader>
    loadPixelShaderFromFile(const char* pixelFilePath, 
                            const char* pixelMainFuntion,
                            const char* shaderVersion) override;

    VertexBuffer*
    createVertexBuffer(const void* data, const uint32 size) override;

    IndexBuffer*
    createIndexBuffer(const void* data, const uint32 size) override;

    ConstantBuffer*
    createConstantBuffer(const uint32 bufferSize) override;

    Textures*
    createTexture(const uint32 width, 
                  const uint32 height, 
                  const uint32 bindFlags,
                  TEXTURE_FORMAT::E textureFormat, 
                  const String fileName) override;

    SPtr<SamplerState>
    createSamplerState() override;

    InputLayout*
    createInputLayout(WeakSPtr<Shaders> vertexShader) override;

    SPtr<RenderTarget>
    createRenderTarget(uint32 width, 
                       uint32 height, 
                       uint32 mipLevels = 1,
                       uint32 numRenderTargets = 1, 
                       float scale = 1.0f,
                       bool depth = false) override;

    /*************************************************************************/
    /**
    * Set methods.
    */
    /*************************************************************************/

    void
    setPixelShader(WeakSPtr<Shaders> pixelShader) override;

    void
    setVertexShader(WeakSPtr<Shaders> vertexShader) override;

    void
    setVertexBuffer(WeakSPtr<VertexBuffer> vertexBuffer) override;

    void
    setIndexBuffer(WeakSPtr<IndexBuffer> indexBuffer) override;

    void
    setConstantBuffer(bool isVertex, 
                      WeakSPtr<ConstantBuffer> constantBuffer,
                      const uint32 startSlot, 
                      const uint32 numBuffers) override;

    void
    setSamplerState(WeakSPtr<SamplerState> sampler, 
                    uint32 startSlot,
                    uint32 numSamplers = 1) override;

    void
    setSamplerVertexShader(WeakSPtr<SamplerState> sampler,
                           uint32 startSlot,
                           uint32 numSamplers = 1) override;

    void
    setSamplerPixelShader(WeakSPtr<SamplerState> sampler, 
                          uint32 startSlot,
                          uint32 numSamplers = 1) override;

    void
    setShaderResourceView(const Vector<Textures*>& texture,
                          const uint32 startSlot,
                          const uint32 numViews) override;

    void
    setShaderResourceView(void* renderTexture,
                          const uint32 startSlot,
                          const uint32 numViews = 1) override;

    void
    setRenderTarget(WeakSPtr<Textures> renderTarget, 
                    WeakSPtr<Textures> depthStencil) override;

    void
    setRenderTarget(WeakSPtr<Textures> renderTarget) override;

    void
    setRenderTarget(WeakSPtr<RenderTarget> renderTarget,
                    WeakSPtr<Textures> depthStencil) override;

    void
    setRenderTarget(WeakSPtr<RenderTarget> renderTarget) override;

    void
    setDepthStencil(WeakSPtr<Textures> depthStencil, const uint32 stencilRef) override;

    void
    setInputLayout(WeakSPtr<InputLayout> vertexLayout) override;

    void
    setViewports(const uint32 width, 
                 const uint32 heigth, 
                 const uint32 numViewports = 1) override;

    void
    setPrimitiveTopology(const uint32 topology = PRIMITIVE_TOPOLOGY::kTriangleList) override;

    void
    setYourVS(WeakSPtr<Shaders> vertexShader) override;

    void
    setYourVSConstantBuffers(WeakSPtr<ConstantBuffer> constantBuffer,
                             const uint32 startSlot,
                             const uint32 numBuffers = 1) override;

    void
    setYourPS(WeakSPtr<Shaders> pixelShader) override;

    void
    setYourPSConstantBuffers(WeakSPtr<ConstantBuffer> constantBuffer, 
                             const uint32 startSlot,
                             const uint32 numBuffers = 1) override;

    void
    setYourPSSampler(WeakSPtr<SamplerState> sampler, 
                     const uint32 startSlot,
                     const uint32 numSamplers) override;

    void
    setShaders(WeakSPtr<Shaders> shaders) override;

    void
    setConstBufferBones(WeakSPtr<ConstantBuffer> cbBones) override;

    /*************************************************************************/
    /**
    * Gets.
    */
    /*************************************************************************/

    Textures*
    getDefaultBackBuffer() override;

    Textures*
    getDefaultDepthStencil() override;

    SPtr<ConstantBuffer>
    getConstBufferBones() override;

    void*
    getDevice() override;

    void*
    getDeviceContext() override;

   private:
    /*************************************************************************/
    /**
    * Members.
    */
    /*************************************************************************/

    /*
    * @brief Member with window information brought from user.
    */
    WindowHandle m_hWnd;

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
    ID3D11DeviceContext* m_pDeviceContext = nullptr;

    /*
    * @brief Member to store and access a texture resource during in-depth template testing.
    */
    Textures* m_pDepthStencil = nullptr;

    /*
    * @brief Member to store the default texture when creating the device.
    */
    Textures* m_pBackBuffer = nullptr;

    /*
    * @brief .
    */
    Vector<SPtr<RenderTarget>> m_vRenderTargets;
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