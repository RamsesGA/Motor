#include <exception>
#include <gaMatrix4x4.h>
#include <comdef.h>

#include "gaGraphicsApiDX.h"
#include "gaConstantBufferDX.h"
#include "gaTexturesDX.h"
#include "gaSamplerStateDX.h"
#include "gaInputLayoutDX.h"
#include "gaShadersDX.h"
#include "gaVertexBufferDX.h"
#include "gaIndexBufferDX.h"
#include "gaRenderTargetDX.h"
#include "stb_image.h"
#include "gaComputeShaderDX.h"
#include "gaComputeBufferDX.h"

namespace gaEngineSDK {
  HRESULT
  CompileShaderFromFile(const WString& szFileName, 
                        const String& szEntryPoint,
                        const String& szShaderModel, 
                        ID3DBlob** ppBlobOut) {
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DCompileFromFile(szFileName.c_str(),
                            nullptr,  
                            nullptr,
                            szEntryPoint.c_str(),
                            szShaderModel.c_str(),
                            dwShaderFlags, 
                            0, 
                            ppBlobOut,
                            &pErrorBlob);

    if (FAILED(hr)) {
      if (pErrorBlob != nullptr) {
        OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
      }
      if (pErrorBlob) {
        pErrorBlob->Release();
      }
      return hr;
    }

    if (pErrorBlob) {
      pErrorBlob->Release();
    }

    return S_OK;
  }

  bool
  AnalyzeVertexShaderDX(const WString& nameVS) {
    String bufferAnalyze;

    uint32 tempVSsize = nameVS.size();
    for (uint32 i = 0; i < tempVSsize; ++i) {
      bufferAnalyze += nameVS[i];

      if (('_' == bufferAnalyze[i]) &&
        ("data/shaders/DX_" != bufferAnalyze)) {
        return false;
      }
      else if (('_' == bufferAnalyze[i]) &&
        ("data/shaders/DX_" == bufferAnalyze)) {
        return true;
      }
    }

    return false;
  }

  bool
  AnalyzePixelShaderDX(const WString& namePS) {
    String bufferAnalyze;

    uint32 tempPSsize = namePS.size();
    for (uint32 i = 0; i < tempPSsize; ++i) {
      bufferAnalyze += namePS[i];

      if (('_' == bufferAnalyze[i]) &&
        ("data/shaders/DX_" != bufferAnalyze)) {
        return false;
      }
      else if (('_' == bufferAnalyze[i]) &&
        ("data/shaders/DX_" == bufferAnalyze)) {
        return true;
      }
    }

    return false;
  }

  /***************************************************************************/
  /**
  * Destructor.
  */
  /***************************************************************************/

  GraphicsApiDX::~GraphicsApiDX() {
    delete m_pDepthStencil;
    delete m_pBackBuffer;
  }

  /***************************************************************************/
  /**
  * Inheritance methods.
  */
  /***************************************************************************/

  bool
  GraphicsApiDX::initDevice(sf::WindowHandle hWnd) {

    HRESULT hr = S_OK;

    RECT rc;

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    m_hWnd = hWnd;

    GetClientRect(m_hWnd, &rc);

    m_width = rc.right - rc.left;
    m_height = rc.bottom - rc.top;

    uint32 createDeviceFlags = 0;
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

    Vector<D3D_DRIVER_TYPE> driverTypes =
    {
      D3D_DRIVER_TYPE_HARDWARE,
      D3D_DRIVER_TYPE_WARP,
      D3D_DRIVER_TYPE_REFERENCE,
    };

    Vector<D3D_FEATURE_LEVEL> featureLevels =
    {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
    };

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = m_width;
    sd.BufferDesc.Height = m_height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = m_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    uint32 tempDriverTSize = driverTypes.size();
    for (uint32 driverTypeIndex = 0; driverTypeIndex < tempDriverTSize; ++driverTypeIndex) {
      hr = D3D11CreateDeviceAndSwapChain(nullptr,
                                         driverTypes[driverTypeIndex],
                                         nullptr, 
                                         createDeviceFlags,
                                         featureLevels.data(), 
                                         (uint32)featureLevels.size(),
                                         D3D11_SDK_VERSION,
                                         &sd,
                                         &m_pSwapChain, 
                                         &m_pd3dDevice,
                                         &featureLevel,
                                         &m_pDeviceContext);
      if (SUCCEEDED(hr)) {
        break;
      }
    }

    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      return false;
    }

    auto* backBuffer = new TexturesDX();

    hr = m_pSwapChain->GetBuffer(0, 
                                 __uuidof(ID3D11Texture2D), 
                                 (LPVOID*)&backBuffer->m_pTexture);

    //We check that everything goes well, if we do not send an error.
    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      delete backBuffer;
      return false;
    }

    backBuffer->m_vRenderTargetView.resize(1);
    hr = m_pd3dDevice->CreateRenderTargetView(backBuffer->m_pTexture,
                                              nullptr,
                                              &backBuffer->m_vRenderTargetView[0]);

    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      delete backBuffer;
      return false;
    }

    m_pBackBuffer = backBuffer;

    auto* depthStencil = new TexturesDX();

    //Texture of the depth and I do the depth.
    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = m_width;
    textureDesc.Height = m_height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    //We create the texture.
    hr = m_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &depthStencil->m_pTexture);

    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      delete backBuffer;
      delete depthStencil;
      return false;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
    depthStencilDesc.Format = textureDesc.Format;
    depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilDesc.Texture2D.MipSlice = 0;

    hr = m_pd3dDevice->CreateDepthStencilView(depthStencil->m_pTexture, 
                                              &depthStencilDesc,
                                              &depthStencil->m_pDepthStencilView);

    //We check that everything goes well, if we do not send an error.
    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      delete backBuffer;
      delete depthStencil;
      return false;
    }

    m_pDeviceContext->OMSetRenderTargets(backBuffer->m_vRenderTargetView.size(),
                                         &backBuffer->m_vRenderTargetView[0],
                                         depthStencil->m_pDepthStencilView);
    m_pDepthStencil = depthStencil;

    return true;
  }

  void
  GraphicsApiDX::drawIndex(uint32 indexCount, uint32 startIndexLocation,
                           uint32 baseVertexLocation) {
    m_pDeviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
  }

  void
  GraphicsApiDX::swapChainPresent(uint32 syncInterval, uint32 flags) {
    m_pSwapChain->Present(syncInterval, flags);
  }

  Textures*
  GraphicsApiDX::loadTextureFromFile(String srcFile) {
    int32 width;
    int32 height;
    int32 components;

    unsigned char* data = stbi_load(srcFile.c_str(), &width, &height, &components, 4);

    if (!data) {
      return nullptr;
    }

    auto* texture = new TexturesDX();

    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.MiscFlags = 0;
    desc.CPUAccessFlags = 0;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    //Texture data
    D3D11_SUBRESOURCE_DATA initData;
    ZeroMemory(&initData, sizeof(initData));
    initData.pSysMem = data;
    initData.SysMemPitch = width * 4;
    initData.SysMemSlicePitch = width * height * 4;

    if (FAILED(m_pd3dDevice->CreateTexture2D(&desc, &initData, &texture->m_pTexture))) {
      delete texture;
      stbi_image_free(data);
      return nullptr;
    }

    //Shader resource data
    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
    ZeroMemory(&viewDesc, sizeof(viewDesc));
    viewDesc.Format = desc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MostDetailedMip = 0;
    viewDesc.Texture2D.MipLevels = 1;

    texture->m_vShaderResourceView.resize(1);
    if (FAILED(m_pd3dDevice->CreateShaderResourceView(texture->m_pTexture, 
                                                      &viewDesc,
                                                      &texture->m_vShaderResourceView[0]))) {
      delete texture;
      stbi_image_free(data);
      return nullptr;
    }

    return texture;
  }

  void
  GraphicsApiDX::unbindOGL() {}

  Matrix4x4
  GraphicsApiDX::matrixPolicy(const Matrix4x4& mat4x4) {
    Matrix4x4 temp;
    temp = mat4x4;
    temp.transpose();
    return temp;
  }

  void 
  GraphicsApiDX::dispatch(uint32 threadGroupCountX, 
                          uint32 threadGroupCountY,
                          uint32 threadGroupCountZ) {
    m_pDeviceContext->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
  }

  void 
  GraphicsApiDX::desbindUAV(uint32 startSlot, uint32 numUAV) {
    ID3D11UnorderedAccessView* UAV_NULL = nullptr;
    m_pDeviceContext->CSSetUnorderedAccessViews(startSlot, numUAV, &UAV_NULL, 0);
  }

  void 
  GraphicsApiDX::desbindRT() {
    static ID3D11RenderTargetView* newRT[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];

    for (uint32 i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
      newRT[i] = nullptr;
    }

    m_pDeviceContext->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, 
                                         newRT, 
                                         nullptr);
  }

  void 
  GraphicsApiDX::desbindSRV(uint32 startSlot) {
    ID3D11ShaderResourceView* pSRV = nullptr;

    m_pDeviceContext->VSSetShaderResources(startSlot, 1, &pSRV);

    m_pDeviceContext->PSSetShaderResources(startSlot, 1, &pSRV);

    m_pDeviceContext->CSSetShaderResources(startSlot, 1, &pSRV);
  }

  /***************************************************************************/
  /**
  * Updates.
  */
  /***************************************************************************/

  void
  GraphicsApiDX::updateConstantBuffer(const void* srcData,
                                      WeakSPtr<ConstantBuffer> updateDataCB) {
    auto cb = updateDataCB.lock().get();
    if (nullptr != cb) {
      ConstantBufferDX* constBuff = reinterpret_cast<ConstantBufferDX*>(cb);

      m_pDeviceContext->UpdateSubresource(constBuff->m_pConstantBuffer, 
                                          0, 
                                          nullptr, 
                                          srcData,
                                          0,
                                          0);
    }
  }

  void 
  GraphicsApiDX::updateComputeBuffer(const void* srcData,
                                     WeakSPtr<ComputeBuffer> computeBuffer) {
    auto cb = computeBuffer.lock().get();
    if (nullptr != cb) {
      ComputeBufferDX* constBuff = reinterpret_cast<ComputeBufferDX*>(cb);

      m_pDeviceContext->UpdateSubresource(constBuff->m_pComputeBuffer,
                                          0,
                                          nullptr,
                                          srcData,
                                          0,
                                          0);
    }
  }

  /***************************************************************************/
  /**
  * Clears.
  */
  /***************************************************************************/

  void
  GraphicsApiDX::clearYourRenderTargetView(WeakSPtr<Textures> renderTarget, Vector4 rgba) {
    auto tempRT = renderTarget.lock().get();
    if (nullptr != tempRT) {
      float clearColor[4] = { rgba.x, rgba.y, rgba.z, rgba.w };

      TexturesDX* renderTGT = reinterpret_cast<TexturesDX*>(tempRT);

      m_pDeviceContext->ClearRenderTargetView(renderTGT->m_vRenderTargetView[0], clearColor);
    }
  }

  void
  GraphicsApiDX::clearYourRenderTarget(WeakSPtr<RenderTarget> renderTarget, Vector4 rgba) {
    auto rt = renderTarget.lock().get();
    if (nullptr != rt) {
      float clearColor[4] = { rgba.x, rgba.y, rgba.z, rgba.w };

      RenderTargetDX* renderTGT = reinterpret_cast<RenderTargetDX*>(rt);

      uint32 numOfTargets = renderTGT->m_renderTarget.m_vRenderTargetView.size();
      for (uint32 i = 0; i < numOfTargets; ++i) {
        m_pDeviceContext->ClearRenderTargetView(renderTGT->m_renderTarget.m_vRenderTargetView[i],
                                                clearColor);
      }
    }
  }

  void
  GraphicsApiDX::clearYourDepthStencilView(WeakSPtr<Textures> depthStencil) {
    auto tempDepth = depthStencil.lock().get();
    if (nullptr != tempDepth) {
      TexturesDX* depthSten = reinterpret_cast<TexturesDX*>(tempDepth);

      m_pDeviceContext->ClearDepthStencilView(depthSten->m_pDepthStencilView,
                                              D3D11_CLEAR_DEPTH, 
                                              1.0f,
                                              0);
    }
  }

  void
  GraphicsApiDX::clearYourDepthStencilView(WeakSPtr<RenderTarget> renderTarg) {
    auto tempDepth = renderTarg.lock().get();
    if (nullptr != tempDepth) {
      RenderTargetDX* depthSten = reinterpret_cast<RenderTargetDX*>(tempDepth);

      if (nullptr != depthSten->m_pDepthStencilV) {
        m_pDeviceContext->ClearDepthStencilView(depthSten->m_pDepthStencilV,
                                                D3D11_CLEAR_DEPTH,
                                                1.0f,
                                                0);
      }
    }
  }

  /***************************************************************************/
  /**
  * Creates.
  */
  /***************************************************************************/

  Shaders*
  GraphicsApiDX::createShadersProgram(const WString& nameVS,
                                      const String& entryPointVS,
                                      const WString& namePS,
                                      const String& entryPointPS) {
    //We generate an auto variable to adapt the type of data we occupy.
    ShadersDX* shaders = new ShadersDX();

    if (!(AnalyzeVertexShaderDX(nameVS))) {
      delete shaders;
      return nullptr;
    }
    if (!(AnalyzePixelShaderDX(namePS))) {
      delete shaders;
      return nullptr;
    }

    //Assign data to variables.
    shaders->m_pPSBlob = nullptr;
    HRESULT hr = S_OK;

    //We compile the received shader.
    hr = CompileShaderFromFile(namePS, entryPointPS.c_str(), "ps_5_0", &shaders->m_pPSBlob);

    //We check that everything goes well, if we do not send an error.
    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      delete shaders;
      return nullptr;
    }

    //We create the pixel shader with the DX function.
    hr = m_pd3dDevice->CreatePixelShader(shaders->m_pPSBlob->GetBufferPointer(),
                                         shaders->m_pPSBlob->GetBufferSize(),
                                         nullptr, &shaders->m_pPixelShader);

    shaders->m_pPSBlob->Release();

    //Finally we return the data in case of not getting an error.
    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      delete shaders;
      return nullptr;
    }

    //Assign data to variables.
    shaders->m_pVSBlob = nullptr;

    //We compile the received shader.
    hr = CompileShaderFromFile(nameVS, entryPointVS.c_str(), "vs_5_0", &shaders->m_pVSBlob);

    //We check that everything goes well, if we do not send an error.
    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      delete shaders;
      return nullptr;
    }

    //We create the vertex shader with the DX function.
    hr = m_pd3dDevice->CreateVertexShader(shaders->m_pVSBlob->GetBufferPointer(),
                                          shaders->m_pVSBlob->GetBufferSize(),
                                          nullptr, &shaders->m_pVertexShader);

    //Finally we return the data in case of not getting an error.
    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      shaders->m_pVSBlob->Release();
      delete shaders;
      return nullptr;
    }

    return shaders;
  }

  Shaders* 
  GraphicsApiDX::createComputeShaderProgram(const WString& fileName,
                                            const String& entryPoint,
                                            const String& versionCS) {
    ShadersDX* shaders = new ShadersDX();

    shaders->m_pComputeShader.reset(new ComputeShaderDX());

    //Assign data to variables.
    shaders->m_pComputeShader->m_pBlob = nullptr;

    if (!(shaders->m_pComputeShader->compileComputeShaderFromFile(fileName,
                                                                  entryPoint,
                                                                  versionCS))) {
      delete shaders;
      return nullptr;
    }

    HRESULT hr = m_pd3dDevice->CreateComputeShader(shaders->m_pComputeShader->
                                                   m_pBlob->GetBufferPointer(),
                                                   shaders->m_pComputeShader->
                                                   m_pBlob->GetBufferSize(),
                                                   nullptr,
                                                   &shaders->m_pComputeShader->
                                                   m_pComputeShaderDX);

    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      delete shaders;
      return nullptr;
    }

    return shaders;
  }

  SPtr<VertexShader>
  GraphicsApiDX::loadVertexShaderFromFile(const char* vertexFilePath,
                                          const char* vertexMainFuntion,
                                          const char* shaderVersion) {
    return SPtr<VertexShader>();
  }

  SPtr<PixelShader>
  GraphicsApiDX::loadPixelShaderFromFile(const char* pixelFilePath,
                                         const char* pixelMainFuntion,
                                         const char* shaderVersion) {
    return SPtr<PixelShader>();
  }

  VertexBuffer*
  GraphicsApiDX::createVertexBuffer(const void* data, const uint32 size) {
    //Generamos una variable auto
    //para adaptar el tipo de dato que ocupamos
    VertexBufferDX* VB = new VertexBufferDX();

    //Asignamos datos a la variable
    HRESULT hr = S_OK;

    if (0 != size) {
      //Rellenamos el descriptor de buffer
      CD3D11_BUFFER_DESC bd(size, D3D11_BIND_VERTEX_BUFFER);

      if (nullptr != data) {
        //Generamos una variable descriptor
        D3D11_SUBRESOURCE_DATA InitData;

        //Limpiamos la memoria y dejamos
        //definido todo en 0
        ZeroMemory(&InitData, sizeof(InitData));

        //Asignamos datos a las variables
        InitData.pSysMem = data;

        //Creamos el buffer
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &VB->m_pVertexBuffer);

        if (FAILED(hr)) {
          _com_error err(hr);
          LPCTSTR errMsg = err.ErrorMessage();
          delete VB;
          return nullptr;
        }

        return VB;
      }
      else {
        hr = m_pd3dDevice->CreateBuffer(&bd, nullptr, &VB->m_pVertexBuffer);

        if (FAILED(hr)) {
          _com_error err(hr);
          LPCTSTR errMsg = err.ErrorMessage();
          delete VB;
          return nullptr;
        }

        return VB;
      }
    }

    delete VB;
    return nullptr;
  }

  IndexBuffer*
  GraphicsApiDX::createIndexBuffer(const void* data, const uint32 size) {
    //Generamos una variable auto
    //para adaptar el tipo de dato que ocupamos
    IndexBufferDX* IB = new IndexBufferDX();

    //Asignamos datos a la variable
    HRESULT hr = S_OK;

    if (0 != size) {
      //Rellenamos el descriptor de buffer
      D3D11_BUFFER_DESC bd;
      ZeroMemory(&bd, sizeof(bd));
      bd.Usage = D3D11_USAGE_DEFAULT;
      bd.ByteWidth = size;
      bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
      bd.CPUAccessFlags = 0;
      bd.MiscFlags = 0;

      if (nullptr != data) {
        //Generamos una variable descriptor
        D3D11_SUBRESOURCE_DATA InitData;

        //Limpiamos la memoria y dejamos
        //definido todo en 0
        ZeroMemory(&InitData, sizeof(InitData));

        //Asignamos datos a las variables
        InitData.pSysMem = data;
        InitData.SysMemPitch = 0;
        InitData.SysMemSlicePitch = 0;

        //Creamos el buffer
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &IB->m_pIndexBuffer);

        if (FAILED(hr)) {
          _com_error err(hr);
          LPCTSTR errMsg = err.ErrorMessage();
          delete IB;
          return nullptr;
        }
        return IB;
      }
      else {
        hr = m_pd3dDevice->CreateBuffer(&bd, nullptr, &IB->m_pIndexBuffer);

        if (FAILED(hr)) {
          _com_error err(hr);
          LPCTSTR errMsg = err.ErrorMessage();
          delete IB;
          return nullptr;
        }
        return IB;
      }
    }

    delete IB;
    return nullptr;
  }

  ConstantBuffer*
  GraphicsApiDX::createConstantBuffer(const uint32 bufferSize) {
    //We generate an auto variable to adapt the type of data that we occupy.
    ConstantBufferDX* constantBuffer = new ConstantBufferDX();

    //Assign data to the variable.
    HRESULT hr = S_OK;

    //We fill the buffer descriptor.
    CD3D11_BUFFER_DESC bd(bufferSize, D3D11_BIND_CONSTANT_BUFFER);

    //We create the buffer.
    hr = m_pd3dDevice->CreateBuffer(&bd, nullptr, &constantBuffer->m_pConstantBuffer);

    //Finally we return the data in case of not getting an error.
    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      delete constantBuffer;
      return nullptr;
    }

    return constantBuffer;
  }

  ConstantBuffer* 
  GraphicsApiDX::createConstantBuffer(const uint32 bufferSize,
                                      CPU_ACCESS::E typeCpu,
                                      USAGE::E typeUsage) {
    //We generate an auto variable to adapt the type of data that we occupy.
    ConstantBufferDX* constantBuffer = new ConstantBufferDX();

    //Assign data to the variable.
    HRESULT hr = S_OK;

    //We fill the buffer descriptor.
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.ByteWidth = bufferSize;
    switch (typeCpu) {
      case CPU_ACCESS::kCpuAccessRead:
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        break;

      case CPU_ACCESS::kCpuAccessWrite:
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        break;
    }
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;
    switch (typeUsage) {
      case USAGE::kUsageDefault:
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        break;

      case USAGE::kUsageDynamic:
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        break;

      case USAGE::kUsageImmutable:
        bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        break;

      case USAGE::kUsageStaging:
        bufferDesc.Usage = D3D11_USAGE_STAGING;
        break;
    }

    //We create the buffer.
    hr = m_pd3dDevice->CreateBuffer(&bufferDesc, nullptr, &constantBuffer->m_pConstantBuffer);

    //Finally we return the data in case of not getting an error.
    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      delete constantBuffer;
      return nullptr;
    }

    return constantBuffer;
  }

  SPtr<ComputeBuffer>
  GraphicsApiDX::createComputeBuffer(const uint32 bufferSize,
                                     const uint32 numElements,
                                     TEXTURE_BIND_FLAGS::E typeBindFlag,
                                     USAGE::E typeUsage) {
    ComputeBufferDX* constantBuffer = new ComputeBufferDX();
    
    D3D11_BUFFER_DESC bufferInfo;
    ZeroMemory(&bufferInfo, sizeof(bufferInfo));
    //Bind flag
    /*
    switch (typeBindFlag) {
      case TEXTURE_BIND_FLAGS::kBindShaderResource:
        bufferInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        break;

      case TEXTURE_BIND_FLAGS::kBindRenderTarget:
        bufferInfo.BindFlags = D3D11_BIND_RENDER_TARGET;
        break;

      case TEXTURE_BIND_FLAGS::kBindDepthStencil:
        bufferInfo.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        break;

      case TEXTURE_BIND_FLAGS::kBindUnorderedAccess:
        bufferInfo.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
        break;
    }
    */
    bufferInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    bufferInfo.ByteWidth = bufferSize;
    bufferInfo.CPUAccessFlags = 0;
    bufferInfo.MiscFlags = 0;
    bufferInfo.StructureByteStride = 0;
    switch (typeUsage) {
      case USAGE::kUsageDefault:
        bufferInfo.Usage = D3D11_USAGE_DEFAULT;
        break;
      
      case USAGE::kUsageDynamic:
        bufferInfo.Usage = D3D11_USAGE_DYNAMIC;
        break;
      
      case USAGE::kUsageImmutable:
        bufferInfo.Usage = D3D11_USAGE_IMMUTABLE;
        break;
      
      case USAGE::kUsageStaging:
        bufferInfo.Usage = D3D11_USAGE_STAGING;
        break;
    }

    HRESULT hr = m_pd3dDevice->CreateBuffer(&bufferInfo,
                                            nullptr,
                                            &constantBuffer->m_pComputeBuffer);
    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      delete constantBuffer;
      return nullptr;
    }

    D3D11_BUFFER_UAV uavInfo;
    uavInfo.FirstElement = 0;
    uavInfo.Flags = 0;
    uavInfo.NumElements = numElements;

    D3D11_UNORDERED_ACCESS_VIEW_DESC uavInfo2;
    uavInfo2.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    uavInfo2.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    uavInfo2.Buffer = uavInfo;

    hr = m_pd3dDevice->CreateUnorderedAccessView(constantBuffer->m_pComputeBuffer,
                                                 &uavInfo2,
                                                 &constantBuffer->m_pComputeUAV);
    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      delete constantBuffer;
      return nullptr;
    }

    D3D11_BUFFER_SRV srvInfo;
    srvInfo.ElementOffset = 0;
    srvInfo.ElementWidth = bufferSize;
    srvInfo.FirstElement = 0;
    srvInfo.NumElements = 1;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvInfo2;
    srvInfo2.Format = DXGI_FORMAT_R32_FLOAT;
    srvInfo2.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
    srvInfo2.Buffer = srvInfo;

    constantBuffer->m_vpComputeSRV.resize(1);
    hr = m_pd3dDevice->CreateShaderResourceView(constantBuffer->m_pComputeBuffer,
                                                &srvInfo2,
                                                &constantBuffer->m_vpComputeSRV[0]);

    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      delete constantBuffer;
      return nullptr;
    }
    
    return SPtr<ComputeBuffer>(constantBuffer);
  }

  Textures*
  GraphicsApiDX::createTexture(const uint32 width, 
                               const uint32 height, 
                               const uint32 bindFlags,
                               TEXTURE_FORMAT::E textureFormat) {
    //Assign data to variable
    HRESULT hr = S_OK;

    //We generate an auto variable to adapt the type of data we occupy.
    TexturesDX* texture = new TexturesDX();

    //We fill in the descriptor.
    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = (DXGI_FORMAT)textureFormat;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = bindFlags;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    //We create the texture.
    hr = m_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &texture->m_pTexture);

    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      delete texture;
      return nullptr;
    }

    //RenderTargetView
    if (bindFlags & D3D11_BIND_RENDER_TARGET) {
      texture->m_vRenderTargetView.resize(1);

      hr = m_pd3dDevice->CreateRenderTargetView(texture->m_pTexture, 
                                                nullptr,
                                                &texture->m_vRenderTargetView[0]);
      if (FAILED(hr)) {
        _com_error err(hr);
        LPCTSTR errMsg = err.ErrorMessage();
        delete texture;
        return nullptr;
      }
    }

    //DepthStencilView
    if (bindFlags & D3D11_BIND_DEPTH_STENCIL) {
      D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc;
      ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
      depthStencilDesc.Format = textureDesc.Format;
      depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
      depthStencilDesc.Texture2D.MipSlice = 0;

      hr = m_pd3dDevice->CreateDepthStencilView(texture->m_pTexture,
                                                &depthStencilDesc,
                                                &texture->m_pDepthStencilView);

      //We check that everything goes well, if we do not send an error.
      if (FAILED(hr)) {
        _com_error err(hr);
        LPCTSTR errMsg = err.ErrorMessage();
        delete texture;
        return nullptr;
      }
    }

    //ShaderResourceView
    if (bindFlags & D3D11_BIND_SHADER_RESOURCE) {
      CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc(D3D11_SRV_DIMENSION_TEXTURE2D);

      texture->m_vShaderResourceView.resize(1);
      hr = m_pd3dDevice->CreateShaderResourceView(texture->m_pTexture,
                                                  &shaderResourceViewDesc,
                                                  &texture->m_vShaderResourceView[0]);
      
      //We check that everything goes well, if we do not send an error.
      if (FAILED(hr)) {
        _com_error err(hr);
        LPCTSTR errMsg = err.ErrorMessage();
        delete texture;
        return nullptr;
      }
    }
    
    //UAV (Unordered Access View)
    if (bindFlags & D3D11_BIND_UNORDERED_ACCESS) {
      CD3D11_UNORDERED_ACCESS_VIEW_DESC uavInfo(D3D11_UAV_DIMENSION_TEXTURE2D);

      m_pd3dDevice->CreateUnorderedAccessView(texture->m_pTexture, &uavInfo, &texture->m_pUAV);
    }

    return texture;
  }

  SPtr<SamplerState>
  GraphicsApiDX::createSamplerState(FILTER::E typeFilter,
                                    TEXTURE_ADDRESS::E textureAddress,
                                    COMPARISON::E typeComparison) {
    //We generate an auto variable to adapt the type of data we occupy.
    SamplerStateDX* samplerState = new SamplerStateDX();

    //Assign data to variable.
    HRESULT hr = S_OK;

    //We define the sampler state.
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    switch (typeFilter) {
      case FILTER::kFilterMinMagMipPoint:
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        break;

      case FILTER::kFilterMinMagPointMipLinear:
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
        break;

      case FILTER::kFilterMinPointMagLinearMipPoint:
        sampDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
        break;

      case FILTER::kFilterMinPointMagMipLinear:
        sampDesc.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
        break;

      case FILTER::kFilterMinLinearMagMipPoint:
        sampDesc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
        break;

      case FILTER::kFilterMinLinearMagPointMipLinear:
        sampDesc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
        break;

      case FILTER::kFilterMinMagLinearMipPoint:
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
        break;

      case FILTER::kFilterMinMagMipLinear:
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        break;

      case FILTER::kFilterAnisotropic:
        sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
        break;

      case FILTER::kFilterComparisionMinPointMagLinearMipPoint:
        sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
        break;

      case FILTER::kFilterComparisionMinPointMagMipLinear:
        sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
        break;

      case FILTER::kFilterComparisionMinLinearMagMipPoint:
        sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
        break;

      case FILTER::kFilterComparisionMinLinearMagPointMipLinear:
        sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
        break;

      case FILTER::kFilterComparisionMinMagLinearMipPoint:
        sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
        break;

      case FILTER::kFilterComparisionMinMagMipLinear:
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        break;

      case FILTER::kFilterComparisionAnisotropic:
        sampDesc.Filter = D3D11_FILTER_COMPARISON_ANISOTROPIC;
        break;
    }
    switch (textureAddress) {
      case TEXTURE_ADDRESS::kTextureAddressWrap:
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        break;

      case TEXTURE_ADDRESS::kTextureAddressMirror:
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
        break;

      case TEXTURE_ADDRESS::kTextureAddressClamp:
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        break;

      case TEXTURE_ADDRESS::kTextureAddressBorder:
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        break;

      case TEXTURE_ADDRESS::kTextureAddressMirrorOnce:
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
        break;
    }
    switch (typeComparison) {
      case COMPARISON::kComparisonNever:
        sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        break;
      
      case COMPARISON::kComparisonLess:
        sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
        break;
      
      case COMPARISON::kComparisonEqual:
        sampDesc.ComparisonFunc = D3D11_COMPARISON_EQUAL;
        break;
      
      case COMPARISON::kComparisonLessEqual:
        sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
        break;
      
      case COMPARISON::kComparisonGreater:
        sampDesc.ComparisonFunc = D3D11_COMPARISON_GREATER;
        break;
      
      case COMPARISON::kComparisonNotEqual:
        sampDesc.ComparisonFunc = D3D11_COMPARISON_NOT_EQUAL;
        break;
      
      case COMPARISON::kComparisonGreaterEqual:
        sampDesc.ComparisonFunc = D3D11_COMPARISON_GREATER_EQUAL;
        break;
      
      case COMPARISON::kComparisonAlways:
        sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        break;
    }
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    sampDesc.MipLODBias = 0;

    hr = m_pd3dDevice->CreateSamplerState(&sampDesc, &samplerState->m_pSamplerState);

    //Finally we return the data in case of not getting an error.
    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      delete samplerState;
      return nullptr;
    }

    return SPtr<SamplerState>(samplerState);
  }

  InputLayout*
  GraphicsApiDX::createInputLayout(WeakSPtr<Shaders> vertexShader) {
    //Asignamos datos a la variable
    HRESULT hr = S_OK;

    InputLayoutDX* inputLayout = new InputLayoutDX();
    ShadersDX* vShader = reinterpret_cast<ShadersDX*>(vertexShader.lock().get());

    //Generamos la información del shader
    ID3D11ShaderReflection* pVertexShaderReflection = nullptr;

    if (nullptr == &vShader) {
      return nullptr;
    }
    if (FAILED(D3DReflect(vShader->m_pVSBlob->GetBufferPointer(),
                          vShader->m_pVSBlob->GetBufferSize(),
                          IID_ID3D11ShaderReflection,
                          (void**)&pVertexShaderReflection))) {
                          delete inputLayout;
                          pVertexShaderReflection->Release();
      return nullptr;
    }

    //Generamos una descriptor del shader
    D3D11_SHADER_DESC shaderDesc;

    //Obtenemos la información de nuestro
    //descriptor del shader
    pVertexShaderReflection->GetDesc(&shaderDesc);

    //Leer la descripción del input layout de 
    // la información del sombreador
    uint32 byteOffset = 0;
    Vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;

    uint32 tempInputParams = shaderDesc.InputParameters;
    for (uint32 i = 0; i < tempInputParams; ++i) {
      D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
      pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

      //Creamos y guardamos la información
      //para input element
      D3D11_INPUT_ELEMENT_DESC elementDesc;

      //Rellenamos 6 datos de 7 el formato es
      //lo que sigue
      elementDesc.SemanticName = paramDesc.SemanticName;
      elementDesc.SemanticIndex = paramDesc.SemanticIndex;
      elementDesc.InputSlot = 0;
      elementDesc.AlignedByteOffset = byteOffset;
      elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
      elementDesc.InstanceDataStepRate = 0;

      //Determinamos el formato DXGI
      // DXGI_FORMAT_R32_UINT
      // DXGI_FORMAT_R32_SINT
      // DXGI_FORMAT_R32_FLOAT
      if (paramDesc.Mask == 1) {
        if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
          elementDesc.Format = DXGI_FORMAT_R32_UINT;
        }
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
          elementDesc.Format = DXGI_FORMAT_R32_SINT;
        }
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
          elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
        }
        byteOffset += 4;
      }
      // DXGI_FORMAT_R32G32_UINT
      // DXGI_FORMAT_R32G32_SINT
      // DXGI_FORMAT_R32G32_FLOAT
      else if (paramDesc.Mask <= 3) {
        if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
          elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
        }
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
          elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
        }
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
          elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
        }
        byteOffset += 8;
      }
      // DXGI_FORMAT_R32G32B32_UINT
      // DXGI_FORMAT_R32G32B32_SINT
      // DXGI_FORMAT_R32G32B32_FLOAT
      else if (paramDesc.Mask <= 7) {
        if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
          elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
        }
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
          elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
        }
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
          elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        }
        byteOffset += 12;
      }
      // DXGI_FORMAT_R32G32B32A32_UINT
      // DXGI_FORMAT_R32G32B32A32_SINT
      // DXGI_FORMAT_R32G32B32A32_FLOAT
      else if (paramDesc.Mask <= 15) {
        if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
          elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
        }
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
          elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
        }
        else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
          elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        }
        byteOffset += 16;
      }

      //Guardamos el element desc
      inputLayoutDesc.push_back(elementDesc);
    }

    //Creamos el input layout
    hr = m_pd3dDevice->CreateInputLayout(&inputLayoutDesc[0],
                                         (uint32)inputLayoutDesc.size(),
                                         vShader->m_pVSBlob->GetBufferPointer(),
                                         vShader->m_pVSBlob->GetBufferSize(),
                                         &inputLayout->m_pVertexLayout);
    //Liberamos el puntero blob
    vShader->m_pVSBlob->Release();

    //Liberación de memoria de reflexión de shader de asignación
    pVertexShaderReflection->Release();

    //Checamos que todo salga bien, si no mandamos un error
    if (FAILED(hr)) {
      _com_error err(hr);
      LPCTSTR errMsg = err.ErrorMessage();
      delete inputLayout;
      return nullptr;
    }

    //Registro de longitud de bytes
    inputLayout->m_inputLayoutByteLength = &byteOffset;

    return inputLayout;
  }

  SPtr<RenderTarget>
  GraphicsApiDX::createRenderTarget(uint32 width, 
                                    uint32 height, 
                                    uint32 mipLevels,
                                    uint32 numRenderTargets, 
                                    float scale, 
                                    bool depth,
                                    TEXTURE_FORMAT::E typeTexture, 
                                    TEXTURE_BIND_FLAGS::E typeBindFlag) {
    if (0 >= numRenderTargets) {
      numRenderTargets = 1;
    }

    HRESULT hr;
    RenderTargetDX* newRT = new RenderTargetDX;
    newRT->m_renderTarget.m_vShaderResourceView.resize(numRenderTargets);
    newRT->m_renderTarget.m_vRenderTargetView.resize(numRenderTargets);
    newRT->m_scale = scale;

    for (uint32 i = 0; i < numRenderTargets; ++i) {
      CD3D11_TEXTURE2D_DESC textureDesc;
      CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
      CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

      /*
      * Map's Texture
      */
      // Initialize the  texture description.
      ZeroMemory(&textureDesc, sizeof(textureDesc));

      //Setup the texture description.
      //We will have our map be a square
      //We will need to have this texture bound as a render target AND a shader resource
      textureDesc.Width = (uint32)(width * scale);
      textureDesc.Height = (uint32)(height * scale);
      textureDesc.MipLevels = mipLevels;
      textureDesc.ArraySize = 1;
      switch (typeTexture) {
        case TEXTURE_FORMAT::E::kUnknown:
          textureDesc.Format = DXGI_FORMAT_UNKNOWN;
          break;

        case TEXTURE_FORMAT::E::kR32G32B32A32Typeless:
          textureDesc.Format = DXGI_FORMAT_R32G32B32A32_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kR32G32B32A32Float:
          textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
          break;

        case TEXTURE_FORMAT::E::kR32G32B32A32UInt:
          textureDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
          break;

        case TEXTURE_FORMAT::E::kR32G32B32A32SInt:
          textureDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
          break;

        case TEXTURE_FORMAT::E::kR32G32B32Typeless:
          textureDesc.Format = DXGI_FORMAT_R32G32B32_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kR32G32B32Float:
          textureDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
          break;

        case TEXTURE_FORMAT::E::kR32G32B32UInt:
          textureDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
          break;

        case TEXTURE_FORMAT::E::kR32G32B32SInt:
          textureDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
          break;

        case TEXTURE_FORMAT::E::kR16G16B16A16Typeless:
          textureDesc.Format = DXGI_FORMAT_R16G16B16A16_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kR16G16B16A16Float:
          textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
          break;

        case TEXTURE_FORMAT::E::kR16G16B16A16UNorm:
          textureDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
          break;

        case TEXTURE_FORMAT::E::kR16G16B16A16UInt:
          textureDesc.Format = DXGI_FORMAT_R16G16B16A16_UINT;
          break;

        case TEXTURE_FORMAT::E::kR16G16B16A16SNorm:
          textureDesc.Format = DXGI_FORMAT_R16G16B16A16_SNORM;
          break;

        case TEXTURE_FORMAT::E::kR16G16B16A16SInt:
          textureDesc.Format = DXGI_FORMAT_R16G16B16A16_SINT;
          break;

        case TEXTURE_FORMAT::E::kR32G32Typeless:
          textureDesc.Format = DXGI_FORMAT_R32G32_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kR32G32Float:
          textureDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
          break;

        case TEXTURE_FORMAT::E::kR32G32UInt:
          textureDesc.Format = DXGI_FORMAT_R32G32_UINT;
          break;

        case TEXTURE_FORMAT::E::kR32G32SInt:
          textureDesc.Format = DXGI_FORMAT_R32G32_SINT;
          break;

        case TEXTURE_FORMAT::E::kR32G8X24Typeless:
          textureDesc.Format = DXGI_FORMAT_R32G8X24_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kD32FloatS8X24UInt:
          textureDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
          break;

        case TEXTURE_FORMAT::E::kR32FloatX8X24Typeless:
          textureDesc.Format = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kX32TypelessG8X24UInt:
          textureDesc.Format = DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
          break;

        case TEXTURE_FORMAT::E::kR10G10B10A2Typeless:
          textureDesc.Format = DXGI_FORMAT_R10G10B10A2_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kR10G10B10A2UNorm:
          textureDesc.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
          break;

        case TEXTURE_FORMAT::E::kR10G10B10A2UInt:
          textureDesc.Format = DXGI_FORMAT_R10G10B10A2_UINT;
          break;

        case TEXTURE_FORMAT::E::kR11G11B10Float:
          textureDesc.Format = DXGI_FORMAT_R11G11B10_FLOAT;
          break;

        case TEXTURE_FORMAT::E::kR8G8B8A8Typeless:
          textureDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kR8G8B8A8UNorm:
          textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
          break;

        case TEXTURE_FORMAT::E::kR8G8B8A8UNorm_SRGB:
          textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
          break;

        case TEXTURE_FORMAT::E::kR8G8B8A8UInt:
          textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UINT;
          break;

        case TEXTURE_FORMAT::E::kR8G8B8A8SNorm:
          textureDesc.Format = DXGI_FORMAT_R8G8B8A8_SNORM;
          break;

        case TEXTURE_FORMAT::E::kR8G8B8A8SInt:
          textureDesc.Format = DXGI_FORMAT_R8G8B8A8_SINT;
          break;

        case TEXTURE_FORMAT::E::kR16G16Typeless:
          textureDesc.Format = DXGI_FORMAT_R16G16_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kR16G16Float:
          textureDesc.Format = DXGI_FORMAT_R16G16_FLOAT;
          break;

        case TEXTURE_FORMAT::E::kR16G16UNorm:
          textureDesc.Format = DXGI_FORMAT_R16G16_UNORM;
          break;

        case TEXTURE_FORMAT::E::kR16G16UInt:
          textureDesc.Format = DXGI_FORMAT_R16G16_UINT;
          break;

        case TEXTURE_FORMAT::E::kR16G16SNorm:
          textureDesc.Format = DXGI_FORMAT_R16G16_SNORM;
          break;

        case TEXTURE_FORMAT::E::kR16G16SInt:
          textureDesc.Format = DXGI_FORMAT_R16G16_SINT;
          break;

        case TEXTURE_FORMAT::E::kR32Typeless:
          textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kD32Float:
          textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
          break;

        case TEXTURE_FORMAT::E::kR32Float:
          textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
          break;

        case TEXTURE_FORMAT::E::kR32UInt:
          textureDesc.Format = DXGI_FORMAT_R32_UINT;
          break;

        case TEXTURE_FORMAT::E::kR32SInt:
          textureDesc.Format = DXGI_FORMAT_R32_SINT;
          break;

        case TEXTURE_FORMAT::E::kR24G8Typeless:
          textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kD24UNormS8UInt:
          textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
          break;

        case TEXTURE_FORMAT::E::kR24UNormX8Typeless:
          textureDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kX24TypelessG8UInt:
          textureDesc.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
          break;

        case TEXTURE_FORMAT::E::kR8G8Typeless:
          textureDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kR8G8UNorm:
          textureDesc.Format = DXGI_FORMAT_R8G8_UNORM;
          break;

        case TEXTURE_FORMAT::E::kR8G8UInt:
          textureDesc.Format = DXGI_FORMAT_R8G8_UINT;
          break;

        case TEXTURE_FORMAT::E::kR8G8SNorm:
          textureDesc.Format = DXGI_FORMAT_R8G8_SNORM;
          break;

        case TEXTURE_FORMAT::E::kR8G8SInt:
          textureDesc.Format = DXGI_FORMAT_R8G8_SINT;
          break;

        case TEXTURE_FORMAT::E::kR16Typeless:
          textureDesc.Format = DXGI_FORMAT_R16_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kR16Float:
          textureDesc.Format = DXGI_FORMAT_R16_FLOAT;
          break;

        case TEXTURE_FORMAT::E::kD16UNorm:
          textureDesc.Format = DXGI_FORMAT_D16_UNORM;
          break;

        case TEXTURE_FORMAT::E::kR16UNorm:
          textureDesc.Format = DXGI_FORMAT_R16G16_UNORM;
          break;

        case TEXTURE_FORMAT::E::kR16UInt:
          textureDesc.Format = DXGI_FORMAT_R16_UINT;
          break;

        case TEXTURE_FORMAT::E::kR16SNorm:
          textureDesc.Format = DXGI_FORMAT_R16G16_SNORM;
          break;

        case TEXTURE_FORMAT::E::kR16SInt:
          textureDesc.Format = DXGI_FORMAT_R16_SINT;
          break;

        case TEXTURE_FORMAT::E::kR8Typeless:
          textureDesc.Format = DXGI_FORMAT_R8_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kR8UNorm:
          textureDesc.Format = DXGI_FORMAT_R8_UNORM;
          break;

        case TEXTURE_FORMAT::E::kR8UInt:
          textureDesc.Format = DXGI_FORMAT_R8_UINT;
          break;

        case TEXTURE_FORMAT::E::kR8SNorm:
          textureDesc.Format = DXGI_FORMAT_R8_SNORM;
          break;

        case TEXTURE_FORMAT::E::kR8SInt:
          textureDesc.Format = DXGI_FORMAT_R8_SINT;
          break;

        case TEXTURE_FORMAT::E::kA8UNorm:
          textureDesc.Format = DXGI_FORMAT_R8_UNORM;
          break;

        case TEXTURE_FORMAT::E::kR1UNorm:
          textureDesc.Format = DXGI_FORMAT_R1_UNORM;
          break;

        case TEXTURE_FORMAT::E::kR9G9B9E5SHAREDEXP:
          textureDesc.Format = DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
          break;

        case TEXTURE_FORMAT::E::kR8G8B8G8UNorm:
          textureDesc.Format = DXGI_FORMAT_R8G8_B8G8_UNORM;
          break;

        case TEXTURE_FORMAT::E::kG8R8G8B8UNorm:
          textureDesc.Format = DXGI_FORMAT_G8R8_G8B8_UNORM;
          break;

        case TEXTURE_FORMAT::E::kBC1Typeless:
          textureDesc.Format = DXGI_FORMAT_BC1_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kBC1UNorm:
          textureDesc.Format = DXGI_FORMAT_BC1_UNORM;
          break;

        case TEXTURE_FORMAT::E::kBC1UNorm_SRGB:
          textureDesc.Format = DXGI_FORMAT_BC1_UNORM_SRGB;
          break;

        case TEXTURE_FORMAT::E::kBC2Typeless:
          textureDesc.Format = DXGI_FORMAT_BC2_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kBC2UNorm:
          textureDesc.Format = DXGI_FORMAT_BC2_UNORM;
          break;

        case TEXTURE_FORMAT::E::kBC2UNorm_SRGB:
          textureDesc.Format = DXGI_FORMAT_BC2_UNORM_SRGB;
          break;

        case TEXTURE_FORMAT::E::kBC3Typeless:
          textureDesc.Format = DXGI_FORMAT_BC3_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kBC3UNorm:
          textureDesc.Format = DXGI_FORMAT_BC3_UNORM;
          break;

        case TEXTURE_FORMAT::E::kBC3UNormSRGB:
          textureDesc.Format = DXGI_FORMAT_BC3_UNORM_SRGB;
          break;

        case TEXTURE_FORMAT::E::kBC4Typeless:
          textureDesc.Format = DXGI_FORMAT_BC4_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kBC4UNorm:
          textureDesc.Format = DXGI_FORMAT_BC4_UNORM;
          break;

        case TEXTURE_FORMAT::E::kBC4SNorm:
          textureDesc.Format = DXGI_FORMAT_BC4_SNORM;
          break;

        case TEXTURE_FORMAT::E::kBC5Typeless:
          textureDesc.Format = DXGI_FORMAT_BC5_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kBC5UNorm:
          textureDesc.Format = DXGI_FORMAT_BC5_UNORM;
          break;

        case TEXTURE_FORMAT::E::kBC5SNorm:
          textureDesc.Format = DXGI_FORMAT_BC5_SNORM;
          break;

        case TEXTURE_FORMAT::E::kB5G6R5UNorm:
          textureDesc.Format = DXGI_FORMAT_B5G6R5_UNORM;
          break;

        case TEXTURE_FORMAT::E::kB5G5R5A1UNorm:
          textureDesc.Format = DXGI_FORMAT_B5G5R5A1_UNORM;
          break;

        case TEXTURE_FORMAT::E::kB8G8R8A8UNorm:
          textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
          break;

        case TEXTURE_FORMAT::E::kB8G8R8X8UNorm:
          textureDesc.Format = DXGI_FORMAT_B8G8R8X8_UNORM;
          break;

        case TEXTURE_FORMAT::E::kR10G10B10XRBiasA2UNorm:
          textureDesc.Format = DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
          break;

        case TEXTURE_FORMAT::E::kB8G8R8A8Typeless:
          textureDesc.Format = DXGI_FORMAT_B8G8R8A8_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kB8G8R8A8UNormSRGB:
          textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
          break;

        case TEXTURE_FORMAT::E::kB8G8R8X8Typeless:
          textureDesc.Format = DXGI_FORMAT_B8G8R8X8_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kB8G8R8X8UNormSRGB:
          textureDesc.Format = DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
          break;

        case TEXTURE_FORMAT::E::kBC6HTypeless:
          textureDesc.Format = DXGI_FORMAT_BC6H_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kBC6HUF16:
          textureDesc.Format = DXGI_FORMAT_BC6H_UF16;
          break;

        case TEXTURE_FORMAT::E::kBC6HSF16:
          textureDesc.Format = DXGI_FORMAT_BC6H_SF16;
          break;

        case TEXTURE_FORMAT::E::kBC7Typeless:
          textureDesc.Format = DXGI_FORMAT_BC7_TYPELESS;
          break;

        case TEXTURE_FORMAT::E::kBC7UNorm:
          textureDesc.Format = DXGI_FORMAT_BC7_UNORM;
          break;

        case TEXTURE_FORMAT::E::kBC7UNormSRGB:
          textureDesc.Format = DXGI_FORMAT_BC7_UNORM_SRGB;
          break;
      }
      textureDesc.SampleDesc.Count = 1;
      textureDesc.Usage = D3D11_USAGE_DEFAULT;

      if ((TEXTURE_BIND_FLAGS::kDefault != typeBindFlag) && 
          (TEXTURE_BIND_FLAGS::kBindUnorderedAccess == typeBindFlag)) {

        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET |
                                D3D11_BIND_SHADER_RESOURCE |
                                D3D11_BIND_UNORDERED_ACCESS;
      }
      else {
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
      }

      textureDesc.CPUAccessFlags = 0;
      textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_SHARED;

      newRT->m_mipLevel = mipLevels;

      // Create the texture
      hr = m_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &newRT->m_renderTarget.m_pTexture);

      if (FAILED(hr)) {
        _com_error err(hr);
        LPCTSTR errMsg = err.ErrorMessage();
        delete &newRT->m_renderTarget.m_pTexture;
        return nullptr;
      }

      //
      if ((TEXTURE_BIND_FLAGS::kDefault != typeBindFlag) &&
          (TEXTURE_BIND_FLAGS::kBindUnorderedAccess == typeBindFlag)) {
        newRT->m_renderTarget.m_pCompBuff = make_shared<ComputeBufferDX>();

        D3D11_BUFFER_UAV uavInfo;
        uavInfo.FirstElement = 0;
        uavInfo.Flags = 0;
        uavInfo.NumElements = numRenderTargets;

        D3D11_UNORDERED_ACCESS_VIEW_DESC uavInfo2;
        uavInfo2.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        uavInfo2.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
        uavInfo2.Buffer = uavInfo;

        hr = m_pd3dDevice->CreateUnorderedAccessView(
                                   newRT->m_renderTarget.m_pTexture,
                                   &uavInfo2,
                                   &newRT->m_renderTarget.m_pCompBuff->m_pComputeUAV);
        if (FAILED(hr)) {
          _com_error err(hr);
          LPCTSTR errMsg = err.ErrorMessage();
          delete &newRT->m_renderTarget.m_pCompBuff;
          return nullptr;
        }
      }

      /*
      * Map's Render Target
      */
      // Setup the description of the render target view.
      renderTargetViewDesc.Format = textureDesc.Format;
      renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
      renderTargetViewDesc.Texture2D.MipSlice = 0;

      // Create the render target view.
      hr = m_pd3dDevice->CreateRenderTargetView(newRT->m_renderTarget.m_pTexture,
                                                &renderTargetViewDesc,
                                                &newRT->m_renderTarget.m_vRenderTargetView[i]);
      if (FAILED(hr)) {
        _com_error err(hr);
        LPCTSTR errMsg = err.ErrorMessage();
        return nullptr;
      }

      /*
      *  Map's Shader Resource View
      */

      // Setup the description of the shader resource view.
      shaderResourceViewDesc.Format = textureDesc.Format;
      shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
      shaderResourceViewDesc.Texture2D.MipLevels = mipLevels;

      // Create the shader resource view.
      hr =  m_pd3dDevice->CreateShaderResourceView(
                          newRT->m_renderTarget.m_pTexture,
                          &shaderResourceViewDesc,
                          &newRT->m_renderTarget.m_vShaderResourceView[i]);

      if (FAILED(hr)) {
        _com_error err(hr);
        LPCTSTR errMsg = err.ErrorMessage();
        return nullptr;
      }
    }

    if (depth) {
      CD3D11_TEXTURE2D_DESC descDepth;
      ZeroMemory(&descDepth, sizeof(descDepth));
      descDepth.Width = width * scale;
      descDepth.Height = height * scale;
      descDepth.MipLevels = mipLevels;
      descDepth.ArraySize = 1;
      descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
      descDepth.SampleDesc.Count = 1;
      descDepth.SampleDesc.Quality = 0;
      descDepth.Usage = D3D11_USAGE_DEFAULT;
      descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
      descDepth.CPUAccessFlags = 0;
      descDepth.MiscFlags = 0;

      m_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &newRT->m_pTextureDepthSV);

      D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
      ZeroMemory(&descDSV, sizeof(descDSV));
      descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
      descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
      descDSV.Texture2D.MipSlice = 0;

      m_pd3dDevice->CreateDepthStencilView(newRT->m_pTextureDepthSV,
                                           &descDSV,
                                           &newRT->m_pDepthStencilV);
    }

    SPtr<RenderTarget> newRenderTarg(newRT);
    m_vRenderTargets.push_back(newRenderTarg);
    return newRenderTarg;
  }

  void
  GraphicsApiDX::createMipMaps(WeakSPtr<RenderTarget> renderTarg) {
    auto rt = renderTarg.lock().get();
    if (nullptr != rt) {
      RenderTargetDX* rendTarget = reinterpret_cast<RenderTargetDX*>(rt);

      uint32 numRT = rendTarget->m_renderTarget.m_vShaderResourceView.size();
      for (uint32 i = 0; i < numRT; ++i) {
        m_pDeviceContext->GenerateMips(rendTarget->m_renderTarget.m_vShaderResourceView[i]);
      }
    }
  }

  /***************************************************************************/
  /**
  * Sets.
  */
  /***************************************************************************/

  void
  GraphicsApiDX::setPixelShader(WeakSPtr<Shaders> pixelShader) {
    ShadersDX* pShader = reinterpret_cast<ShadersDX*> (pixelShader.lock().get());

    m_pDeviceContext->PSSetShader(pShader->m_pPixelShader, nullptr, 0);
  }

  void
  GraphicsApiDX::setVertexShader(WeakSPtr<Shaders> vertexShader) {
    ShadersDX* vShader = reinterpret_cast<ShadersDX*>(vertexShader.lock().get());

    m_pDeviceContext->VSSetShader(vShader->m_pVertexShader, nullptr, 0);
  }

  void
  GraphicsApiDX::setVertexBuffer(WeakSPtr<VertexBuffer> vertexBuffer) {
    auto tempVertex = vertexBuffer.lock().get();
    if (nullptr != tempVertex) {
      VertexBufferDX* vBuffer = reinterpret_cast<VertexBufferDX*>(tempVertex);

      //TODO: cambiar a que este dato sea manipulable y no dependa de ese nombre "vertex"
      uint32 stride = sizeof(Vertex);
      uint32 offset = 0;

      m_pDeviceContext->IASetVertexBuffers(0, 
                                           1, 
                                           &vBuffer->m_pVertexBuffer, 
                                           &stride,
                                           &offset);
    }
  }

  void
  GraphicsApiDX::setIndexBuffer(WeakSPtr<IndexBuffer> indexBuffer) {
    auto tempIndex = indexBuffer.lock().get();
    if (nullptr != tempIndex) {
      IndexBufferDX* iBuffer = reinterpret_cast<IndexBufferDX*>(tempIndex);

      m_pDeviceContext->IASetIndexBuffer(iBuffer->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    }
  }

  void
  GraphicsApiDX::setConstantBuffer(bool isVertex, 
                                   WeakSPtr<ConstantBuffer> constantBuffer,
                                   const uint32 startSlot, 
                                   const uint32 numBuffers) {
    ConstantBufferDX* constBuffer =
                      reinterpret_cast<ConstantBufferDX*>(constantBuffer.lock().get());
    if (true == isVertex) {
      m_pDeviceContext->VSSetConstantBuffers(startSlot, 
                                             numBuffers,
                                             &constBuffer->m_pConstantBuffer);
    }
    else {
      m_pDeviceContext->PSSetConstantBuffers(startSlot,
                                             numBuffers, 
                                             &constBuffer->m_pConstantBuffer);
    }
  }

  void
  GraphicsApiDX::setSamplerState(WeakSPtr<SamplerState> sampler, 
                                 uint32 startSlot,
                                 uint32 numSamplers) {
    auto tempSamp = sampler.lock().get();
    if (nullptr != tempSamp) {
      SPtr<SamplerState> tempSampler(tempSamp);

      setSamplerVertexShader(tempSampler, startSlot, numSamplers);
      setSamplerPixelShader(tempSampler, startSlot, numSamplers);
    }
  }

  void
  GraphicsApiDX::setSamplerVertexShader(WeakSPtr<SamplerState> sampler, 
                                        uint32 startSlot,
                                        uint32 numSamplers) {
    auto tempSamp = sampler.lock().get();
    if (nullptr != tempSamp) {
      SamplerStateDX* samplerDX = reinterpret_cast<SamplerStateDX*>(tempSamp);

      m_pDeviceContext->VSSetSamplers(startSlot, numSamplers, &samplerDX->m_pSamplerState);
    }
  }

  void
  GraphicsApiDX::setSamplerPixelShader(WeakSPtr<SamplerState> sampler, 
                                       uint32 startSlot,
                                       uint32 numSamplers) {
    auto tempSamp = sampler.lock().get();
    if (nullptr != tempSamp) {
      SamplerStateDX* samplerDX = reinterpret_cast<SamplerStateDX*>(tempSamp);

      m_pDeviceContext->PSSetSamplers(startSlot, numSamplers, &samplerDX->m_pSamplerState);
    }
  }

  void
  GraphicsApiDX::setShaderResourceView(const Vector<Textures*>& texture,
                                       const uint32 startSlot,
                                       const uint32 numViews) {
    uint32 tempSize = texture.size();
    for (uint32 i = 0; i < tempSize; ++i) {
      TexturesDX* shaderResource = reinterpret_cast<TexturesDX*>(texture[i]);

      if (!shaderResource) {
        ID3D11ShaderResourceView* temp = nullptr;
        m_pDeviceContext->PSSetShaderResources(i, numViews, &temp);
      }
      if (nullptr != shaderResource) {
        m_pDeviceContext->PSSetShaderResources(i, 
                                               numViews, 
                                               shaderResource->m_vShaderResourceView.data());
      }
    }
  }

  void
  GraphicsApiDX::setShaderResourceView(void* renderTexture,
                                       const uint32 startSlot,
                                       const uint32 numViews) {
    ID3D11ShaderResourceView* shaderResource =
                              reinterpret_cast<ID3D11ShaderResourceView*>(renderTexture);

    if (nullptr != shaderResource) {
      m_pDeviceContext->PSSetShaderResources(startSlot, numViews, &shaderResource);
    }
  }

  void 
  GraphicsApiDX::csSetShaderResource(void* renderTexture, 
                                     const uint32 startSlot, 
                                     const uint32 numViews) {
    ID3D11ShaderResourceView* shaderResource =
                              reinterpret_cast<ID3D11ShaderResourceView*>(renderTexture);
    if (nullptr != shaderResource) {
      m_pDeviceContext->CSSetShaderResources(startSlot, numViews, &shaderResource);
    }
  }

  void
  GraphicsApiDX::setRenderTarget(WeakSPtr<Textures> renderTarget, 
                                 WeakSPtr<Textures> depthStencil) {
    auto rt = renderTarget.lock().get();
    auto ds = depthStencil.lock().get();

    if ((nullptr != rt) && (nullptr != ds)) {
      TexturesDX* rTarget = reinterpret_cast<TexturesDX*>(rt);
      TexturesDX* dStencil = reinterpret_cast<TexturesDX*>(ds);

      m_pDeviceContext->OMSetRenderTargets(rTarget->m_vRenderTargetView.size(),
                                           &rTarget->m_vRenderTargetView[0],
                                           dStencil->m_pDepthStencilView);
    }
    else if ((nullptr != rt) && (nullptr == ds)) {
      SPtr<Textures> temp(rt);
      setRenderTarget(temp);
    }
  }

  void
  GraphicsApiDX::setRenderTarget(WeakSPtr<Textures> renderTarget) {
    auto rt = renderTarget.lock().get();

    if (nullptr != rt) {
      TexturesDX* rTarget = reinterpret_cast<TexturesDX*>(rt);

      m_pDeviceContext->OMSetRenderTargets(rTarget->m_vRenderTargetView.size(),
                                           &rTarget->m_vRenderTargetView[0], 
                                           nullptr);
    }
  }

  void
  GraphicsApiDX::setRenderTarget(WeakSPtr<RenderTarget> renderTarget,
                                 WeakSPtr<Textures> depthStencil) {
    auto rt = renderTarget.lock().get();
    auto ds = depthStencil.lock().get();

    if ((nullptr != rt) && (nullptr != ds)) {
      RenderTargetDX* rTarget = reinterpret_cast<RenderTargetDX*>(rt);
      TexturesDX* dStencil = reinterpret_cast<TexturesDX*>(ds);

      m_pDeviceContext->OMSetRenderTargets(rTarget->m_renderTarget.m_vRenderTargetView.size(),
                                           &rTarget->m_renderTarget.m_vRenderTargetView[0],
                                           dStencil->m_pDepthStencilView);
    }
    else if ((nullptr != rt) && (nullptr == ds)) {
      SPtr<RenderTarget>temp(rt);
      setRenderTarget(temp);
    }
  }

  void
  GraphicsApiDX::setRenderTarget(WeakSPtr<RenderTarget> renderTarget) {
    auto rt = renderTarget.lock().get();

    if (nullptr != rt) {
      RenderTargetDX* rTarget = reinterpret_cast<RenderTargetDX*>(rt);

      if (nullptr != rTarget->m_pDepthStencilV) {
        m_pDeviceContext->OMSetRenderTargets(rTarget->m_renderTarget.m_vRenderTargetView.size(),
                                             &rTarget->m_renderTarget.m_vRenderTargetView[0],
                                             rTarget->m_pDepthStencilV);
      }
      else {
        m_pDeviceContext->OMSetRenderTargets(rTarget->m_renderTarget.m_vRenderTargetView.size(),
                                             &rTarget->m_renderTarget.m_vRenderTargetView[0],
                                             nullptr);
      }
    }
  }

  void
  GraphicsApiDX::setDepthStencil(WeakSPtr<Textures>depthStencil, const uint32 stencilRef) {
    auto ds = depthStencil.lock().get();
    if (nullptr != ds) {
      TexturesDX* dStencil = reinterpret_cast<TexturesDX*>(ds);

      m_pDeviceContext->OMSetDepthStencilState(dStencil->m_pDepthStencilState, stencilRef);
    }
  }

  void
  GraphicsApiDX::setInputLayout(WeakSPtr<InputLayout> vertexLayout) {
    auto vl = vertexLayout.lock().get();
    if (nullptr != vl) {
      InputLayoutDX* inputLayout = reinterpret_cast<InputLayoutDX*>(vl);

      m_pDeviceContext->IASetInputLayout(inputLayout->m_pVertexLayout);
    }
  }

  void
  GraphicsApiDX::setViewports(const uint32 width, 
                              const uint32 heigth,
                              const uint32 numViewports) {
    D3D11_VIEWPORT vp;
    vp.Width = (float)width;
    vp.Height = (float)heigth;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

    m_pDeviceContext->RSSetViewports(numViewports, &vp);
  }

  void
  GraphicsApiDX::setPrimitiveTopology(PRIMITIVE_TOPOLOGY::E topology) {
    switch (topology) {
      case PRIMITIVE_TOPOLOGY::kPointList:
        m_pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
        break;

      case PRIMITIVE_TOPOLOGY::kLineList:
        m_pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
        break;

      case PRIMITIVE_TOPOLOGY::kLineStrip:
        m_pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
        break;

      case PRIMITIVE_TOPOLOGY::kTriangleList:
        m_pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        break;

      case PRIMITIVE_TOPOLOGY::kTriangleStrip:
        m_pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        break;
    }
  }

  void
  GraphicsApiDX::setYourVS(WeakSPtr<Shaders> vertexShader) {
    auto vs = vertexShader.lock().get();
    if (nullptr != vs) {
      ShadersDX* vShader = reinterpret_cast<ShadersDX*>(vs);

      m_pDeviceContext->VSSetShader(vShader->m_pVertexShader, nullptr, 0);
    }
  }

  void
  GraphicsApiDX::setYourVSConstantBuffers(WeakSPtr<ConstantBuffer> constantBuffer,
                                          const uint32 startSlot,
                                          const uint32 numBuffers) {
    auto cb = constantBuffer.lock().get();
    if (nullptr != cb) {
      ConstantBufferDX* cBuffer = reinterpret_cast<ConstantBufferDX*>(cb);

      m_pDeviceContext->VSSetConstantBuffers(startSlot, 
                                             numBuffers,
                                             &cBuffer->m_pConstantBuffer);
    }
  }

  void
  GraphicsApiDX::setYourPS(WeakSPtr<Shaders> pixelShader) {
    auto ps = pixelShader.lock().get();
    if (nullptr != ps) {
      ShadersDX* pShader = reinterpret_cast<ShadersDX*>(ps);

      m_pDeviceContext->PSSetShader(pShader->m_pPixelShader, nullptr, 0);
    }
  }

  void
  GraphicsApiDX::setYourPSConstantBuffers(WeakSPtr<ConstantBuffer> constantBuffer,
                                          const uint32 startSlot, 
                                          const uint32 numBuffers) {
    auto cb = constantBuffer.lock().get();
    if (nullptr != cb) {
      ConstantBufferDX* cBuffer = reinterpret_cast<ConstantBufferDX*>(cb);

      m_pDeviceContext->PSSetConstantBuffers(startSlot, 
                                             numBuffers, 
                                             &cBuffer->m_pConstantBuffer);
    }
  }

  void
  GraphicsApiDX::setYourPSSampler(WeakSPtr<SamplerState> sampler, 
                                  const uint32 startSlot,
                                  const uint32 numSamplers) {
    auto tempSamp = sampler.lock().get();
    if (nullptr != tempSamp) {
      SamplerStateDX* SMP = reinterpret_cast<SamplerStateDX*>(tempSamp);

      m_pDeviceContext->PSSetSamplers(startSlot, numSamplers, &SMP->m_pSamplerState);
    }
  }

  void
  GraphicsApiDX::setShaders(WeakSPtr<Shaders> shaders) {
    auto tempShader = shaders.lock().get();
    if (nullptr != tempShader) {
      ShadersDX* shader = reinterpret_cast<ShadersDX*>(tempShader);

      m_pDeviceContext->VSSetShader(shader->m_pVertexShader, 0, 0);

      m_pDeviceContext->PSSetShader(shader->m_pPixelShader, 0, 0);
    }
  }

  void
  GraphicsApiDX::setConstBufferBones(WeakSPtr<ConstantBuffer> cbBones) {
    m_bonesBuffer = cbBones.lock();
  }

  void 
  GraphicsApiDX::setComputeUAV(WeakSPtr<ComputeBuffer> pComBuff,
                               uint32 startSlot, 
                               uint32 numUAV) {
    ComputeBuffer* tempCS = pComBuff.lock().get();
    if (nullptr != tempCS) {
      ComputeBufferDX* newCS = reinterpret_cast<ComputeBufferDX*>(tempCS);

      m_pDeviceContext->CSSetUnorderedAccessViews(startSlot,
                                                  numUAV,
                                                  &newCS->m_pComputeUAV,
                                                  nullptr);
    }
  }

  void 
  GraphicsApiDX::setRtUAV(WeakSPtr<RenderTarget> pCompBuffRT,
                           uint32 startSlot, 
                           uint32 numUAV) {
    RenderTarget* tempCB = pCompBuffRT.lock().get();
    if (nullptr != tempCB) {
      RenderTargetDX* newRT = reinterpret_cast<RenderTargetDX*>(tempCB);
      ComputeBufferDX* newCS = newRT->m_renderTarget.m_pCompBuff.get();

      m_pDeviceContext->CSSetUnorderedAccessViews(startSlot,
                                                  numUAV,
                                                  &newCS->m_pComputeUAV,
                                                  nullptr);
    }
  }

  void 
  GraphicsApiDX::setComputeShader(WeakSPtr<Shaders> shader) {
    auto tempShader = shader.lock().get();
    if (nullptr != tempShader) {
      ShadersDX* shader = reinterpret_cast<ShadersDX*>(tempShader);

      m_pDeviceContext->CSSetShader(shader->m_pComputeShader->m_pComputeShaderDX, nullptr, 0);
    }
  }

  void
  GraphicsApiDX::setCSConstantBuffer(WeakSPtr<ConstantBuffer> constBuffer,
                                     const uint32 startSlot, 
                                     const uint32 numBuffers) {
    auto cb = constBuffer.lock().get();
    if (nullptr != cb) {
      ConstantBufferDX* cBuffer = reinterpret_cast<ConstantBufferDX*>(cb);

      m_pDeviceContext->CSSetConstantBuffers(startSlot,
                                             numBuffers,
                                             &cBuffer->m_pConstantBuffer);
    }
  }



  void
  GraphicsApiDX::setCSSampler(WeakSPtr<SamplerState> sampler, 
                              uint32 startSlot,
                              uint32 numSamplers) {
    auto tempSamp = sampler.lock().get();
    if (nullptr != tempSamp) {
      SamplerStateDX* samplerDX = reinterpret_cast<SamplerStateDX*>(tempSamp);

      m_pDeviceContext->CSSetSamplers(startSlot, numSamplers, &samplerDX->m_pSamplerState);
    }
  }

  /***************************************************************************/
  /**
  * Gets.
  */
  /***************************************************************************/

  Textures*
  GraphicsApiDX::getDefaultBackBuffer() {
    return m_pBackBuffer;
  }

  Textures*
  GraphicsApiDX::getDefaultDepthStencil() {
    return m_pDepthStencil;
  }

  SPtr<ConstantBuffer>
  GraphicsApiDX::getConstBufferBones() {
    if (nullptr != m_bonesBuffer) {
      return m_bonesBuffer;
    }
    return nullptr;
  }

  void*
  GraphicsApiDX::getDevice() {
    if (nullptr != m_pd3dDevice) {
      return m_pd3dDevice;
    }
    return nullptr;
  }

  void*
  GraphicsApiDX::getDeviceContext() {
    if (nullptr != m_pDeviceContext) {
      return m_pDeviceContext;
    }
    return nullptr;
  }
}