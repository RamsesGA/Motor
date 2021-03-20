#include <tchar.h>

#include "gaPrerequisitesCore.h"
#include "gaGraphicsApi.h"
#include "gaCamera.h"
#include "gaModels.h"

#if GA_PLATFORM_WIN32
#include <windows.h>
#endif

///
/// V A R I A B L E S
/// G L O B A L E S
///

///The main window class name.
static TCHAR g_szWindowClass[] = _T("DesktopApp");
///The string that appears in the application's title bar.
static TCHAR g_szTitle[] = _T("Windows - KriegerFS-");
///Tamaño de pantalla
const unsigned int g_width = 1000;
const unsigned int g_height = 800;

glm::vec4 g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);

///
/// M O D E L
/// 

CModel* g_model = nullptr;

///
/// C A M E R A
/// 

glm::mat4x4 g_world;
CCamera g_mainCamera;

///
/// G R A P H I C
/// A P I
/// 

//CGraphicApi* g_pGraphicApi = new CGraphicApiDX();
CGraphicApi* g_pGraphicApi = new CGraphicApiOGL();

CTexture* g_pRenderTargetView = nullptr;
CTexture* g_pDepthStencil = nullptr;

CInputLayout* g_pVertexLayout = nullptr;

CShaders* g_pBothShaders = nullptr;

CVertexBuffer* g_pVertexBuffer = nullptr;

CIndexBuffer* g_pIndexBuffer = nullptr;

CConstantBuffer* g_pConstantBuffer1 = nullptr;
CConstantBuffer* g_pConstantBuffer2 = nullptr;

///
/// S T R U C T´s
/// 

struct ConstantBuffer1 {

  glm::mat4x4 mView;
  glm::mat4x4 mProjection;
};

struct ConstantBuffer2 {

  glm::mat4x4 mWorld;
  glm::vec4 vMeshColor;
};

///
/// F U N C I O N E S
///

/// <summary>
/// Manejar los mensajes que la aplicación 
/// recibe de Windows cuando ocurren eventos.
/// </summary>
/// <param name="_hWnd"></param>
/// <param name="_message"></param>
/// <param name="_wParam"></param>
/// <param name="_lParam"></param>
/// <returns></returns>
LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message,
  WPARAM _wParam, LPARAM _lParam) {

  PAINTSTRUCT ps;
  HDC hdc;
  POINT Temp;

  switch (_message) {

  case WM_PAINT:
    hdc = BeginPaint(_hWnd, &ps);
    EndPaint(_hWnd, &ps);
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

    /// WM_KEYDOWN: A window receives keyboard 
    /// input in the form of keystroke messages and character messages.
  case WM_KEYDOWN: {
    g_mainCamera.InputDetection(_wParam);

    ConstantBuffer1 cb;
    cb.mView = g_mainCamera.GetView();

    g_pGraphicApi->UpdateConstantBuffer
    (&cb, *g_pConstantBuffer1);
    break;
  }

                 /// WM_LBUTTONDOWN: Posted when the user presses 
                 /// the left mouse button while the cursor is in the client area of a window.
  case WM_LBUTTONDOWN: {
    GetCursorPos(&Temp);
    g_mainCamera.SetOriginalMousePos(Temp.x, Temp.y);
    g_mainCamera.SetClickPressed(true);
    break;
  }

  case WM_MOUSEMOVE: {
    if (g_mainCamera.GetClickPressed()) {

      g_mainCamera.SetOriginalMousePos
      (g_mainCamera.GetOriginalMousePos().x, g_mainCamera.GetOriginalMousePos().y);

      g_mainCamera.MouseRotation();

      ConstantBuffer1 cb;
      cb.mView = g_mainCamera.GetView();
      g_pGraphicApi->UpdateConstantBuffer
      (&cb, *g_pConstantBuffer1);
    }
    break;
  }

                   /// WM_LBUTTONUP: Posted when the user releases 
                   /// the left mouse button while the cursor is in the client area of a window.
  case WM_LBUTTONUP:
    g_mainCamera.SetClickPressed(false);
    break;

  default:
    return DefWindowProc(_hWnd, _message, _wParam, _lParam);
    break;
  }

  return 0;
}

/// <summary>
/// Guardamos información de la ventana
/// del typedef "WNDCLASSEX"
/// </summary>
/// <param name="_wcex"></param>
/// <param name="_hInstance"></param>
void SetWndClassEx(WNDCLASSEX& _wcex, _In_ HINSTANCE& _hInstance) {

  _wcex.cbSize = sizeof(WNDCLASSEX);
  _wcex.style = CS_HREDRAW | CS_VREDRAW;
  _wcex.lpfnWndProc = WndProc;
  _wcex.cbClsExtra = 0;
  _wcex.cbWndExtra = 0;
  _wcex.hInstance = _hInstance;
  _wcex.hIcon = LoadIcon(_hInstance, IDI_APPLICATION);
  _wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  _wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  _wcex.lpszMenuName = NULL;
  _wcex.lpszClassName = g_szWindowClass;
  _wcex.hIconSm = LoadIcon(_wcex.hInstance, IDI_APPLICATION);
}

/// <summary>
/// Creación de la ventana
/// </summary>
/// <returns></returns>
HWND& CreateNewWindow() {

  HWND hWnd = CreateWindowEx(
    0,
    ///g_szWindowClass: the name of the application
    g_szWindowClass,
    ///g_szTitle: the text that appears in the title bar
    g_szTitle,
    // WS_OVERLAPPEDWINDOW: the type of window to create
    WS_OVERLAPPEDWINDOW,
    ///CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    CW_USEDEFAULT, CW_USEDEFAULT,
    ///initial size (width, height)
    g_width, g_height,
    ///NULL: the parent of this window
    nullptr,
    ///NULL: this application does not have a menu bar
    nullptr,
    // _hInstance: the first parameter from WinMain
    ::GetModuleHandle(nullptr),
    ///NULL: not used in this application
    nullptr
  );

  return hWnd;
}

/// <summary>
/// Función para inicializaro los
/// valores principales de la cámara
/// </summary>
void InitCamera(bool _isOGL = false) {

  ///Inicializamos la matriz de identidad
  g_world = glm::mat4(1.0f);

  CameraDescriptor mainCamera;
  mainCamera.s_lookAt = glm::vec3(0.0f, 1.0f, 0.0f);
  mainCamera.s_eye = glm::vec3(0.0f, 3.0f, -6.0f);
  mainCamera.s_up = glm::vec3(0.0f, 1.0f, 0.0f);
  mainCamera.s_far = 1000.0f;
  mainCamera.s_near = 0.01f;
  mainCamera.s_foV = 0.78539816339f;
  mainCamera.s_height = g_height;
  mainCamera.s_width = g_width;

  g_mainCamera.Init(mainCamera, _isOGL);
}

/// <summary>
/// Función para almacenar todas las funciones
/// con descripción de "UPDATE"
/// </summary>
void Update() {

  ConstantBuffer1 meshData;

  meshData.mProjection = g_mainCamera.GetProjection();
  meshData.mView = g_mainCamera.GetView();

  g_pGraphicApi->UpdateConstantBuffer
  (&meshData, *g_pConstantBuffer1);

  ConstantBuffer2 cb;
  cb.mWorld = g_world;
  cb.vMeshColor = g_vMeshColor;

  g_pGraphicApi->UpdateConstantBuffer(&cb, *g_pConstantBuffer2);
}

/// <summary>
/// Función para guardar valores y generar
/// información para la pantalla
/// </summary>
void Render() {

  ///Guardamos los render targets
  g_pGraphicApi->SetRenderTarget(g_pRenderTargetView, g_pDepthStencil);

  ///Guardamos un viewport
  g_pGraphicApi->SetViewport(1, g_width, g_height);

  ///
  /// Clear the back buffer
  ///
  g_pGraphicApi->ClearYourRenderTargetView(g_pRenderTargetView,
    0.0f, 156.0f, 140.0f, 1.6f);

  ///
  /// Clear the depth buffer to 1.0 (max depth)
  ///
  g_pGraphicApi->ClearYourDepthStencilView(g_pDepthStencil);

  ///Guardamos el input layout
  g_pGraphicApi->SetInputLayout(*g_pVertexLayout);
  ///Guardamos el vertex buffer
  g_pGraphicApi->SetVertexBuffer(*g_pVertexBuffer);
  ///Guardamos el index buffer
  g_pGraphicApi->SetIndexBuffer(*g_pIndexBuffer);
  ///Guardamos la topología
  g_pGraphicApi->SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  g_pGraphicApi->SetShaders(*g_pBothShaders);
  g_pGraphicApi->SetYourVSConstantBuffers(g_pConstantBuffer1, 0, 1);
  g_pGraphicApi->SetYourVSConstantBuffers(g_pConstantBuffer2, 1, 1);
  g_pGraphicApi->SetYourPSConstantBuffers(g_pConstantBuffer2, 1, 1);

  g_model->Draw(g_pGraphicApi);

  ///
  /// Present our back buffer to our front buffer
  ///

  g_pGraphicApi->SwapChainPresent(0, 0);
}

/// <summary>
/// Función donde generamos el proyecto
/// </summary>
/// <param name="_hWnd"></param>
void CreateProject(HWND _hWnd) {

  ///Mandamos la ventana a la API
  g_pGraphicApi->InitDevice(_hWnd);

  InitCamera();

  ///
  /// C R E A T E´s
  ///

  ///Creamos el render target view
  g_pRenderTargetView = g_pGraphicApi->GetDefaultBackBuffer();

  ///Creamos el depth stencil view
  g_pDepthStencil = g_pGraphicApi->GetDefaultDepthStencil();

  ///Creamos el vertex shader y pixel shader
  g_pBothShaders = g_pGraphicApi->CreateShadersProgram(L"DX_CubeShader.fx", "VS",
    L"DX_CubeShader.fx", "PS");
  if (nullptr == g_pBothShaders) {

    g_pBothShaders = g_pGraphicApi->CreateShadersProgram(L"OGL_VertexShader.txt", "main",
      L"OGL_PixelShader.txt", "main");
    if (nullptr == g_pBothShaders) {

      exit(1);
    }

    InitCamera(true);
  }

  ///Creamos el input layout
  g_pVertexLayout = g_pGraphicApi->CreateInputLayout(*g_pBothShaders);
  if (nullptr == g_pVertexLayout) {

    exit(1);
  }

  ///Creamos el vertex buffer
  g_pVertexBuffer = g_pGraphicApi->CreateVertexBuffer(nullptr, sizeof(Matrices));
  if (nullptr == g_pVertexBuffer) {

    exit(1);
  }

  ///Creamos el index buffer
  g_pIndexBuffer = g_pGraphicApi->CreateIndexBuffer(nullptr, sizeof(ViewCB));
  if (nullptr == g_pIndexBuffer) {

    exit(1);
  }

  ///Creamos los constant buffers para el shader
  g_pConstantBuffer1 = g_pGraphicApi->CreateConstantBuffer(sizeof(ConstantBuffer1));
  if (nullptr == g_pConstantBuffer1) {

    exit(1);
  }

  g_pConstantBuffer2 = g_pGraphicApi->CreateConstantBuffer(sizeof(ConstantBuffer2));
  if (nullptr == g_pConstantBuffer2) {

    exit(1);
  }

  g_model = new CModel();
  //g_model->Init("Models/POD/OBJ/POD.obj", g_pGraphicApi);
  //g_model->Init("Models/ugandan/FBX/Knuckles.fbx", g_pGraphicApi);
  //g_model->Init("Models/sonic/FBX/sonic.fbx", g_pGraphicApi);
  g_model->Init("Models/Nier2b/OBJ/Nier2b.obj", g_pGraphicApi);
}

/// <summary>
/// Función para eliminar los punteros
/// declarados en la aplicación
/// </summary>
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

///
/// F U N C I Ó N
/// P R I N C I P A L
/// 
int CALLBACK WinMain(
  _In_ HINSTANCE _hInstance,
  _In_opt_ HINSTANCE _hPrevInstance,
  _In_ LPSTR     _lpCmdLine,
  _In_ int       _nCmdShow) {

  WNDCLASSEX wcex;

  ///Guardamos la información principal
  /// para la ventana
  SetWndClassEx(wcex, _hInstance);

  ///Posible error
  if (!RegisterClassEx(&wcex)) {

    MessageBox(NULL,
      _T("Call to RegisterClassEx failed!"),
      _T("Windows Desktop Guided Tour"),
      NULL);

    return 1;
  }

  ///Creamos la ventana
  HWND hWnd = CreateNewWindow();

  ///Posible error
  if (!hWnd) {

    MessageBox(NULL,
      _T("Call to CreateWindow failed!"),
      _T("Windows Desktop Guided Tour"),
      NULL);

    return 1;
  }

  ///hWnd: the value returned from CreateWindow
  ShowWindow(hWnd, SW_SHOW);

  ///Función para mandar a llamar
  /// la generación del proyecto
  CreateProject(hWnd);

  ///Main message loop:
  MSG msg = { 0 };
  while (WM_QUIT != msg.message) {

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    Update();

    Render();
  }

  ///Mandar a llamar todos los deletes
  /// del lado de la aplicación
  Delete();

  return (int)msg.wParam;
}