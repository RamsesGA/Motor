#include "gaComputeShaderDX.h"

namespace gaEngineSDK {
  bool 
  ComputeShaderDX::compileComputeShaderFromFile(const WString& fileName, 
                                                const String& entryPoint, 
                                                const String& versionCS) {
    HRESULT hr = S_OK;
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
    ID3DBlob* pErrorBlob;

    hr = D3DCompileFromFile(fileName.c_str(),
                            nullptr,
                            nullptr,
                            entryPoint.c_str(),
                            versionCS.c_str(),
                            shaderFlags,
                            0,
                            &m_pBlob,
                            &pErrorBlob);

    if (FAILED(hr)) {
      if (pErrorBlob) {
        OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
        pErrorBlob->Release();
        return false;
      }
    }

    return true;
  }
}