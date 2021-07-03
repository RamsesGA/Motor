#include <exception>
#include <gaMatrix4x4.h>

#include "gaGraphicsApiOGL.h"
#include "gaConstantBufferOGL.h"
#include "gaTexturesOGL.h"
#include "gaSamplerStateOGL.h"
#include "gaInputLayoutOGL.h"
#include "gaShadersOGL.h"
#include "gaVertexBufferOGL.h"
#include "gaIndexBufferOGL.h"
#include "stb_image.h"

namespace gaEngineSDK {

  String
  ReadShaderOGL(const WString& _nameVS) {
    IfStream shaderFile;

    shaderFile.exceptions(IfStream::failbit | IfStream::badbit);

    try {
      shaderFile.open(_nameVS);

      StrStream shaderStream;
      shaderStream << shaderFile.rdbuf();

      shaderFile.close();

      return shaderStream.str();
    }
    catch (IfStream::failure e) {
      std::cout << "ERROR, no se pudo leer el shader\n";
      return "";
    }
  }

  bool 
  AnalyzeVertexShaderOGL(const WString& _nameVS) {
    String bufferAnalyze;

    for (uint32 i = 0; i < _nameVS.size(); ++i) {

      bufferAnalyze += _nameVS[i];

      if (('_' == bufferAnalyze[i]) &&
          ("data/shaders/OGL_" != bufferAnalyze)) {
        return false;
      }
      else if (('_' == bufferAnalyze[i]) &&
               ("data/shaders/OGL_" == bufferAnalyze)) {
        return true;
      }
    }

    return false;
  }

  bool 
  AnalyzePixelShaderOGL(const WString& _namePS) {
    String bufferAnalyze;

    for (uint32 i = 0; i < _namePS.size(); ++i) {

      bufferAnalyze += _namePS[i];

      if (('_' == bufferAnalyze[i]) &&
          ("data/shaders/OGL_" != bufferAnalyze)) {
        return false;
      }
      else if (('_' == bufferAnalyze[i]) &&
               ("data/shaders/OGL_" == bufferAnalyze)) {
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
  GraphicsApiOGL::initDevice(sf::WindowHandle hWnd) {

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

    int32 pixelFormat = ChoosePixelFormat(m_HandleToDC, &pfd);

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

    setViewports(m_width, m_height);

    glEnable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    uint32 detectError = glGetError();

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

    uint32 detectError = glGetError();

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
  GraphicsApiOGL::loadTextureFromFile(String srcFile) {
    int32 width;
    int32 height;
    int32 components;
    unsigned char* data = stbi_load(srcFile.c_str(),
                                    &width, &height, &components, 0);
    if (!data) {
      return nullptr;
      stbi_image_free(data);
    }
    else {
      GLenum format = GL_ZERO;
      if (1 == components) {
        format = GL_RED;
      }
      else if (2 == components) {
        format = GL_RG;
      }
      else if (3 == components) {
        format = GL_RGB;
      }
      else if (4 == components) {
        format = GL_RGBA;
      }

      auto* texture = new TexturesOGL();

      glGenTextures(1, &texture->m_texture);
      glBindTexture(GL_TEXTURE_2D, texture->m_texture);

      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glBindTexture(GL_TEXTURE_2D, 0);

      stbi_image_free(data);

      return texture;
    }
  }
  
  void GraphicsApiOGL::unbindOGL() {
    glUseProgram(0);
  }

  Matrix4x4 
  GraphicsApiOGL::matrixPolicy(const Matrix4x4& mat4x4) {
    return mat4x4;
  }
  
  /***************************************************************************/
  /**
  * Updates.
  */
  /***************************************************************************/
  
  void 
  GraphicsApiOGL::updateConstantBuffer(const void* srcData,
                                       WeakSPtr<ConstantBuffer> updateDataCB) {
    if (nullptr == &updateDataCB) {
      throw new std::exception("Error, parametro de ConstanBuffer nulo OGL");
    }

    ConstantBufferOGL* UBO = reinterpret_cast<ConstantBufferOGL*>(updateDataCB.lock().get());

    glBindBuffer(GL_UNIFORM_BUFFER, UBO->m_uniformBufferObject);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, UBO->m_bufferSize, srcData);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    uint32 detectError = glGetError();

    if (detectError != 0) {
      throw new std::exception("Error, al actualizar el Constant Buffer OGL");
    }
  }
  
  /***************************************************************************/
  /**
  * Clears.
  */
  /***************************************************************************/
  
  void 
  GraphicsApiOGL::clearYourRenderTargetView(WeakSPtr<Textures> renderTarget, Vector4 rgba) {
    glClearColor(rgba.x, rgba.y, rgba.z, rgba.w);

    glClear(GL_COLOR_BUFFER_BIT);
  }
  
  void 
  GraphicsApiOGL::clearYourDepthStencilView(WeakSPtr<Textures> depthStencil) {
    glClear(GL_DEPTH_BUFFER_BIT);
  }
  
  /***************************************************************************/
  /**
  * Creates.
  */
  /***************************************************************************/
  
  Shaders* 
  GraphicsApiOGL::createShadersProgram(const WString& nameVS, const String& entryPointVS, 
                                       const WString& namePS, const String& entryPointPS) {
    if (!(AnalyzeVertexShaderOGL(nameVS))) {
      throw new std::exception("Error, no se encontro vertex shader OGL");
    }
    if (!(AnalyzePixelShaderOGL(namePS))) {
      throw new std::exception("Error, no se encontro pixel shader OGL");
    }

    String VS_ShaderSrc = ReadShaderOGL(nameVS);
    String PS_ShaderSrc = ReadShaderOGL(namePS);

    //Generamos el tipo de dato para ir guardando
    //los datos necesarios y entregarlo al usuario
    auto* shaders = new ShadersOGL();

    // Create an empty vertex shader handle
    uint32 vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that String's .c_str is NULL character terminated.
    const char* source = (const char*)VS_ShaderSrc.c_str();
    glShaderSource(vertexShader, 1, &source, nullptr);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    int32 isCompiled = 0;

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
      int32 maxLength = 0;
      glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      Vector<char> infoLog(maxLength);
      glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

      // We don't need the shader anymore.
      glDeleteShader(vertexShader);
      delete shaders;

      throw new std::exception("Error, isCompiled es falso OGL");
    }

    shaders->m_vertexShader = vertexShader;

    // Create an empty fragment shader handle
    uint32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that String's .c_str is NULL character terminated.
    source = (const char*)PS_ShaderSrc.c_str();
    glShaderSource(fragmentShader, 1, &source, nullptr);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
      int32 maxLength = 0;
      glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      Vector<char> infoLog(maxLength);
      glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

      // We don't need the shader anymore.
      glDeleteShader(fragmentShader);

      // Either of them. Don't leak shaders.
      glDeleteShader(vertexShader);
      delete shaders;

      throw new std::exception("Error, isCompiled es falso OGL");
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
    int32 isLinked = 0;
    glGetProgramiv(shaders->m_rendererID, GL_LINK_STATUS, (int32*)&isLinked);

    if (isLinked == GL_FALSE) {
      int32 maxLength = 0;
      glGetProgramiv(shaders->m_rendererID, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      Vector<char> infoLog(maxLength);
      glGetProgramInfoLog(shaders->m_rendererID, maxLength, &maxLength, &infoLog[0]);

      String caca;
      for (uint32 z = 0; z < infoLog.size(); ++z) {
        caca += infoLog[z];
      }

      // We don't need the program anymore.
      glDeleteProgram(shaders->m_rendererID);
      // Don't leak shaders either.
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);
      delete shaders;

      throw new std::exception("Error, isLinked es falso OGL");
    }

    shaders->m_fragmentShader = fragmentShader;

    // Always detach shaders after a successful link.
    glDetachShader(shaders->m_rendererID, vertexShader);
    glDetachShader(shaders->m_rendererID, fragmentShader);

    return shaders;
  }

  SPtr<VertexShader> 
  GraphicsApiOGL::loadVertexShaderFromFile(const char* vertexFilePath, 
                                           const char* vertexMainFuntion, 
                                           const char* shaderVersion) {
    return SPtr<VertexShader>();
  }

  SPtr<PixelShader> 
  GraphicsApiOGL::loadPixelShaderFromFile(const char* pixelFilePath, 
                                          const char* pixelMainFuntion, 
                                          const char* shaderVersion) {
    return SPtr<PixelShader>();
  }
  
  VertexBuffer* 
  GraphicsApiOGL::createVertexBuffer(const void* data, const uint32 size) {
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

    uint32 detectError = glGetError();

    if (detectError != 0) {
      delete VBO;
      throw new std::exception("Error, al crear el vertex buffer OGL");
    }

    return VBO;
  }
  
  IndexBuffer* 
  GraphicsApiOGL::createIndexBuffer(const void* data, const uint32 size) {
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

    uint32 detectError = glGetError();

    if (detectError != 0) {
      delete EBO;
      throw new std::exception("Error, al crear el index buffer OGL");
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

    uint32 detectError = glGetError();

    if (detectError != 0) {
      delete UBO;
      throw new std::exception("Error, al crear el constant buffer OGL");
    }

    return UBO;
  }
  
  Textures* 
  GraphicsApiOGL::createTexture(const uint32 width, const uint32 height,
                                const uint32 bindFlags, TEXTURE_FORMAT::E textureFormat,
                                const String fileName) {
    auto* tex = new TexturesOGL();

    uint32 texture;

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    //int32 width, height, nrChannels;

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

    uint32 detectError = glGetError();

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

    uint32 detectError = glGetError();

    if (detectError != 0) {
      delete samplerState;
      exit(1);
    }

    return samplerState;
  }
  
  InputLayout* 
  GraphicsApiOGL::createInputLayout(WeakSPtr<Shaders> vertexShader) {
    auto* inputLayout = new InputLayoutOGL();
    ShadersOGL* shader = reinterpret_cast<ShadersOGL*>(vertexShader.lock().get());

    if (nullptr == &shader) {
      throw new std::exception("Error, shader nulo OGL");
    }

    ///Creamos el vertex array
    glGenVertexArrays(1, &inputLayout->m_inputLayout);

    ///Ahora indicamos a OGL que lo vamos a usar
    glBindVertexArray(inputLayout->m_inputLayout);

    bool firstOffSet = true;
    uint32 offSet = 0;
    uint32 sizeComponent = 0;
    int32 total = -1;

    glGetProgramiv(shader->m_rendererID, GL_ACTIVE_ATTRIBUTES, &total);

    for (uint32 i = 0; i < total; ++i) {
      char name[100];
      memset(name, ' ', 100);

      int32 num = -1;
      int32 name_len = -1;

      GLenum type = GL_ZERO;

      glGetActiveAttrib(shader->m_rendererID, uint32(i),
                        sizeof(name) - 1, &name_len, &num,
                        &type, name);

      name[name_len] = 0;

      uint32 location = glGetAttribLocation(shader->m_rendererID, name);

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

        case GL_INT_VEC4:
          sizeComponent = 4;
          if (!firstOffSet) {
            offSet += 16;
          }
          break;
      }

      glVertexAttribFormat(location, sizeComponent, GL_FLOAT, false, offSet);

      uint32 detectError = glGetError();

      if (detectError != 0) {
        delete inputLayout;
        throw new std::exception("Error, al crear el input layout OGL");
      }

      glVertexAttribBinding(location, 0);
      glEnableVertexAttribArray(location);
    }

    uint32 detectError = glGetError();

    if (detectError != 0) {
      delete inputLayout;
      throw new std::exception("Error, al crear el input layout OGL");
    }

    return inputLayout;
  }
  
  /***************************************************************************/
  /**
  * Sets.
  */
  /***************************************************************************/
  
  void 
  GraphicsApiOGL::setPixelShader(WeakSPtr<Shaders> pixelShader) {
    ShadersOGL* shader = reinterpret_cast<ShadersOGL*>(pixelShader.lock().get());

    glUseProgram(shader->m_rendererID);

    uint32 detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  void 
  GraphicsApiOGL::setVertexShader(WeakSPtr<Shaders> vertexShader) {
    ShadersOGL* shader = reinterpret_cast<ShadersOGL*>(vertexShader.lock().get());

    glUseProgram(shader->m_rendererID);

    uint32 detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  void 
  GraphicsApiOGL::setVertexBuffer(WeakSPtr<VertexBuffer> vertexBuffer) {
    if (nullptr == &vertexBuffer) {
      throw new std::exception("Error, parametro nulo en set Vertex Buffer OGL");
    }

    VertexBufferOGL* vertex = reinterpret_cast<VertexBufferOGL*>(vertexBuffer.lock().get());

    glBindVertexBuffer(0, vertex->m_vertexBufferObject, 0, sizeof(Vertex));
  }
  
  void 
  GraphicsApiOGL::setIndexBuffer(WeakSPtr<IndexBuffer> indexBuffer) {
    if (nullptr == &indexBuffer) {
      throw new std::exception("Error, parametro nulo en set Index Buffer OGL");
    }

    IndexBufferOGL* index = reinterpret_cast<IndexBufferOGL*>(indexBuffer.lock().get());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index->m_indexBufferObject);
  }
  
  void 
  GraphicsApiOGL::setConstantBuffer(bool isVertex, WeakSPtr<ConstantBuffer> constantBuffer,
                                    const uint32 startSlot, const uint32 numBuffers) {
    ConstantBufferOGL* cBuffer =
                       reinterpret_cast<ConstantBufferOGL*>(constantBuffer.lock().get());

    glBindBuffer(GL_UNIFORM_BUFFER, cBuffer->m_uniformBufferObject);

    uint32 detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  void 
  GraphicsApiOGL::setSamplerState(WeakSPtr<SamplerState> sampler, WeakSPtr<Textures> texture, 
                                  uint32 startSlot, uint32 numSamplers) {
    if ((nullptr != sampler.lock().get()) && (nullptr != texture.lock().get())) {
      //auto& sampler = reinterpret_cast<SamplerStateOGL&>(samplerState);
      //auto& tex = reinterpret_cast<TexturesOGL&>(texture);
      //
      //glBindSampler(tex.m_texture, sampler.m_samplerState);
      //
      //uint32 detectError = glGetError();
      //
      //if (detectError != 0) {
      //  exit(1);
      //}
    }
  }

  void 
  GraphicsApiOGL::setSamplerVertexShader(WeakSPtr<SamplerState> sampler, 
                                         WeakSPtr<Textures> texture, 
                                         uint32 startSlot, uint32 numSamplers) {
  }

  void
  GraphicsApiOGL::setSamplerPixelShader(WeakSPtr<SamplerState> sampler, 
                                        WeakSPtr<Textures> texture, 
                                        uint32 startSlot, uint32 numSamplers) {
  }

  
  void
  GraphicsApiOGL::setShaderResourceView(WeakSPtr<Textures> shaderResourceView,
                                        const uint32 startSlot, const uint32 numViews) {
    TexturesOGL* resourceView = reinterpret_cast<TexturesOGL*>(shaderResourceView.lock().get());

    glActiveTexture(GL_TEXTURE0 + startSlot);

    glBindTexture(GL_TEXTURE_2D, resourceView->m_texture);

    uint32 detectError = glGetError();

    if (detectError != 0) {
      delete resourceView;
      exit(1);
    }
  }
  
  void
  GraphicsApiOGL::setRenderTarget(WeakSPtr<Textures> renderTarget,
                                  WeakSPtr<Textures> depthStencil) {
    if (nullptr != renderTarget.lock().get()) {
      TexturesOGL* rTarget = reinterpret_cast<TexturesOGL*>(renderTarget.lock().get());
      TexturesOGL* dStencil = reinterpret_cast<TexturesOGL*>(depthStencil.lock().get());

      glBindFramebuffer(GL_FRAMEBUFFER, rTarget->m_framebuffer);

      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8,
                                GL_RENDERBUFFER, 
                                dStencil->m_renderBufferObject);

      uint32 detectError = glGetError();

      if (detectError != 0) {
        throw new std::exception("Error, al guardar el Render Target OGL");
      }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  
  void 
  GraphicsApiOGL::setDepthStencil(WeakSPtr<Textures> depthStencil, const uint32 stencilRef) {}
  
  void 
  GraphicsApiOGL::setInputLayout(WeakSPtr<InputLayout> vertexLayout) {
    if (nullptr == &vertexLayout) {
      throw new std::exception("Error, parametro nulo en set Input Layout OGL");
    }

    InputLayoutOGL* inputLayout = reinterpret_cast<InputLayoutOGL*>(vertexLayout.lock().get());

    glBindVertexArray(inputLayout->m_inputLayout);
  }
  
  void 
  GraphicsApiOGL::setViewports(const uint32 width, const uint32 heigth, 
                               const uint32 numViewports) {
    glViewport(0, 0, width, heigth);
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

    uint32 detectError = glGetError();

    if (detectError != 0) {
      exit(1);
    }
  }
  
  void 
  GraphicsApiOGL::setYourVS(WeakSPtr<Shaders> vertexShader) {}
  
  void
  GraphicsApiOGL::setYourVSConstantBuffers(WeakSPtr<ConstantBuffer> constantBuffer,
                                           const uint32 startSlot, const uint32 numBuffers) {
    if (nullptr == constantBuffer.lock().get()) {
      throw new std::exception("Error, parametro nulo en set Your Const Buff OGL");
    }

    ConstantBufferOGL* cBuffer = 
                       reinterpret_cast<ConstantBufferOGL*>(constantBuffer.lock().get());

    glBindBufferBase(GL_UNIFORM_BUFFER, startSlot, cBuffer->m_uniformBufferObject);
  }
  
  void 
  GraphicsApiOGL::setYourPS(WeakSPtr<Shaders> pixelShader) {}
  
  void 
  GraphicsApiOGL::setYourPSConstantBuffers(WeakSPtr<ConstantBuffer> constantBuffer,
                                           const uint32 startSlot, const uint32 numBuffers) {
    if (nullptr == constantBuffer.lock().get()) {
      throw new std::exception("Error, parametro nulo en set Your Pixel Shader Const Buff OGL");
    }

    ConstantBufferOGL* cBuffer = 
                       reinterpret_cast<ConstantBufferOGL*>(constantBuffer.lock().get());

    glBindBufferBase(GL_UNIFORM_BUFFER, startSlot,
                     cBuffer->m_uniformBufferObject);
  }
  
  void
  GraphicsApiOGL::setYourPSSampler(WeakSPtr<SamplerState> sampler, const uint32 startSlot,
                                   const uint32 numSamplers) {}
  
  void
  GraphicsApiOGL::setShaders(WeakSPtr<Shaders> shaders) {
    if (nullptr == &shaders) {
      throw new std::exception("Error, parametro nulo en set Shaders OGL");
    }

    ShadersOGL* shader = reinterpret_cast<ShadersOGL*>(shaders.lock().get());

    glUseProgram(shader->m_rendererID);
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