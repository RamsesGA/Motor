#include "gaGraphicsApiOGL.h"
#include "gaConstantBufferOGL.h"
#include "gaTexturesOGL.h"
#include "gaSamplerStateOGL.h"
#include "gaInputLayoutOGL.h"
#include "gaShadersOGL.h"
#include "gaVertexBufferOGL.h"
#include "gaIndexBufferOGL.h"
//#include <stb_image.h>

#include <iostream>
#include <vector>

namespace gaEngineSDK {

  std::string 
  ReadShaderOGL(const std::wstring& _nameVS) {
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      shaderFile.open(_nameVS);

      std::stringstream shaderStream;
      shaderStream << shaderFile.rdbuf();

      shaderFile.close();

      return shaderStream.str();
    }
    catch (std::ifstream::failure e) {
      std::cout << "ERROR, no se pudo leer el shader\n";
      return "";
    }
  }

  bool 
  AnalyzeVertexShaderOGL(const std::wstring& _nameVS) {
    std::string bufferAnalyze;

    for (unsigned int i = 0; i < _nameVS.size(); i++) {

      bufferAnalyze += _nameVS[i];

      if (('_' == bufferAnalyze[i]) &&
          ("OGL_" != bufferAnalyze)) {
        return false;
      }
      else if (('_' == bufferAnalyze[i]) &&
               ("OGL_" == bufferAnalyze)) {
        return true;
      }
    }

    return false;
  }

  bool 
  AnalyzePixelShaderOGL(const std::wstring& _namePS) {
    std::string bufferAnalyze;

    for (unsigned int i = 0; i < _namePS.size(); i++) {

      bufferAnalyze += _namePS[i];

      if (('_' == bufferAnalyze[i]) &&
          ("OGL_" != bufferAnalyze)) {
        return false;
      }
      else if (('_' == bufferAnalyze[i]) &&
               ("OGL_" == bufferAnalyze)) {
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

  GraphicsApiOGL::~GraphicsApiOGL() {
    // delete the rendering context  
    wglDeleteContext(m_renderingContext);
  }
  
  
  /***************************************************************************/
  /**
  * Inheritance methods.
  */
  /***************************************************************************/
  
  bool 
  GraphicsApiOGL::initDevice(HWND hWnd) {

    PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
      PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
      32,                   // Colordepth of the framebuffer.
      0, 0, 0, 0, 0, 0,
      0,
      0,
      0,
      0, 0, 0, 0,
      24,                   // Number of bits for the depthbuffer
      8,                    // Number of bits for the stencilbuffer
      0,                    // Number of Aux buffers in the framebuffer.
      PFD_MAIN_PLANE,
      0,
      0, 0, 0
    };

    m_hWnd = hWnd;

    m_HandleToDC = GetDC(hWnd);

    int pixelFormat = ChoosePixelFormat(m_HandleToDC, &pfd);

    SetPixelFormat(m_HandleToDC, pixelFormat, &pfd);

    m_renderingContext = wglCreateContext(m_HandleToDC);

    wglMakeCurrent(m_HandleToDC, m_renderingContext);

    if (!gladLoadGL()) {
      return false;
    }

    RECT rc;

    GetClientRect(hWnd, &rc);

    m_width = rc.right - rc.left;
    m_height = rc.bottom - rc.top;

    setViewport(1, m_width, m_height);

    glEnable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }

    return true;
  }
  
  void 
  GraphicsApiOGL::drawIndex(uint32 indexCount,
                            uint32 startIndexLocation,
                            uint32 baseVertexLocation) {
    glDrawElements(m_topology, indexCount,
                   GL_UNSIGNED_INT, 0);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  void 
  GraphicsApiOGL::swapChainPresent(uint32 syncInterval,
                                   uint32 flags) {
    SwapBuffers(m_HandleToDC);
  }
  
  Textures* 
  GraphicsApiOGL::loadTextureFromFile(std::string srcFile) {

    auto* texture = new TexturesOGL();

    return texture;
    //int width;
    //int height;
    //int components;
    //unsigned char* data = stbi_load(srcFile.c_str(),
    //                                &width, &height, &components, 0);
    //if (!data) {
    //  return nullptr;
    //  stbi_image_free(data);
    //}
    //else {
    //  GLenum format = GL_ZERO;
    //  if (1 == components) {
    //    format = GL_RED;
    //  }
    //  else if (2 == components) {
    //    format = GL_RG;
    //  }
    //  else if (3 == components) {
    //    format = GL_RGB;
    //  }
    //  else if (4 == components) {
    //    format = GL_RGBA;
    //  }
    //  auto* texture = new TexturesOGL();
    //  glGenTextures(1, &texture->m_texture);
    //  glBindTexture(GL_TEXTURE_2D, texture->m_texture);
    //  glTexImage2D(GL_TEXTURE_2D,
    //               0,
    //               format,
    //               width,
    //               height,
    //               0,
    //               format,
    //               GL_UNSIGNED_BYTE,
    //               data);
    //  glGenerateMipmap(GL_TEXTURE_2D);
    //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //  glBindTexture(GL_TEXTURE_2D, 0);
    //  stbi_image_free(data);
    //  return texture;
    //}
  }
  
  void GraphicsApiOGL::unbindOGL() {
    glUseProgram(0);
  }
  
  /***************************************************************************/
  /**
  * Updates.
  */
  /***************************************************************************/
  
  void 
  GraphicsApiOGL::updateConstantBuffer(const void* srcData,
                                      ConstantBuffer& updateDataCB) {
    auto& UBO = reinterpret_cast<ConstantBufferOGL&>(updateDataCB);

    glBindBuffer(GL_UNIFORM_BUFFER, UBO.m_uniformBufferObject);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, UBO.m_bufferSize, srcData);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  /***************************************************************************/
  /**
  * Clears.
  */
  /***************************************************************************/
  
  void 
  GraphicsApiOGL::clearYourRenderTargetView(Textures* renderTarget,
                                           float r, float g, float b, float a) {
    glClearColor(r, g, b, a);

    glClear(GL_COLOR_BUFFER_BIT);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  void 
  GraphicsApiOGL::clearYourDepthStencilView(Textures* depthStencil) {
    glClear(GL_DEPTH_BUFFER_BIT);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  /***************************************************************************/
  /**
  * Creates.
  */
  /***************************************************************************/
  
  Shaders* 
  GraphicsApiOGL::createShadersProgram(const std::wstring& nameVS,
                                      const std::string& entryPointVS, 
                                      const std::wstring& namePS,
                                      const std::string& entryPointPS) {
    if (!(AnalyzeVertexShaderOGL(nameVS))) {
      return nullptr;
    }
    if (!(AnalyzePixelShaderOGL(namePS))) {
      return nullptr;
    }

    std::string VS_ShaderSrc = ReadShaderOGL(nameVS);
    std::string PS_ShaderSrc = ReadShaderOGL(namePS);

    //Generamos el tipo de dato para ir guardando
    //los datos necesarios y entregarlo al usuario
    auto* shaders = new ShadersOGL();

    // Create an empty vertex shader handle
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar* source = (const GLchar*)VS_ShaderSrc.c_str();
    glShaderSource(vertexShader, 1, &source, nullptr);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
      GLint maxLength = 0;
      glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

      // We don't need the shader anymore.
      glDeleteShader(vertexShader);
      delete shaders;

      return nullptr;
    }

    shaders->m_vertexShader = vertexShader;

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = (const GLchar*)PS_ShaderSrc.c_str();
    glShaderSource(fragmentShader, 1, &source, nullptr);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
      GLint maxLength = 0;
      glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

      // We don't need the shader anymore.
      glDeleteShader(fragmentShader);

      // Either of them. Don't leak shaders.
      glDeleteShader(vertexShader);
      delete shaders;

      return nullptr;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    shaders->m_rendererID = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(shaders->m_rendererID, vertexShader);
    glAttachShader(shaders->m_rendererID, fragmentShader);

    // Link our program
    glLinkProgram(shaders->m_rendererID);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(shaders->m_rendererID, GL_LINK_STATUS, (int*)&isLinked);

    if (isLinked == GL_FALSE) {
      GLint maxLength = 0;
      glGetProgramiv(shaders->m_rendererID, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(shaders->m_rendererID, maxLength, &maxLength, &infoLog[0]);

      // We don't need the program anymore.
      glDeleteProgram(shaders->m_rendererID);
      // Don't leak shaders either.
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);
      delete shaders;

      return nullptr;
    }

    shaders->m_fragmentShader = fragmentShader;

    // Always detach shaders after a successful link.
    glDetachShader(shaders->m_rendererID, vertexShader);
    glDetachShader(shaders->m_rendererID, fragmentShader);

    return shaders;
  }
  
  VertexBuffer* 
  GraphicsApiOGL::createVertexBuffer(const void* data,
                                    const uint32 size) {
    auto* VBO = new VertexBufferOGL();

    ///Guardamos el tamaño del buffer
    VBO->m_vertexBufferSize = size;

    ///Creamos objetos de búfer y devolvemos los identificadores de los objetos de búfer
    glGenBuffers(1, &VBO->m_vertexBufferObject);

    ///Enlazamos VBO para la matriz de vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO->m_vertexBufferObject);

    ///Copiamos los datos en el objeto de búfer
    glBufferData(GL_ARRAY_BUFFER,
                 VBO->m_vertexBufferSize,
                 data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      delete VBO;
      exit(1);
    }

    return VBO;
  }
  
  IndexBuffer* 
  GraphicsApiOGL::createIndexBuffer(const void* data,
                                   const uint32 size) {
    auto* EBO = new IndexBufferOGL();

    EBO->m_indexBufferSize = size;

    ///Creamos objetos de búfer y devolvemos los identificadores de los objetos de búfer
    glGenBuffers(1, &EBO->m_indexBufferObject);

    ///Enlazamos el EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO->m_indexBufferObject);

    ///Copiamos los datos en el objeto de búfer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, EBO->m_indexBufferSize, data,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      delete EBO;
      exit(1);
    }

    return EBO;
  }
  
  ConstantBuffer* 
  GraphicsApiOGL::createConstantBuffer(const uint32 bufferSize) {
    auto* UBO = new ConstantBufferOGL();

    ///Generamos el buffer y lo inicializamos
    glGenBuffers(1, &UBO->m_uniformBufferObject);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO->m_uniformBufferObject);
    glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    UBO->m_bufferSize = bufferSize;

    GLuint detectError = glGetError();

    if (detectError != 0) {
      delete UBO;
      exit(1);
    }

    return UBO;
  }
  
  Textures* 
  GraphicsApiOGL::createTexture(const uint32 width,
                               const uint32 height,
                               const uint32 bindFlags,
                               TEXTURE_FORMAT::E textureFormat,
                               const std::string fileName) {
    auto* tex = new TexturesOGL();

    unsigned int texture;

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    //int width, height, nrChannels;

    //unsigned char* data = stbi_load(_fileName.c_str(), &width, &height, &nrChannels, 4);

    //if (data) {
    //  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //  glGenerateMipmap(GL_TEXTURE_2D);
    //  tex->m_texture = texture;
    //  tex->m_data = *data;
    //}
    //else {
    //  std::cout << "Failed to load texture" << std::endl;
    //}

    //stbi_image_free(data);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      delete tex;
      exit(1);
    }

    return tex;
  }
  
  SamplerState* 
  GraphicsApiOGL::createSamplerState() {
    auto* samplerState = new SamplerStateOGL();

    glGenSamplers(1, &samplerState->m_samplerState);
    glSamplerParameteri(samplerState->m_samplerState, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(samplerState->m_samplerState, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glSamplerParameteri(samplerState->m_samplerState, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(samplerState->m_samplerState, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      delete samplerState;
      exit(1);
    }

    return samplerState;
  }
  
  InputLayout* 
  GraphicsApiOGL::createInputLayout(Shaders& vertexShader) {
    auto* inputLayout = new InputLayoutOGL();
    auto& shader = reinterpret_cast<ShadersOGL&>(vertexShader);

    ///Creamos el vertex array
    glGenVertexArrays(1, &inputLayout->m_inputLayout);

    ///Ahora indicamos a OGL que lo vamos a usar
    glBindVertexArray(inputLayout->m_inputLayout);

    bool firstOffSet = true;
    unsigned int offSet = 0;
    unsigned int sizeComponent = 0;
    int total = -1;

    glGetProgramiv(shader.m_rendererID, GL_ACTIVE_ATTRIBUTES, &total);

    for (unsigned int i = 0; i < total; i++) {
      char name[100];
      memset(name, ' ', 100);

      int num = -1;
      int name_len = -1;

      GLenum type = GL_ZERO;

      glGetActiveAttrib(shader.m_rendererID, GLuint(i),
                        sizeof(name) - 1, &name_len, &num,
                        &type, name);

      name[name_len] = 0;

      GLuint location = glGetAttribLocation(shader.m_rendererID, name);

      ///Switch para obtener el tamaño del componente
      /// y asignar su offset correspondiente
      switch (type) {

        case GL_FLOAT_VEC2:
          sizeComponent = 2;
          if (!firstOffSet) {
            offSet += 8;
          }
          break;
        
        case GL_FLOAT_VEC3:
          sizeComponent = 3;
          if (!firstOffSet) {
            offSet += 12;
          }
          break;
        
        case GL_FLOAT_VEC4:
          sizeComponent = 4;
          if (!firstOffSet) {
            offSet += 16;
          }
          break;
      }

      glVertexAttribFormat(location, sizeComponent, GL_FLOAT, false, offSet);

      GLuint detectError = glGetError();

      if (detectError != 0) {
        delete inputLayout;
        exit(1);
      }

      glVertexAttribBinding(location, 0);
      glEnableVertexAttribArray(location);
    }

    GLuint detectError = glGetError();

    if (detectError != 0) {
      delete inputLayout;
      exit(1);
    }

    return inputLayout;
  }
  
  /***************************************************************************/
  /**
  * Sets.
  */
  /***************************************************************************/
  
  void 
  GraphicsApiOGL::setPixelShader(Shaders& pixelShader) {
    auto& shader = reinterpret_cast<ShadersOGL&>(pixelShader);

    glUseProgram(shader.m_rendererID);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  void 
  GraphicsApiOGL::setVertexShader(Shaders& vertexShader) {
    auto& shader = reinterpret_cast<ShadersOGL&>(vertexShader);

    glUseProgram(shader.m_rendererID);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  void 
  GraphicsApiOGL::setVertexBuffer(VertexBuffer& vertexBuffer) {
    auto& vertex = reinterpret_cast<VertexBufferOGL&>(vertexBuffer);

    glBindVertexBuffer(0, vertex.m_vertexBufferObject, 0, sizeof(Vertex));

    GLuint detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  void 
  GraphicsApiOGL::setIndexBuffer(IndexBuffer& indexBuffer) {
    auto& index = reinterpret_cast<IndexBufferOGL&>(indexBuffer);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index.m_indexBufferObject);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  void 
  GraphicsApiOGL::setConstantBuffer(bool isVertex,
                                   ConstantBuffer& constantBuffer,
                                   const uint32 startSlot,
                                   const uint32 numBuffers) {
    auto& cBuffer = reinterpret_cast<ConstantBufferOGL&>(constantBuffer);

    glBindBuffer(GL_UNIFORM_BUFFER, cBuffer.m_uniformBufferObject);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  void 
  GraphicsApiOGL::setSamplerState(const uint32 startSlot,
                                 Vector<SamplerState*>& samplerState,
                                 Textures& texture) {
    auto& sampler = reinterpret_cast<SamplerStateOGL&>(samplerState);
    auto& tex = reinterpret_cast<TexturesOGL&>(texture);

    glBindSampler(tex.m_texture, sampler.m_samplerState);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  void
  GraphicsApiOGL::setShaderResourceView(Textures* shaderResourceView,
                                       const uint32 startSlot,
                                       const uint32 numViews) {
    auto* resourceView = reinterpret_cast<TexturesOGL*>(shaderResourceView);

    glActiveTexture(GL_TEXTURE0 + startSlot);

    glBindTexture(GL_TEXTURE_2D, resourceView->m_texture);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      delete resourceView;
      exit(1);
    }
  }
  
  void
  GraphicsApiOGL::setRenderTarget(Textures* renderTarget,
                                 Textures* depthStencil) {
    if (nullptr != renderTarget) {
      auto* rTarget = reinterpret_cast<TexturesOGL*>(renderTarget);
      auto* dStencil = reinterpret_cast<TexturesOGL*>(depthStencil);

      glBindFramebuffer(GL_FRAMEBUFFER, rTarget->m_framebuffer);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8,
                                GL_RENDERBUFFER, 
                                dStencil->m_renderBufferObject);

      GLuint detectError = glGetError();

      if (detectError != 0) {
        exit(1);
      }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  
  void 
  GraphicsApiOGL::setDepthStencil(Textures& depthStencil,
                                 const uint32 stencilRef) {}
  
  void 
  GraphicsApiOGL::setInputLayout(InputLayout& vertexLayout) {
    auto& inputLayout = reinterpret_cast<InputLayoutOGL&>(vertexLayout);

    glBindVertexArray(inputLayout.m_inputLayout);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  void 
  GraphicsApiOGL::setViewport(const uint32 numViewports,
                             const uint32 width, 
                             const uint32 heigth) {
    glViewport(0, 0, width, heigth);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  void 
  GraphicsApiOGL::setPrimitiveTopology(const uint32 topology) {
    switch (topology) {
      case PRIMITIVE_TOPOLOGY::E::kPointList:
        m_topology = GL_POINTS;
        break;
      
      case PRIMITIVE_TOPOLOGY::E::kLineList:
        m_topology = GL_LINES;
        break;
      
      case PRIMITIVE_TOPOLOGY::E::kLineStrip:
        m_topology = GL_LINE_STRIP;
        break;
      
      case PRIMITIVE_TOPOLOGY::E::kTriangleList:
        m_topology = GL_TRIANGLES;
        break;
      
      case PRIMITIVE_TOPOLOGY::E::kTriangleStrip:
        m_topology = GL_TRIANGLE_STRIP;
        break;
    }

    GLuint detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  void 
  GraphicsApiOGL::setYourVS(Shaders& vertexShader) {}
  
  void
  GraphicsApiOGL::setYourVSConstantBuffers(ConstantBuffer* constantBuffer,
                                          const uint32 startSlot,
                                          const uint32 numBuffers) {
    auto* cBuffer = reinterpret_cast<ConstantBufferOGL*>(constantBuffer);

    glBindBufferBase(GL_UNIFORM_BUFFER, startSlot, 
                     cBuffer->m_uniformBufferObject);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      delete cBuffer;
      exit(1);
    }
  }
  
  void 
  GraphicsApiOGL::setYourPS(Shaders& pixelShader) {}
  
  void 
  GraphicsApiOGL::setYourPSConstantBuffers(ConstantBuffer* constantBuffer,
                                          const uint32 startSlot,
                                          const uint32 numBuffers) {
    auto* cBuffer = reinterpret_cast<ConstantBufferOGL*>(constantBuffer);

    glBindBufferBase(GL_UNIFORM_BUFFER, startSlot,
                     cBuffer->m_uniformBufferObject);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      delete cBuffer;
      exit(1);
    }
  }
  
  void
  GraphicsApiOGL::setYourPSSampler(SamplerState& sampler,
                                  const uint32 startSlot,
                                  const uint32 numSamplers) {}
  
  void
  GraphicsApiOGL::setShaders(Shaders& shaders) {
    auto& shader = reinterpret_cast<ShadersOGL&>(shaders);

    glUseProgram(shader.m_rendererID);

    GLuint detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }

  /***************************************************************************/
  /**
  * Gets.
  */
  /***************************************************************************/

  Textures*
  GraphicsApiOGL::getDefaultBackBuffer() { return nullptr; }

  Textures*
  GraphicsApiOGL::getDefaultDepthStencil() { return nullptr; }
}