#pragma once

#include <windows.h>
#include <gaVector3.h>
#include <gaVector2.h>

#include <gaPrerequisitesUtilities.h>

//#include "Structures.h"
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix transform.hpp"

namespace gaEngineSDK {

  /**
  * @brief Enum with all numbers with the type of bind flag for DX.
  */
  namespace TEXTURE_BIND_FLAG {
    enum class E {
      kShaderResource = 0x8L,
      kRenderTarget = 0x20L,
      kDepthStencil = 0x40L,
      kUnorderedAccess = 0x80L,
    };
  }

  /**
  * @brief Enum with all numbers with format type for DX.
  */
  namespace TEXTURE_FORMAT {
    enum class E {
      kUnknown = 0,
      kR32G32B32A32Typeless = 1,
      kR32G32B32A32Float = 2,
      kR32G32B32A32UInt = 3,
      kR32G32B32A32SInt = 4,
      kR32G32B32Typeless = 5,
      kR32G32B32Float = 6,
      kR32G32B32UInt = 7,
      kR32G32B32SInt = 8,
      kR16G16B16A16Typeless = 9,
      kR16G16B16A16Float = 10,
      kR16G16B16A16UNorm = 11,
      kR16G16B16A16UInt = 12,
      kR16G16B16A16SNorm = 13,
      kR16G16B16A16SInt = 14,
      kR32G32Typeless = 15,
      kR32G32Float = 16,
      kR32G32UInt = 17,
      kR32G32SInt = 18,
      kR32G8X24Typeless = 19,
      kD32FloatS8X24UInt = 20,
      kR32FloatX8X24Typeless = 21,
      kX32TypelessG8X24UInt = 22,
      kR10G10B10A2Typeless = 23,
      kR10G10B10A2UNorm = 24,
      kR10G10B10A2UInt = 25,
      kR11G11B10Float = 26,
      kR8G8B8A8Typeless = 27,
      kR8G8B8A8UNorm = 28,
      kR8G8B8A8UNorm_SRGB = 29,
      kR8G8B8A8UInt = 30,
      kR8G8B8A8SNorm = 31,
      kR8G8B8A8SInt = 32,
      kR16G16Typeless = 33,
      kR16G16Float = 34,
      kR16G16UNorm = 35,
      kR16G16UInt = 36,
      kR16G16SNorm = 37,
      kR16G16SInt = 38,
      kR32Typeless = 39,
      kD32Float = 40,
      kR32Float = 41,
      kR32UInt = 42,
      kR32SInt = 43,
      kR24G8Typeless = 44,
      kD24UNormS8UInt = 45,
      kR24UNormX8Typeless = 46,
      kX24TypelessG8UInt = 47,
      kR8G8Typeless = 48,
      kR8G8UNorm = 49,
      kR8G8UInt = 50,
      kR8G8SNorm = 51,
      kR8G8SInt = 52,
      kR16Typeless = 53,
      kR16Float = 54,
      kD16UNorm = 55,
      kR16UNorm = 56,
      kR16UInt = 57,
      kR16SNorm = 58,
      kR16SInt = 59,
      kR8Typeless = 60,
      kR8UNorm = 61,
      kR8UInt = 62,
      kR8SNorm = 63,
      kR8SInt = 64,
      kA8UNorm = 65,
      kR1UNorm = 66,
      kR9G9B9E5SHAREDEXP = 67,
      kR8G8B8G8UNorm = 68,
      kG8R8G8B8UNorm = 69,
      kBC1Typeless = 70,
      kBC1UNorm = 71,
      kBC1UNorm_SRGB = 72,
      kBC2Typeless = 73,
      kBC2UNorm = 74,
      kBC2UNorm_SRGB = 75,
      kBC3Typeless = 76,
      kBC3UNorm = 77,
      kBC3UNormSRGB = 78,
      kBC4Typeless = 79,
      kBC4UNorm = 80,
      kBC4SNorm = 81,
      kBC5Typeless = 82,
      kBC5UNorm = 83,
      kBC5SNorm = 84,
      kB5G6R5UNorm = 85,
      kB5G5R5A1UNorm = 86,
      kB8G8R8A8UNorm = 87,
      kB8G8R8X8UNorm = 88,
      kR10G10B10XRBiasA2UNorm = 89,
      kB8G8R8A8Typeless = 90,
      kB8G8R8A8UNormSRGB = 91,
      kB8G8R8X8Typeless = 92,
      kB8G8R8X8UNormSRGB = 93,
      kBC6HTypeless = 94,
      kBC6HUF16 = 95,
      kBC6HSF16 = 96,
      kBC7Typeless = 97,
      kBC7UNorm = 98,
      kBC7UNormSRGB = 99,
    };
  }

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

  /**
  * @brief 
  */
  struct Vertex 
  {
    Vector3 position;
    Vector2 texCoords;
    Vector3 normal;
    Vector3 tangent;
    Vector3 bitangent;
  };
  
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
  class GraphicsApi 
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
      
      /**
      * @brief Function to initialize the device and generate 
      *    the back buffer and depth stencil for DX by default
      * @param hWnd handle to a window.
      * @return true or false.
      */
      virtual bool 
      initDevice(HWND hWnd) = 0;
      
      /**
      * @brief Function to send to draw the indices of a declared object.
      * @param indexCount 
      *        startIndexLocation 
      *        baseVertexLocation 
      */
      virtual void 
      drawIndex(uint32 indexCount,
                uint32 startIndexLocation,
                uint32 baseVertexLocation) = 0;
      
      /**
      * @brief Function to exchange buffers and update your information.
      * @param syncInterval 
      *        flags 
      */
      virtual void 
      swapChainPresent(uint32 syncInterval,
                       uint32 flags) = 0;
      
      /**
      * @brief Function to load textures in file.
      * @param srcFile
      * @return Textures.
      */
      virtual Textures* 
      loadTextureFromFile(std::string srcFile) = 0;
      
      /**
      * @brief OGL function to separate a program.
      */
      virtual void 
      unbindOGL() = 0;
      
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
                           ConstantBuffer& updateDataCB) = 0;
      
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
                                float r, float g, float b, float a) = 0;
      
      /**
      * @brief Function to clean our depth stencil view.
      * @param depthStencil
      */
      virtual void 
      clearYourDepthStencilView(Textures* depthStencil) = 0;
      
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
                           const std::string& entryPointPS) = 0;
      
      /**
      * @brief Function to generate the vertex buffer.
      * @param data
      *        size
      * @return gaVertexBuffer
      */
      virtual VertexBuffer* 
      createVertexBuffer(const void* data,
                         const uint32 size) = 0;
      
      /**
      * @brief Function to generate the index buffer.
      * @param data
      *        size
      * @return gaIndexBuffer
      */
      virtual IndexBuffer*
      createIndexBuffer(const void* data,
                        const uint32 size) = 0;
      
      /**
      * @brief Function to generate constant buffers.
      * @param bufferSize
      * @return ConstantBuffer
      */
      virtual ConstantBuffer* 
      createConstantBuffer(const uint32 bufferSize) = 0;
      
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
                    const std::string fileName) = 0;
      
      /**
      * @brief Function to generate the sampler state.
      * @return SamplerState
      */
      virtual SamplerState* 
      createSamplerState() = 0;
      
      /**
      * @brief Function to generate the input layout.
      * @param vertexShader
      * @return InputLayout
      */
      virtual InputLayout* 
      createInputLayout(Shaders& vertexShader) = 0;
      
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
      setPixelShader(Shaders& pixelShader) = 0;
      
      /**
      * @brief Function to save vertex shader information.
      * @param vertexShader
      */
      virtual void 
      setVertexShader(Shaders& vertexShader) = 0;
      
      /**
      * @brief Function to save vertex buffer information.
      * @param vertexBuffer
      */
      virtual void 
      setVertexBuffer(VertexBuffer& vertexBuffer) = 0;
      
      /**
      * @brief Function to save the index buffer information.
      * @param indexBuffer
      */
      virtual void 
      setIndexBuffer(IndexBuffer& indexBuffer) = 0;
      
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
                        const uint32 numBuffers) = 0;
      
      /**
      * @brief Function to save the information of the sampler state.
      * @param startSlot
      *        samplerState
      *        texture
      */
      virtual void 
      setSamplerState(const uint32 startSlot,
                      std::vector<SamplerState*>& samplerState,
                      Textures& texture) = 0;
      
      /**
      * @brief Function to save the information of the shader resource view.
      * @param shaderResourceView
      *        startSlot
      *        numViews
      */
      virtual void 
      setShaderResourceView(Textures* shaderResourceView,
                            const uint32 startSlot,
                            const uint32 numViews) = 0;
      
      /**
      * @brief Function to save the information of the render target.
      * @param renderTarget
      *        depthStencil
      */
      virtual void 
      setRenderTarget(Textures* renderTarget,
                      Textures* depthStencil) = 0;
      
      /**
      * @brief Function to save the depth stencil information.
      * @param depthStencil
      *        stencilRef
      */
      virtual void 
      setDepthStencil(Textures& depthStencil,
                      const uint32 stencilRef) = 0;
      
      /**
      * @brief Function to save the information of the input layout.
      * @param vertexLayout
      */
      virtual void 
      setInputLayout(InputLayout& vertexLayout) = 0;
      
      /**
      * @brief Function to save the viewport information.
      * @param numViewports
      *        width
      *        heigth
      */
      virtual void 
      setViewport(const uint32 numViewports,
                  const uint32 width, 
                  const uint32 heigth) = 0;
      
      /**
      * @brief Function to save the topology information.
      * @param topology
      */
      virtual void 
      setPrimitiveTopology(const uint32 topology) = 0;
      
      /**
      * @brief Function to call VSSetShader.
      * @param vertexShader
      */
      virtual void 
      setYourVS(Shaders& vertexShader) = 0;
      
      /**
      * @brief Function to call VSSetConstantBuffers.
      * @param constantBuffer
      *        startSlot
      *        numBuffers
      */
      virtual void 
      setYourVSConstantBuffers(ConstantBuffer* constantBuffer,
                               const uint32 startSlot,
                               const uint32 numBuffers) = 0;
      
      /**
      * @brief Function to call PSSetShader.
      * @param pixelShader
      */
      virtual void 
      setYourPS(Shaders& pixelShader) = 0;
      
      /**
      * @brief Function to call PSSetConstantBuffers.
      * @param constantBuffer
      *        startSlot
      *        numBuffers
      */
      virtual void 
      setYourPSConstantBuffers(ConstantBuffer* constantBuffer,
                               const uint32 startSlot,
                               const uint32 numBuffers) = 0;
      
      /**
      * @brief Function to call PSSetSamplers.
      * @param sampler
      *        startSlot
      *        numSamplers
      */
      virtual void 
      setYourPSSampler(SamplerState& sampler,
                       const uint32 startSlot,
                       const uint32 numSamplers) = 0;
      
      /**
      * @brief Function to call program and link for OGL.
      * @param shaders
      */
      virtual void 
      setShaders(Shaders& shaders) = 0;
      
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
      getDefaultBackBuffer() = 0;
      
      /**
      * @brief Function to obtain the automatically generated depth stencil.
      * @return Textures.
      */
      virtual Textures*
      getDefaultDepthStencil() = 0;

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
}