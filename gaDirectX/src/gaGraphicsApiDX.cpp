#include "gaGraphicsApiDX.h"
#include "gaConstantBufferDX.h"
#include "gaTexturesDX.h"
#include "gaSamplerStateDX.h"
#include "gaInputLayoutDX.h"
#include "gaShadersDX.h"
#include "gaVertexBufferDX.h"
#include "gaIndexBufferDX.h"
//#include <stb_image.h>

#include <iostream>
#include <vector>

namespace gaEngineSDK {
  HRESULT 
  CompileShaderFromFile(const std::wstring& szFileName, 
                        const std::string& szEntryPoint,
                        const std::string& szShaderModel, 
                        ID3DBlob** ppBlobOut) {
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
  
  #if defined( DEBUG ) || defined( _DEBUG )
    dwShaderFlags |= D3DCOMPILE_DEBUG;
  #endif
  
    ID3DBlob* pErrorBlob;
    hr = D3DCompileFromFile(szFileName.c_str(),
                            nullptr, nullptr,
                            szEntryPoint.c_str(),
                            szShaderModel.c_str(),
                            dwShaderFlags, 0, ppBlobOut,
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
  AnalyzeVertexShaderDX(const std::wstring& _nameVS) {
    std::string bufferAnalyze;
  
    for (uint32 i = 0; i < _nameVS.size(); i++) {
      bufferAnalyze += _nameVS[i];
  
      if (('_' == bufferAnalyze[i]) &&
          ("DX_" != bufferAnalyze)) {
        return false;
      }
      else if (('_' == bufferAnalyze[i]) &&
               ("DX_" == bufferAnalyze)) {
        return true;
      }
    }
  
    return false;
  }
  
  bool 
  AnalyzePixelShaderDX(const std::wstring& _namePS) {
    std::string bufferAnalyze;
  
    for (uint32 i = 0; i < _namePS.size(); i++) {
      bufferAnalyze += _namePS[i];
  
      if (('_' == bufferAnalyze[i]) &&
          ("DX_" != bufferAnalyze)) {
        return false;
      }
      else if (('_' == bufferAnalyze[i]) &&
               ("DX_" == bufferAnalyze)) {
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
  
    ///Liberar mis miembros del API
    /// (SON LOS QUE TENGO DOMINIO PROPIO)
    delete m_pd3dDevice;
  
    delete m_pSwapChain;
  
    delete m_pImmediateContext;
  
    delete m_pDepthStencil;
  
    delete m_pBackBuffer;
  }
  
  
  /***************************************************************************/
  /**
  * Inheritance methods.
  */
  /***************************************************************************/
  
  bool 
  GraphicsApiDX::initDevice(HWND hWnd) {

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
    
    for (UINT driverTypeIndex = 0; driverTypeIndex < driverTypes.size(); driverTypeIndex++) {

      hr = D3D11CreateDeviceAndSwapChain(nullptr, driverTypes[driverTypeIndex],
                                         nullptr, createDeviceFlags,
                                         featureLevels.data(), featureLevels.size(),
                                         D3D11_SDK_VERSION, &sd,
                                         &m_pSwapChain, &m_pd3dDevice,
                                         &featureLevel, &m_pImmediateContext);
      if (SUCCEEDED(hr)) {
        break;
      }
    }
    
    if (FAILED(hr)) {
      return false;
    }
    
    auto* backBuffer = new TexturesDX();
    
    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
      (LPVOID*)&backBuffer->m_pTexture);
    
    ///Checamos que todo salga bien, si no mandamos un error
    if (FAILED(hr)) {
      delete backBuffer;
      return false;
    }
    
    hr = m_pd3dDevice->CreateRenderTargetView(backBuffer->m_pTexture,
      nullptr,
      &backBuffer->m_pRenderTargetView);
    
    if (FAILED(hr)) {
      delete backBuffer;
      return false;
    }
    
    m_pBackBuffer = backBuffer;
    
    auto* depthStencil = new TexturesDX();
    
    //Textura del depth y hago el depth
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
    
    ///Creamos la textura
    hr = m_pd3dDevice->CreateTexture2D(&textureDesc,
      nullptr, &depthStencil->m_pTexture);
    
    if (FAILED(hr)) {
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
    
    ///Checamos que todo salga bien, si no mandamos un error
    if (FAILED(hr)) {
      delete backBuffer;
      delete depthStencil;
      return false;
    }
    
    m_pImmediateContext->OMSetRenderTargets(1,
      &backBuffer->m_pRenderTargetView,
      depthStencil->m_pDepthStencilView);
    
    m_pDepthStencil = depthStencil;
    
    return true;
  }
  
  void 
  GraphicsApiDX::drawIndex(uint32 indexCountDX,
                           uint32 startIndexLocationDX,
                           uint32 baseVertexLocationDX) {
    m_pImmediateContext->DrawIndexed(indexCountDX, startIndexLocationDX,
                                     baseVertexLocationDX);
  }
  
  void 
  GraphicsApiDX::swapChainPresent(uint32 syncIntervalDX,
                                  uint32 flagsDX) {
    m_pSwapChain->Present(syncIntervalDX, flagsDX);
  }
  
  Textures* 
  GraphicsApiDX::loadTextureFromFile(std::string srcFile) {
  
    auto* texture = new TexturesDX();
    return texture;
    //int width;
    //int height;
    //int components;
    //unsigned char* data = stbi_load(srcFile.c_str(),
    //                                &width, &height, &components, 0);
    //if (!data) {
    //  return nullptr;
    //}
    //auto* texture = new TexturesDX();
    //D3D11_TEXTURE2D_DESC desc;
    //ZeroMemory(&desc, sizeof(desc));
    //desc.Width = width;
    //desc.Height = height;
    //desc.MipLevels = 1;
    //desc.ArraySize = 1;
    //desc.SampleDesc.Count = 1;
    //desc.SampleDesc.Quality = 0;
    //desc.Usage = D3D11_USAGE_DEFAULT;
    //desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    //desc.MiscFlags = 0;
    //if (1 == components) {
    //  desc.Format = DXGI_FORMAT_R16_FLOAT;
    //}
    //else if (2 == components) {
    //  desc.Format = DXGI_FORMAT_R16G16_FLOAT;
    //}
    //else if (3 == components) {
    //  desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
    //}
    //else if (4 == components) {
    //  desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
    //}
    ////Texture data
    //D3D11_SUBRESOURCE_DATA initData;
    //ZeroMemory(&initData, sizeof(initData));
    //initData.pSysMem = data;
    //initData.SysMemPitch = 1;
    //if (FAILED(m_pd3dDevice->CreateTexture2D(&desc,
    //  &initData,
    //  &texture->m_pTexture))) {
    //  delete texture;
    //  stbi_image_free(data);
    //  return nullptr;
    //}
    ////Shader resource data
    //D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
    //ZeroMemory(&viewDesc, sizeof(viewDesc));
    //viewDesc.Format = desc.Format;
    //viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    //viewDesc.Texture2D.MostDetailedMip = 0;
    //viewDesc.Texture2D.MipLevels = 1;
    //if (FAILED(m_pd3dDevice->CreateShaderResourceView(texture->m_pTexture,
    //  &viewDesc,
    //  &texture->m_pShaderResourceView))) {
    //  delete texture;
    //  stbi_image_free(data);
    //  return nullptr;
    //}
    //return texture;
  }
  
  void GraphicsApiDX::unbindOGL() {}
  
  /***************************************************************************/
  /**
  * Updates.
  */
  /***************************************************************************/
  
  void 
  GraphicsApiDX::updateConstantBuffer(const void* srcData,
                                      ConstantBuffer& updateDataCB) {
    auto& constantBuffer = reinterpret_cast<ConstantBufferDX&>(updateDataCB);
  
    m_pImmediateContext->UpdateSubresource(constantBuffer.m_pConstantBuffer, 0,
                                           nullptr, srcData,
                                           0, 0);
  }
  
  /***************************************************************************/
  /**
  * Clears.
  */
  /***************************************************************************/
  
  void 
  GraphicsApiDX::clearYourRenderTargetView(Textures* renderTarget,
                                           float r, float g, float b, float a) {
    float ClearColor[4] = { r, g, b, a };
  
    auto* renderTGT = reinterpret_cast<TexturesDX*>(renderTarget);
  
    m_pImmediateContext->ClearRenderTargetView(renderTGT->m_pRenderTargetView,
                                               ClearColor);
  }
  
  void 
  GraphicsApiDX::clearYourDepthStencilView(Textures* depthStencilDX) {
    auto* depthStencil = reinterpret_cast<TexturesDX*>(depthStencilDX);
  
    m_pImmediateContext->ClearDepthStencilView(depthStencil->m_pDepthStencilView,
                                               D3D11_CLEAR_DEPTH, 1.0f, 0);
  }
  
  /***************************************************************************/
  /**
  * Creates.
  */
  /***************************************************************************/
  
  Shaders* 
  GraphicsApiDX::createShadersProgram(const std::wstring& nameVS,
                                      const std::string& entryPointVS, 
                                      const std::wstring& namePS,
                                      const std::string& entryPointPS) {
    ///Generamos una variable auto
    ///para adaptar el tipo de dato que ocupamos
    auto* shaders = new ShadersDX();
  
    if (!(AnalyzeVertexShaderDX(nameVS))) {
      delete shaders;
      return nullptr;
    }
    if (!(AnalyzePixelShaderDX(namePS))) {
      delete shaders;
      return nullptr;
    }
  
    ///Asignamos datos a las variables
    shaders->m_pPSBlob = NULL;
    HRESULT hr = S_OK;
  
    ///Compilamos el shader recibido
    hr = CompileShaderFromFile(namePS, entryPointPS.c_str(), "ps_4_0", &shaders->m_pPSBlob);
  
    ///Checamos que todo salga bien, si no mandamos un error
    if (FAILED(hr)) {
      delete shaders;
      return nullptr;
    }
  
    ///Creamos el pixel shader con la función de DX
    hr = m_pd3dDevice->CreatePixelShader(shaders->m_pPSBlob->GetBufferPointer(),
                                         shaders->m_pPSBlob->GetBufferSize(),
                                         nullptr, &shaders->m_pPixelShader);
    shaders->m_pPSBlob->Release();
  
    ///Finalmente regresamos el dato en caso
    ///de no obtener un error
    if (FAILED(hr)) {
      delete shaders;
      return nullptr;
    }
  
    ///Asignamos datos a las variables
    shaders->m_pVSBlob = NULL;
  
    ///Compilamos el shader recibido
    hr = CompileShaderFromFile(nameVS, entryPointVS.c_str(), "vs_4_0", &shaders->m_pVSBlob);
  
    ///Checamos que todo salga bien, si no mandamos un error
    if (FAILED(hr)) {
      delete shaders;
      return nullptr;
    }
  
    ///Creamos el vertex shader con la función de DX
    hr = m_pd3dDevice->CreateVertexShader(shaders->m_pVSBlob->GetBufferPointer(),
                                          shaders->m_pVSBlob->GetBufferSize(),
                                          nullptr, &shaders->m_pVertexShader);
    ///Finalmente regresamos el dato en caso
    ///de no obtener un error
    if (FAILED(hr)) {
      shaders->m_pVSBlob->Release();
      delete shaders;
      return nullptr;
    }
  
    return shaders;
  }
  
  VertexBuffer* 
  GraphicsApiDX::createVertexBuffer(const void* data,
                                    const uint32 size) {
    ///Generamos una variable auto
    ///para adaptar el tipo de dato que ocupamos
    auto* VB = new VertexBufferDX();
  
    ///Asignamos datos a la variable
    HRESULT hr = S_OK;
  
    if (0 != size) {
      ///Rellenamos el descriptor de buffer
      CD3D11_BUFFER_DESC bd(size, D3D11_BIND_VERTEX_BUFFER);
  
      if (nullptr != data) {
        ///Generamos una variable descriptor
        D3D11_SUBRESOURCE_DATA InitData;
        ///Limpiamos la memoria y dejamos
        ///definido todo en 0
        ZeroMemory(&InitData, sizeof(InitData));
        ///Asignamos datos a las variables
        InitData.pSysMem = data;
  
        ///Creamos el buffer
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData,
          &VB->m_pVertexBuffer);
  
        if (FAILED(hr)) {
          delete VB;
          return nullptr;
        }
  
        return VB;
      }
      else {
        hr = m_pd3dDevice->CreateBuffer(&bd, nullptr, &VB->m_pVertexBuffer);
  
        if (FAILED(hr)) {
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
  GraphicsApiDX::createIndexBuffer(const void* data,
                                   const uint32 size) {
    ///Generamos una variable auto
    ///para adaptar el tipo de dato que ocupamos
    auto* IB = new IndexBufferDX();
  
    ///Asignamos datos a la variable
    HRESULT hr = S_OK;
  
    if (0 != size) {
      ///Rellenamos el descriptor de buffer
      D3D11_BUFFER_DESC bd;
      ZeroMemory(&bd, sizeof(bd));
      bd.Usage = D3D11_USAGE_DEFAULT;
      bd.ByteWidth = size;
      bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
      bd.CPUAccessFlags = 0;
      bd.MiscFlags = 0;
  
      if (nullptr != data) {
        ///Generamos una variable descriptor
        D3D11_SUBRESOURCE_DATA InitData;
        ///Limpiamos la memoria y dejamos
        ///definido todo en 0
        ZeroMemory(&InitData, sizeof(InitData));
        ///Asignamos datos a las variables
        InitData.pSysMem = data;
        InitData.SysMemPitch = 0;
        InitData.SysMemSlicePitch = 0;
  
        ///Creamos el buffer
        hr = m_pd3dDevice->CreateBuffer(&bd, &InitData,
                                        &IB->m_pIndexBuffer);
        if (FAILED(hr)) {
          delete IB;
          return nullptr;
        }
        return IB;
      }
      else {
        hr = m_pd3dDevice->CreateBuffer(&bd, nullptr, &IB->m_pIndexBuffer);
  
        if (FAILED(hr)) {
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
    ///Generamos una variable auto
    ///para adaptar el tipo de dato que ocupamos
    auto* constantBuffer = new ConstantBufferDX();
  
    ///Asignamos datos a la variable
    HRESULT hr = S_OK;
  
    ///Rellenamos el descriptor de buffer
    CD3D11_BUFFER_DESC bd(bufferSize, D3D11_BIND_CONSTANT_BUFFER);
  
    ///Creamos el buffer
    hr = m_pd3dDevice->CreateBuffer(&bd, nullptr,
                                    &constantBuffer->m_pConstantBuffer);
  
    ///Finalmente regresamos el dato en caso
    ///de no obtener un error
    if (FAILED(hr)) {
      delete constantBuffer;
      return nullptr;
    }
  
    return constantBuffer;
  }
  
  Textures* 
  GraphicsApiDX::createTexture(const uint32 width,
                               const uint32 height,
                               const uint32 bindFlags,
                               TEXTURE_FORMAT textureFormat,
                               const std::string fileName) {
    ///Asignamos datos a la variable
    HRESULT hr = S_OK;
  
    ///Generamos una variable auto
    ///para adaptar el tipo de dato que ocupamos
    auto* texture = new TexturesDX();
  
    ///Rellenamos el descriptor
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
  
    ///Creamos la textura
    hr = m_pd3dDevice->CreateTexture2D(&textureDesc, nullptr, &texture->m_pTexture);
  
    if (FAILED(hr)) {
      delete texture;
      return nullptr;
    }
  
    ///RenderTargetView
    if (bindFlags & D3D11_BIND_RENDER_TARGET) {
      hr = m_pd3dDevice->CreateRenderTargetView(texture->m_pTexture,
        nullptr,
        &texture->m_pRenderTargetView);
  
      if (FAILED(hr)) {
        delete texture;
        return nullptr;
      }
  
      return texture;
    }
  
    ///DepthStencilView
    if (bindFlags & D3D11_BIND_DEPTH_STENCIL) {
      D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc;
      ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
      depthStencilDesc.Format = textureDesc.Format;
      depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
      depthStencilDesc.Texture2D.MipSlice = 0;
  
      hr = m_pd3dDevice->CreateDepthStencilView(texture->m_pTexture,
                                                &depthStencilDesc,
                                                &texture->m_pDepthStencilView);
      ///Checamos que todo salga bien, si no mandamos un error
      if (FAILED(hr)) {
        delete texture;
        return nullptr;
      }
  
      return texture;
    }
  
    ///ShaderResourceView
    if (bindFlags & D3D11_BIND_SHADER_RESOURCE) {
      CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc(D3D11_SRV_DIMENSION_TEXTURE2D);
  
      hr = m_pd3dDevice->CreateShaderResourceView(texture->m_pTexture,
        &shaderResourceViewDesc,
        &texture->m_pShaderResourceView);
  
      ///Checamos que todo salga bien, si no mandamos un error
      if (FAILED(hr)) {
  
        delete texture;
        return nullptr;
      }
  
      return texture;
    }
  }
  
  SamplerState* 
  GraphicsApiDX::createSamplerState() {
    ///Generamos una variable auto
    ///para adaptar el tipo de dato que ocupamos
    auto* samplerState = new SamplerStateDX();
  
    ///Asignamos datos a la variable
    HRESULT hr = S_OK;
  
    ///Definimos el sampler state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
  
    hr = m_pd3dDevice->CreateSamplerState(&sampDesc,
                                          &samplerState->m_pSamplerState);
    ///Finalmente regresamos el dato en caso
    ///de no obtener un error
    if (FAILED(hr)) {
      delete samplerState;
      return nullptr;
    }
  
    return samplerState;
  }
  
  InputLayout* 
  GraphicsApiDX::createInputLayout(Shaders& vertexShaderDX) {
    ///Asignamos datos a la variable
    HRESULT hr = S_OK;
  
    auto* inputLayout = new InputLayoutDX();
    auto& vertexShader = reinterpret_cast<ShadersDX&>(vertexShaderDX);
  
    ///Generamos la información del shader
    ID3D11ShaderReflection* pVertexShaderReflection = nullptr;
  
    if (FAILED(D3DReflect(vertexShader.m_pVSBlob->GetBufferPointer(),
                          vertexShader.m_pVSBlob->GetBufferSize(),
                          IID_ID3D11ShaderReflection,
                          (void**)&pVertexShaderReflection))) {
      delete inputLayout;
      pVertexShaderReflection->Release();
      delete pVertexShaderReflection;
      return nullptr;
    }
  
    ///Generamos una descriptor del shader
    D3D11_SHADER_DESC shaderDesc;
  
    ///Obtenemos la información de nuestro
    ///descriptor del shader
    pVertexShaderReflection->GetDesc(&shaderDesc);
  
    ///Leer la descripción del input layout de 
    /// la información del sombreador
    uint32 byteOffset = 0;
    Vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
  
    for (uint32 i = 0; i < shaderDesc.InputParameters; i++) {
      D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
      pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);
  
      ///Creamos y guardamos la información
      ///para input element
      D3D11_INPUT_ELEMENT_DESC elementDesc;
  
      ///Rellenamos 6 datos de 7 el formato es
      ///lo que sigue
      elementDesc.SemanticName = paramDesc.SemanticName;
      elementDesc.SemanticIndex = paramDesc.SemanticIndex;
      elementDesc.InputSlot = 0;
      elementDesc.AlignedByteOffset = byteOffset;
      elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
      elementDesc.InstanceDataStepRate = 0;
  
      ///Determinamos el formato DXGI
      /// DXGI_FORMAT_R32_UINT
      /// DXGI_FORMAT_R32_SINT
      /// DXGI_FORMAT_R32_FLOAT
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
      /// DXGI_FORMAT_R32G32_UINT
      /// DXGI_FORMAT_R32G32_SINT
      /// DXGI_FORMAT_R32G32_FLOAT
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
      /// DXGI_FORMAT_R32G32B32_UINT
      /// DXGI_FORMAT_R32G32B32_SINT
      /// DXGI_FORMAT_R32G32B32_FLOAT
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
      /// DXGI_FORMAT_R32G32B32A32_UINT
      /// DXGI_FORMAT_R32G32B32A32_SINT
      /// DXGI_FORMAT_R32G32B32A32_FLOAT
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
  
      ///Guardamos el element desc
      inputLayoutDesc.push_back(elementDesc);
    }
  
    ///Creamos el input layout
    hr = m_pd3dDevice->CreateInputLayout(&inputLayoutDesc[0],
                                         inputLayoutDesc.size(), 
                                         vertexShader.m_pVSBlob->GetBufferPointer(),
                                         vertexShader.m_pVSBlob->GetBufferSize(), 
                                         &inputLayout->m_pVertexLayout);
    ///Liberamos el puntero blob
    vertexShader.m_pVSBlob->Release();
  
    ///Liberación de memoria de reflexión de shader de asignación
    pVertexShaderReflection->Release();
  
    ///Checamos que todo salga bien, si no mandamos un error
    if (FAILED(hr)) {
      delete inputLayout;
      delete pVertexShaderReflection;
      return nullptr;
    }
  
    ///Registro de longitud de bytes
    inputLayout->m_inputLayoutByteLength = &byteOffset;
  
    return inputLayout;
  }
  
  /***************************************************************************/
  /**
  * Sets.
  */
  /***************************************************************************/
  
  void 
  GraphicsApiDX::setPixelShader(Shaders& pixelShaderDX) {
    ///Generamos una variable auto
    ///para adaptar el tipo de dato que ocupamos
    ///Y hacemos un casteo reinterprete para convertir el puntero
    auto& pixelShader = reinterpret_cast<ShadersDX&> (pixelShaderDX);
  
    m_pImmediateContext->PSSetShader(pixelShader.m_pPixelShader, NULL, 0);
  }
  
  void 
  GraphicsApiDX::setVertexShader(Shaders& vertexShaderDX) {
    auto& vertexShader = reinterpret_cast<ShadersDX&>(vertexShaderDX);
  
    m_pImmediateContext->VSSetShader(vertexShader.m_pVertexShader, NULL, 0);
  }
  
  void 
  GraphicsApiDX::setVertexBuffer(VertexBuffer& vertexBufferDX) {
    auto& vertexBuffer = reinterpret_cast<VertexBufferDX&>(vertexBufferDX);
  
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
  
    m_pImmediateContext->IASetVertexBuffers(0,
                                            1,
                                            &vertexBuffer.m_pVertexBuffer,
                                            &stride,
                                            &offset);
  }
  
  void 
  GraphicsApiDX::setIndexBuffer(IndexBuffer& indexBufferDX) {
    auto& indexBuffer = reinterpret_cast<IndexBufferDX&>(indexBufferDX);
  
    m_pImmediateContext->IASetIndexBuffer(indexBuffer.m_pIndexBuffer,
                                          DXGI_FORMAT_R32_UINT,
                                          0);
  }
  
  void 
  GraphicsApiDX::setConstantBuffer(bool isVertex,
                                   ConstantBuffer& constantBuffer,
                                   const uint32 startSlot,
                                   const uint32 numBuffers) {
    auto& constBuffer = reinterpret_cast<ConstantBufferDX&>(constantBuffer);
  
    if (true == isVertex) {
      m_pImmediateContext->VSSetConstantBuffers(startSlot, numBuffers,
                                                &constBuffer.m_pConstantBuffer);
    }
    else {
      m_pImmediateContext->PSSetConstantBuffers(startSlot, numBuffers,
                                                &constBuffer.m_pConstantBuffer);
    }
  }
  
  void 
  GraphicsApiDX::setSamplerState(const uint32 startSlotDX,
                                 Vector<SamplerState*>& samplerStateDX,
                                 Textures& texture) {
  
    for (uint32 i = 0; i < samplerStateDX.size(); i++) {
      auto* samplerState = reinterpret_cast<SamplerStateDX*>(samplerStateDX[i]);
  
      m_pImmediateContext->PSSetSamplers(startSlotDX,
                                         samplerStateDX.size(),
                                         &samplerState->m_pSamplerState);
    }
  }
  
  void
  GraphicsApiDX::setShaderResourceView(Textures* shaderResourceViewDX,
                                       const uint32 startSlotDX,
                                       const uint32 numViewsDX) {
    auto* shaderResource = reinterpret_cast<TexturesDX*>(shaderResourceViewDX);
  
    m_pImmediateContext->PSSetShaderResources(startSlotDX, numViewsDX,
                                              &shaderResource->m_pShaderResourceView);
  }
  
  void
  GraphicsApiDX::setRenderTarget(Textures* renderTargetDX,
                                 Textures* depthStencilDX) {
    auto* depthStencil = reinterpret_cast<TexturesDX*>(depthStencilDX);
    auto* renderTarget = reinterpret_cast<TexturesDX*>(renderTargetDX);
  
    m_pImmediateContext->OMSetRenderTargets(1, &renderTarget->m_pRenderTargetView,
                                            depthStencil->m_pDepthStencilView);
  }
  
  void 
  GraphicsApiDX::setDepthStencil(Textures& depthStencilDX,
                                 const uint32 stencilRefDX) {
    auto& depthStencil = reinterpret_cast<TexturesDX&>(depthStencilDX);
  
    m_pImmediateContext->OMSetDepthStencilState(depthStencil.m_pDepthStencilState,
                                                stencilRefDX);
  }
  
  void 
  GraphicsApiDX::setInputLayout(InputLayout& vertexLayoutDX) {
    auto& inputLayout = reinterpret_cast<InputLayoutDX&>(vertexLayoutDX);
  
    m_pImmediateContext->IASetInputLayout(inputLayout.m_pVertexLayout);
  }
  
  void 
  GraphicsApiDX::setViewport(const uint32 numViewportsDX,
                             const uint32 widthDX, 
                             const uint32 heigthDX) {
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)widthDX;
    vp.Height = (FLOAT)heigthDX;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
  
    m_pImmediateContext->RSSetViewports(numViewportsDX, &vp);
  }
  
  void 
  GraphicsApiDX::setPrimitiveTopology(const uint32 topologyDX) {
    m_pImmediateContext->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)topologyDX);
  }
  
  void 
  GraphicsApiDX::setYourVS(Shaders& vertexShaderDX) {
    auto& vertexShader = reinterpret_cast<ShadersDX&>(vertexShaderDX);
  
    m_pImmediateContext->VSSetShader(vertexShader.m_pVertexShader,
                                     nullptr, 0);
  }
  
  void
  GraphicsApiDX::setYourVSConstantBuffers(ConstantBuffer* constantBufferDX,
                                          const uint32 startSlotDX,
                                          const uint32 numBuffersDX) {
    auto* constantBuffer = reinterpret_cast<ConstantBufferDX*>(constantBufferDX);
  
    m_pImmediateContext->VSSetConstantBuffers(startSlotDX, numBuffersDX,
                                              &constantBuffer->m_pConstantBuffer);
  }
  
  void 
  GraphicsApiDX::setYourPS(Shaders& pixelShaderDX) {
    auto& pixelShader = reinterpret_cast<ShadersDX&>(pixelShaderDX);
  
    m_pImmediateContext->PSSetShader(pixelShader.m_pPixelShader,
                                     nullptr, 0);
  }
  
  void 
  GraphicsApiDX::setYourPSConstantBuffers(ConstantBuffer* constantBufferDX,
                                          const uint32 startSlotDX,
                                          const uint32 numBuffersDX) {
    auto* constantBuffer = reinterpret_cast<ConstantBufferDX*>(constantBufferDX);
  
    m_pImmediateContext->PSSetConstantBuffers(startSlotDX, numBuffersDX,
                                              &constantBuffer->m_pConstantBuffer);
  }
  
  void
  GraphicsApiDX::setYourPSSampler(SamplerState& samplerDX,
                                  const uint32 startSlotDX,
                                  const uint32 numSamplersDX) {
    auto& sampler = reinterpret_cast<SamplerStateDX&>(samplerDX);
  
    m_pImmediateContext->PSSetSamplers(startSlotDX, numSamplersDX,
                                       &sampler.m_pSamplerState);
  }
  
  void
  GraphicsApiDX::setShaders(Shaders& shaders) {
  
    auto& shader = reinterpret_cast<ShadersDX&>(shaders);
  
    m_pImmediateContext->VSSetShader(shader.m_pVertexShader,
                                     0, 0);

    m_pImmediateContext->PSSetShader(shader.m_pPixelShader,
                                     0, 0);
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
}