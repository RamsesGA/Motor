#include <tchar.h>
#include <gaVector4.h>
#include <gaMatrix4x4.h>

#include "gaPrerequisitesCore.h"
#include "gaGraphicsApi.h"
#include "gaCamera.h"
#include "gaModels.h"

#if GA_PLATFORM_WIN32
#include <windows.h>
#endif

namespace gaEngineSDK {
  /***************************************************************************/
  /**
  * Global variables
  */
  /***************************************************************************/

  //The main window class name.
  static TCHAR g_szWindowClass[] = _T("DesktopApp");
  //The string that appears in the application's title bar.
  static TCHAR g_szTitle[] = _T("Windows - KriegerFS-");
  //Tamaño de pantalla
  const unsigned int g_width = 1000;
  const unsigned int g_height = 800;
  
  Vector4 g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);
 
  /***************************************************************************/
  /**
  * Model
  */
  /***************************************************************************/
  
  Model* g_model = nullptr;
  
  /***************************************************************************/
  /**
  * Camera
  */
  /***************************************************************************/
  //Matrix4x4 g_world;
  //Camera g_mainCamera;
  
  /***************************************************************************/
  /**
  * Graphic Api
  */
  /***************************************************************************/
  //GraphicsApi* g_pGraphicApi = new GraphicApiDX();
  //GraphicsApi* g_pGraphicApi = new GraphicApiOGL();
  GraphicsApi* g_pGraphicApi;
  
  Textures* g_pRenderTargetView = nullptr;
  Textures* g_pDepthStencil = nullptr;
  
  InputLayout* g_pVertexLayout = nullptr;
  
  Shaders* g_pBothShaders = nullptr;
  
  VertexBuffer* g_pVertexBuffer = nullptr;
  
  IndexBuffer* g_pIndexBuffer = nullptr;
  
  ConstantBuffer* g_pConstantBuffer1 = nullptr;
  ConstantBuffer* g_pConstantBuffer2 = nullptr;
  
  /***************************************************************************/
  /**
  * Structs
  */
  /***************************************************************************/
  struct ConstantBuffer1 {
    Matrix4x4 mView;
    Matrix4x4 mProjection;
  };
  
  struct ConstantBuffer2 {
    Matrix4x4 mWorld;
    Vector4 vMeshColor;
  };
  
  /***************************************************************************/
  /**
  * Functions
  */
  /***************************************************************************/
  
  /**
  * @brief Handle the messages that the application 
  *        receives from Windows when events occur.
  */
  LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
                           WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;
    POINT Temp;
  
    switch (message) {
  
      case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
      
      case WM_DESTROY:
        PostQuitMessage(0);
        break;
      
        // WM_KEYDOWN: A window receives keyboard 
        // input in the form of keystroke messages and character messages.
      case WM_KEYDOWN: {
        //g_mainCamera.inputDetection(wParam);
      
        /*ConstantBuffer1 cb;
        cb.mView = g_mainCamera.getView();
      
        g_pGraphicApi->updateConstantBuffer(&cb, *g_pConstantBuffer1);*/
        break;
      }
      
      // WM_LBUTTONDOWN: Posted when the user presses 
      // the left mouse button while the cursor is in the client area of a window.
      case WM_LBUTTONDOWN: {
        GetCursorPos(&Temp);

        //g_mainCamera.setOriginalMousePos(Temp.x, Temp.y);
        //g_mainCamera.setClickPressed(true);
        break;
      }
      
      case WM_MOUSEMOVE: {
        //if (g_mainCamera.getClickPressed()) {
      
          //g_mainCamera.setOriginalMousePos(g_mainCamera.getOriginalMousePos().x, 
          //                                 g_mainCamera.getOriginalMousePos().y);
      
          //g_mainCamera.mouseRotation();
      
          /*ConstantBuffer1 cb;
          cb.mView = g_mainCamera.getView();
          g_pGraphicApi->updateConstantBuffer(&cb, *g_pConstantBuffer1);*/
        //}
        break;
      }
      
      // WM_LBUTTONUP: Posted when the user releases 
      // the left mouse button while the cursor is in the client area of a window.
      case WM_LBUTTONUP:
        //g_mainCamera.setClickPressed(false);
        break;
      
      default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
  
    return 0;
  }
  
  /**
  * @brief We save information from the typedef window "WNDCLASSEX".
  */
  void SetWndClassEx(WNDCLASSEX& wcex, _In_ HINSTANCE& hInstance) {
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = g_szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
  }
  
  /**
  * @brief Window creation.
  */
  HWND& CreateNewWindow() {
    HWND hWnd = CreateWindowEx(
      0,
      //g_szWindowClass: the name of the application
      g_szWindowClass,
      //g_szTitle: the text that appears in the title bar
      g_szTitle,
      // WS_OVERLAPPEDWINDOW: the type of window to create
      WS_OVERLAPPEDWINDOW,
      //CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
      CW_USEDEFAULT, CW_USEDEFAULT,
      //initial size (width, height)
      g_width, g_height,
      //NULL: the parent of this window
      nullptr,
      //NULL: this application does not have a menu bar
      nullptr,
      // _hInstance: the first parameter from WinMain
      ::GetModuleHandle(nullptr),
      //NULL: not used in this application
      nullptr
    );
  
    return hWnd;
  }
  
  /**
  * @brief Function to initialize the main values of the camera.
  */
  void InitCamera(bool _isOGL = false) {
    ////Inicializamos la matriz de identidad
    //g_world = glm::mat4(1.0f);
  
    //CameraDescriptor mainCamera;
    //mainCamera.camLookAt = Vector3(0.0f, 1.0f, 0.0f);
    //mainCamera.camEye = Vector3(0.0f, 3.0f, -6.0f);
    //mainCamera.camUp = Vector3(0.0f, 1.0f, 0.0f);
    //mainCamera.camFar = 1000.0f;
    //mainCamera.camNear = 0.01f;
    //mainCamera.camFoV = 0.78539816339f;
    //mainCamera.camHeight = g_height;
    //mainCamera.camWidth = g_width;
  
    //g_mainCamera.init(mainCamera, _isOGL);
  }
  
  /**
  * @brief Function to store all functions with description of "UPDATE".
  */
  void Update() {
  
    /*ConstantBuffer1 meshData;
  
    meshData.mProjection = g_mainCamera.getProjection();
    meshData.mView = g_mainCamera.getView();
  
    g_pGraphicApi->updateConstantBuffer
    (&meshData, *g_pConstantBuffer1);
  
    ConstantBuffer2 cb;
    cb.mWorld = g_world;
    cb.vMeshColor = g_vMeshColor;
  
    g_pGraphicApi->updateConstantBuffer(&cb, *g_pConstantBuffer2);*/
  }
  
  /**
  * @brief Function to save values and generate information for the screen.
  */
  void Render() {
    //Guardamos los render targets
    g_pGraphicApi->setRenderTarget(g_pRenderTargetView, g_pDepthStencil);
  
    //Guardamos un viewport
    g_pGraphicApi->setViewport(1, g_width, g_height);
  
    //
    // Clear the back buffer
    //
    g_pGraphicApi->clearYourRenderTargetView(g_pRenderTargetView,
      0.0f, 156.0f, 140.0f, 1.6f);
  
    //
    // Clear the depth buffer to 1.0 (max depth)
    //
    g_pGraphicApi->clearYourDepthStencilView(g_pDepthStencil);
  
    //Guardamos el input layout
    g_pGraphicApi->setInputLayout(*g_pVertexLayout);
    //Guardamos el vertex buffer
    g_pGraphicApi->setVertexBuffer(*g_pVertexBuffer);
    //Guardamos el index buffer
    g_pGraphicApi->setIndexBuffer(*g_pIndexBuffer);
    //Guardamos la topología
    //g_pGraphicApi->setPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  
    g_pGraphicApi->setShaders(*g_pBothShaders);
    g_pGraphicApi->setYourVSConstantBuffers(g_pConstantBuffer1, 0, 1);
    g_pGraphicApi->setYourVSConstantBuffers(g_pConstantBuffer2, 1, 1);
    g_pGraphicApi->setYourPSConstantBuffers(g_pConstantBuffer2, 1, 1);
  
    //g_model->draw(g_pGraphicApi);
  
    //
    // Present our back buffer to our front buffer
    //
  
    g_pGraphicApi->swapChainPresent(0, 0);
  }
  
  // <summary>
  // Función donde generamos el proyecto
  // </summary>
  // <param name="_hWnd"></param>
  void CreateProject(HWND _hWnd) {
  
    //Mandamos la ventana a la API
    g_pGraphicApi->initDevice(_hWnd);
  
    InitCamera();
  
    //
    // C R E A T E´s
    //
  
    //Creamos el render target view
    g_pRenderTargetView = g_pGraphicApi->getDefaultBackBuffer();
  
    //Creamos el depth stencil view
    g_pDepthStencil = g_pGraphicApi->getDefaultDepthStencil();
  
    //Creamos el vertex shader y pixel shader
    g_pBothShaders = g_pGraphicApi->createShadersProgram(L"DX_CubeShader.fx", "VS",
                                                         L"DX_CubeShader.fx", "PS");
    if (nullptr == g_pBothShaders) {
  
      g_pBothShaders = g_pGraphicApi->createShadersProgram(L"OGL_VertexShader.txt", "main",
                                                           L"OGL_PixelShader.txt", "main");
      if (nullptr == g_pBothShaders) {
  
        exit(1);
      }
  
      InitCamera(true);
    }
  
    //Creamos el input layout
    g_pVertexLayout = g_pGraphicApi->createInputLayout(*g_pBothShaders);
    if (nullptr == g_pVertexLayout) {
  
      exit(1);
    }
  
    ////Creamos el vertex buffer
    //g_pVertexBuffer = g_pGraphicApi->createVertexBuffer(nullptr, sizeof(Matrices));
    //if (nullptr == g_pVertexBuffer) {
  
    //  exit(1);
    //}
  
    ////Creamos el index buffer
    //g_pIndexBuffer = g_pGraphicApi->createIndexBuffer(nullptr, sizeof(ViewCB));
    //if (nullptr == g_pIndexBuffer) {
  
    //  exit(1);
    //}
  
    //Creamos los constant buffers para el shader
    g_pConstantBuffer1 = g_pGraphicApi->createConstantBuffer(sizeof(ConstantBuffer1));
    if (nullptr == g_pConstantBuffer1) {
  
      exit(1);
    }
  
    g_pConstantBuffer2 = g_pGraphicApi->createConstantBuffer(sizeof(ConstantBuffer2));
    if (nullptr == g_pConstantBuffer2) {
  
      exit(1);
    }
  
    g_model = new Model();
    //g_model->Init("Models/POD/OBJ/POD.obj", g_pGraphicApi);
    //g_model->Init("Models/ugandan/FBX/Knuckles.fbx", g_pGraphicApi);
    //g_model->Init("Models/sonic/FBX/sonic.fbx", g_pGraphicApi);
    //g_model->init("Models/Nier2b/OBJ/Nier2b.obj", g_pGraphicApi);
  }
  
  // <summary>
  // Función para eliminar los punteros
  // declarados en la aplicación
  // </summary>
  void Delete() {
  
    delete g_model;
  
    delete g_pRenderTargetView;
  
    delete g_pDepthStencil;
  
    delete g_pVertexLayout;
  
    delete g_pBothShaders;
  
    delete g_pVertexBuffer;
  
    delete g_pIndexBuffer;
  
    delete g_pConstantBuffer1;
  
    delete g_pConstantBuffer2;
  
    delete g_pGraphicApi;
  }
  
  //
  // F U N C I Ó N
  // P R I N C I P A L
  // 
  int CALLBACK WinMain(_In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance,
                       _In_ LPSTR lpCmdLine,
                       _In_ int nCmdShow) {
    WNDCLASSEX wcex;
  
    //Guardamos la información principal
    // para la ventana
    SetWndClassEx(wcex, hInstance);
  
    //Posible error
    if (!RegisterClassEx(&wcex)) {
      MessageBox(NULL,  _T("Call to RegisterClassEx failed!"),
                 _T("Windows Desktop Guided Tour"),  NULL);
      return 1;
    }
  
    //Creamos la ventana
    HWND hWnd = CreateNewWindow();
  
    //Posible error
    if (!hWnd) {
  
      MessageBox(NULL, _T("Call to CreateWindow failed!"),
                 _T("Windows Desktop Guided Tour"),  NULL);
      return 1;
    }
  
    //hWnd: the value returned from CreateWindow
    ShowWindow(hWnd, SW_SHOW);
  
    //Función para mandar a llamar
    // la generación del proyecto
    CreateProject(hWnd);
  
    //Main message loop:
    MSG msg = { 0 };
    while (WM_QUIT != msg.message) {
  
      while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
  
      Update();
  
      Render();
    }
  
    //Mandar a llamar todos los deletes
    // del lado de la aplicación
    Delete();
  
    return (int)msg.wParam;
  }
}
