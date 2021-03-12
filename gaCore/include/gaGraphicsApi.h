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
  using TEXTURE_BIND_FLAG = enum
{
 TEXTURE_BIND_SHADER_RESOURCE = 0x8L,
 TEXTURE_BIND_RENDER_TARGET  = 0x20L,
 TEXTURE_BIND_DEPTH_STENCIL  = 0x40L,
 TEXTURE_BIND_UNORDERED_ACCESS = 0x80L,
};
  
  /**
  * @brief Enum with all numbers with format type for DX.
  */
  using TEXTURE_FORMAT = enum
{
 TEXTURE_FORMAT_UNKNOWN = 0,
 TEXTURE_FORMAT_R32G32B32A32_TYPELESS = 1,
 TEXTURE_FORMAT_R32G32B32A32_FLOAT = 2,
 TEXTURE_FORMAT_R32G32B32A32_UINT = 3,
 TEXTURE_FORMAT_R32G32B32A32_SINT = 4,
 TEXTURE_FORMAT_R32G32B32_TYPELESS = 5,
 TEXTURE_FORMAT_R32G32B32_FLOAT = 6,
 TEXTURE_FORMAT_R32G32B32_UINT = 7,
 TEXTURE_FORMAT_R32G32B32_SINT = 8,
 TEXTURE_FORMAT_R16G16B16A16_TYPELESS = 9,
 TEXTURE_FORMAT_R16G16B16A16_FLOAT = 10,
 TEXTURE_FORMAT_R16G16B16A16_UNORM = 11,
 TEXTURE_FORMAT_R16G16B16A16_UINT = 12,
 TEXTURE_FORMAT_R16G16B16A16_SNORM = 13,
 TEXTURE_FORMAT_R16G16B16A16_SINT = 14,
 TEXTURE_FORMAT_R32G32_TYPELESS = 15,
 TEXTURE_FORMAT_R32G32_FLOAT = 16,
 TEXTURE_FORMAT_R32G32_UINT = 17,
 TEXTURE_FORMAT_R32G32_SINT = 18,
 TEXTURE_FORMAT_R32G8X24_TYPELESS = 19,
 TEXTURE_FORMAT_D32_FLOAT_S8X24_UINT = 20,
 TEXTURE_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
 TEXTURE_FORMAT_X32_TYPELESS_G8X24_UINT = 22,
 TEXTURE_FORMAT_R10G10B10A2_TYPELESS = 23,
 TEXTURE_FORMAT_R10G10B10A2_UNORM = 24,
 TEXTURE_FORMAT_R10G10B10A2_UINT = 25,
 TEXTURE_FORMAT_R11G11B10_FLOAT = 26,
 TEXTURE_FORMAT_R8G8B8A8_TYPELESS = 27,
 TEXTURE_FORMAT_R8G8B8A8_UNORM = 28,
 TEXTURE_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
 TEXTURE_FORMAT_R8G8B8A8_UINT = 30,
 TEXTURE_FORMAT_R8G8B8A8_SNORM = 31,
 TEXTURE_FORMAT_R8G8B8A8_SINT = 32,
 TEXTURE_FORMAT_R16G16_TYPELESS = 33,
 TEXTURE_FORMAT_R16G16_FLOAT = 34,
 TEXTURE_FORMAT_R16G16_UNORM = 35,
 TEXTURE_FORMAT_R16G16_UINT = 36,
 TEXTURE_FORMAT_R16G16_SNORM = 37,
 TEXTURE_FORMAT_R16G16_SINT = 38,
 TEXTURE_FORMAT_R32_TYPELESS = 39,
 TEXTURE_FORMAT_D32_FLOAT = 40,
 TEXTURE_FORMAT_R32_FLOAT = 41,
 TEXTURE_FORMAT_R32_UINT = 42,
 TEXTURE_FORMAT_R32_SINT = 43,
 TEXTURE_FORMAT_R24G8_TYPELESS = 44,
 TEXTURE_FORMAT_D24_UNORM_S8_UINT = 45,
 TEXTURE_FORMAT_R24_UNORM_X8_TYPELESS = 46,
 TEXTURE_FORMAT_X24_TYPELESS_G8_UINT = 47,
 TEXTURE_FORMAT_R8G8_TYPELESS = 48,
 TEXTURE_FORMAT_R8G8_UNORM = 49,
 TEXTURE_FORMAT_R8G8_UINT = 50,
 TEXTURE_FORMAT_R8G8_SNORM = 51,
 TEXTURE_FORMAT_R8G8_SINT = 52,
 TEXTURE_FORMAT_R16_TYPELESS = 53,
 TEXTURE_FORMAT_R16_FLOAT = 54,
 TEXTURE_FORMAT_D16_UNORM = 55,
 TEXTURE_FORMAT_R16_UNORM = 56,
 TEXTURE_FORMAT_R16_UINT = 57,
 TEXTURE_FORMAT_R16_SNORM = 58,
 TEXTURE_FORMAT_R16_SINT = 59,
 TEXTURE_FORMAT_R8_TYPELESS = 60,
 TEXTURE_FORMAT_R8_UNORM = 61,
 TEXTURE_FORMAT_R8_UINT = 62,
 TEXTURE_FORMAT_R8_SNORM = 63,
 TEXTURE_FORMAT_R8_SINT = 64,
 TEXTURE_FORMAT_A8_UNORM = 65,
 TEXTURE_FORMAT_R1_UNORM = 66,
 TEXTURE_FORMAT_R9G9B9E5_SHAREDEXP = 67,
 TEXTURE_FORMAT_R8G8_B8G8_UNORM = 68,
 TEXTURE_FORMAT_G8R8_G8B8_UNORM = 69,
 TEXTURE_FORMAT_BC1_TYPELESS = 70,
 TEXTURE_FORMAT_BC1_UNORM = 71,
 TEXTURE_FORMAT_BC1_UNORM_SRGB = 72,
 TEXTURE_FORMAT_BC2_TYPELESS = 73,
 TEXTURE_FORMAT_BC2_UNORM = 74,
 TEXTURE_FORMAT_BC2_UNORM_SRGB = 75,
 TEXTURE_FORMAT_BC3_TYPELESS = 76,
 TEXTURE_FORMAT_BC3_UNORM = 77,
 TEXTURE_FORMAT_BC3_UNORM_SRGB = 78,
 TEXTURE_FORMAT_BC4_TYPELESS = 79,
 TEXTURE_FORMAT_BC4_UNORM = 80,
 TEXTURE_FORMAT_BC4_SNORM = 81,
 TEXTURE_FORMAT_BC5_TYPELESS = 82,
 TEXTURE_FORMAT_BC5_UNORM = 83,
 TEXTURE_FORMAT_BC5_SNORM = 84,
 TEXTURE_FORMAT_B5G6R5_UNORM = 85,
 TEXTURE_FORMAT_B5G5R5A1_UNORM = 86,
 TEXTURE_FORMAT_B8G8R8A8_UNORM = 87,
 TEXTURE_FORMAT_B8G8R8X8_UNORM = 88,
 TEXTURE_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
 TEXTURE_FORMAT_B8G8R8A8_TYPELESS = 90,
 TEXTURE_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
 TEXTURE_FORMAT_B8G8R8X8_TYPELESS = 92,
 TEXTURE_FORMAT_B8G8R8X8_UNORM_SRGB = 93,
 TEXTURE_FORMAT_BC6H_TYPELESS = 94,
 TEXTURE_FORMAT_BC6H_UF16 = 95,
 TEXTURE_FORMAT_BC6H_SF16 = 96,
 TEXTURE_FORMAT_BC7_TYPELESS = 97,
 TEXTURE_FORMAT_BC7_UNORM = 98,
 TEXTURE_FORMAT_BC7_UNORM_SRGB = 99,
};
  
  /**
  * @brief Enum with all numbers with topology type for DX.
  */
  using PRIMITIVE_TOPOLOGY = enum
{
 PRIMITIVE_TOPOLOGY_POINTLIST = 1,
 PRIMITIVE_TOPOLOGY_LINELIST = 2,
 PRIMITIVE_TOPOLOGY_LINESTRIP = 3,
 PRIMITIVE_TOPOLOGY_TRIANGLELIST = 4,
 PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = 5,
};

  struct Vertex {
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
      *        � ShaderResourceView 
      *        � DepthStencilView 
      *        � RenderTargetView.
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
                    TEXTURE_FORMAT textureFormat,
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