//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   window.cpp
//  Description :   Code for Class window app
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes
#include "defines.h"
#include "clock.h"
#include "openglrenderer.h"

// This Include
#include "window.h"

// Static Variables

// Static Function Prototypes

// Implementation
#define WM_WINDOW_CREATION (WM_USER + 1)
/**
*
* Main window message handler proc
*
* @author Christopher Howlett
*
*/
LRESULT CALLBACK
WindowProc(HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	static Window* pThisWindow = 0;

	switch( _uiMsg )
	{
		case WM_CREATE:
			{
			
			}
			break;
		case WM_WINDOW_CREATION:
			{
				pThisWindow = (Window*)_wParam;
			}
			break;
		case WM_KEYDOWN:
			{
				switch(LOWORD(_wParam))
				{
				case VK_ESCAPE:
					{
						PostQuitMessage(0);
					}
					break;
				}
			}
			break;
		case WM_QUIT: //Fall through
		case WM_CLOSE://Fall through
		case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			break;
		default:
			break;
	}

	// Don't start processing messages until after WM_CREATE.
	if( pThisWindow )
	{
		return pThisWindow->msgProc(_uiMsg, _wParam, _lParam);
	}
	else
	{
		return DefWindowProc(_hWnd, _uiMsg, _wParam, _lParam);
	}
}
/**
*
* CWindow Constructor
*
* @author Christopher Howlett
*
*/
Window::Window()
:	m_pRenderer(0)
,	m_hMainWnd(0)
,	m_pClock(0)
{
	m_iWindowWidth    = static_cast<int>(WINDOW_WIDTH);
	m_iWindowHeight   = static_cast<int>(WINDOW_HEIGHT);
}
/**
*
* CWindow Destructor
*
* @author Christopher Howlett
*
*/
Window::~Window()
{
	if(m_pRenderer)
	{
		delete m_pRenderer;
		m_pRenderer = 0;
	}
	if(m_pClock)
	{
		delete m_pClock;
		m_pClock = 0;
	}
}
/**
*
* CWindow GetInstance
*
* @author Christopher Howlett
* @return Returns this window instance
*
*/
HINSTANCE 
Window::GetInstance()
{
	return m_hInstance;
}
/**
*
* CWindow GetWindowHandle
*
* @author Christopher Howlett
* @return Returns this window handle
*
*/
HWND 
Window::GetHandle()
{
	return m_hMainWnd;
}
/**
*
* CWindow initialisation
*
* @author Christopher Howlett
* @param _hInstance Handle to this instance
* @param _eRenderer DirectX or OpenGL Renderer
* @return Returns success of window creation
*
*/
bool
Window::Initialise(HINSTANCE _hInstance, ERendererType _eRenderer)
{
	//Setup game clock
	m_pClock = new Clock();
	m_pClock->Initialise();
	m_pClock->LimitFramesPerSecond(60.0f);

	
	ZeroMemory(&m_tInput, sizeof(TInputStruct));
	m_hInstance = _hInstance;
	InitialiseMainWindow(L"OpenGL Window", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, false, true);
	SendMessage(m_hMainWnd, WM_WINDOW_CREATION, (WPARAM)this, NULL);


	//Create renderer type specified
	switch (_eRenderer)
	{
	case RENDERER_OPENGL:
	{
		m_pRenderer = new OpenGLRenderer();
	}
		break;
	case RENDERER_DIRECTX:
	{

	}
		break;
	};

	m_pRenderer->Initialise( this, m_iWindowWidth, m_iWindowHeight, &m_tInput);

	return true;
}


/**
*
* CWindow Initialises Main Window
*
* @author Christopher Howlett
*
*/
bool
Window::InitialiseMainWindow(wchar_t* _pTitle, int _iX, int _iY, int _iWidth, int _iHeight, bool _bFullscreen, bool _bVSync)
{
	bool bResult = false;
	m_bIsFullscreen = _bFullscreen;

	HWND hWnd;
	WNDCLASSEX wc;
	m_hInstance = GetModuleHandle(NULL);

	//Setup window class
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _pTitle;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	//Create temporary window for extension setup
	//hWnd = CreateWindowEx(WS_EX_APPWINDOW,
	//	_pTitle, _pTitle,
	//	WS_POPUP,
	//	0, 0,
	//	640, 480,
	//	NULL, NULL,
	//	m_hInstance, NULL);
	//ErrAssert(hWnd, L"Could not create temporary window");
	//
	//ShowWindow(hWnd, SW_HIDE);

	///Destroy temporary window
	//DestroyWindow(hWnd);
	//hWnd = NULL;

	//Set window styles
	int winStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	if (_bFullscreen)
	{
		//Fullscreen settings
		DEVMODE dScreenSettings;
		memset(&dScreenSettings, 0, sizeof(DEVMODE));
		dScreenSettings.dmSize = sizeof(DEVMODE);
		dScreenSettings.dmPelsWidth = static_cast<unsigned long>(iScreenWidth);
		dScreenSettings.dmPelsHeight = static_cast<unsigned long>(iScreenHeight);
		dScreenSettings.dmBitsPerPel = 32;
		dScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dScreenSettings, CDS_FULLSCREEN);
	}
	else
	{
		iScreenWidth = _iWidth;
		iScreenHeight = _iHeight;
	}

	//Create window
	hWnd = CreateWindowEx(WS_EX_APPWINDOW, _pTitle, _pTitle,
		winStyle,
		_iX, _iY,
		iScreenWidth, iScreenHeight,
		NULL, NULL,
		m_hInstance, NULL);
	ErrAssert(hWnd, L"Window creation failed");

	//Show window
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	m_hMainWnd = hWnd;

	return bResult;
}
/**
*
* CWindow Run - Runs infinite loop for program
*
* @author Christopher Howlett
*
*/
void 
Window::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
 
	while(msg.message != WM_QUIT)
	{
		// Process windows messages
		if(PeekMessage( &msg, 0, 0U, 0U, PM_REMOVE ))
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}
		// Run the game
		else
        {	
			ExecuteOneFrame();
        }
    }
}
/**
*
* CWindow Executes one frame of program
*
* @author Christopher Howlett
*
*/
void 
Window::ExecuteOneFrame()
{
	m_pClock->Process();
	float fTimeElapsed = m_pClock->GetDeltaTick();
	m_pRenderer->SetFPSCount(m_pClock->GetFPS());
	m_pRenderer->ExecuteOneFrame(fTimeElapsed);
}
/**
*
* CWindow Class Message proc, receives messages from window proc
*
* @author Christopher Howlett
*
*/
LRESULT 
Window::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	// ======================================================= 
	//				MOUSE CONTROLS
	// =======================================================  
	case WM_MOUSEMOVE:
		{
			glm::vec3 vecThisMouse(static_cast<float>(LOWORD(lParam) - (m_iWindowWidth * 0.5f)), static_cast<float>(-(HIWORD(lParam) - (m_iWindowHeight * 0.25f))), 0.0f);
			m_tInput.vecMouseDir = vecThisMouse - glm::vec3(m_tInput.fMouseX, m_tInput.fMouseY, 0.0f);
			m_tInput.fMouseX = vecThisMouse.x;
			m_tInput.fMouseY = vecThisMouse.y;
		}
		break;
	case WM_LBUTTONDOWN:
		{
			m_tInput.bLeftMouseDown = true;
		}
		break;
	case WM_LBUTTONUP:
		{
			m_tInput.bLeftMouseDown = false;
		}
		break;
	case WM_RBUTTONDOWN:
		{
			m_tInput.bRightMouseDown = true;
		}
		break;
	case WM_RBUTTONUP:
		{
			m_tInput.bRightMouseDown = false;
		}
		break;
	// ======================================================= 
	//				KEYBOARD CONTROLS
	// =======================================================  
	case WM_KEYDOWN:
		{
			switch(LOWORD(wParam))
			{
			case VK_UP:
				{
					m_tInput.bUp = true;
				}
				break;
			case VK_DOWN:
				{
					m_tInput.bDown = true;
				}
				break;
			case VK_LEFT:
				{
					m_tInput.bLeft = true;
				}
				break;
			case VK_RIGHT:
				{
					m_tInput.bRight = true;
				}
				break;
			case 'W':
				{
					m_tInput.bW = true;
				}
				break;
			case 'A':
				{
					m_tInput.bA = true;
				}
				break;
			case 'S':
				{
					m_tInput.bS = true;
				}
				break;
			case 'D':
				{
					m_tInput.bD = true;
				}
				break;
			case 'R':
				{
					m_tInput.bReset = true;
				}
				break;
			case 'E':
				{
					m_tInput.bMoveRods = true;
				}
				break;
			case VK_SHIFT:
				{
					m_tInput.bShift = true;
				}
				break;
			case VK_MENU:
				{
					m_tInput.bAlt = true;
				}
				break;
			case VK_SPACE:
				{
					m_tInput.bSpace = true;
				}
				break;
			case VK_CONTROL:
				{
					m_tInput.bCtrl = true;
				}
				break;
			case '1':
				{
					m_tInput.b1 = true;
				}
				break;
			case '2':
				{
					m_tInput.b2 = true;
				}
				break;
			case '3':
				{
					m_tInput.b3 = true;
				}
				break;
			case VK_OEM_3: //Tilde key
				{
					m_tInput.bTilde = true;
				}
				break;
			case VK_ADD:
				{
					m_tInput.bPlus = true;
				}
				break;
			case VK_SUBTRACT:
				{
					m_tInput.bMinus = true;
				}
				break;
			default:
				break;
			}
		}
		break;
	case WM_KEYUP:
		{
			switch(LOWORD(wParam))
			{
			case VK_UP:
				{
					m_tInput.bUp = false;
				}
				break;
			case VK_DOWN:
				{
					m_tInput.bDown = false;
				}
				break;
			case VK_LEFT:
				{
					m_tInput.bLeft = false;
				}
				break;
			case VK_RIGHT:
				{
					m_tInput.bRight = false;
				}
				break;
			case 'W':
				{
					m_tInput.bW = false;
				}
				break;
			case 'A':
				{
					m_tInput.bA = false;
				}
				break;
			case 'S':
				{
					m_tInput.bS = false;
				}
				break;
			case 'D':
				{
					m_tInput.bD = false;
				}
				break;
			case 'R':
				{
					m_tInput.bReset = false;
				}
				break;
			case 'E':
				{
					m_tInput.bMoveRods = false;
				}
				break;
			case VK_SHIFT:
				{
					m_tInput.bShift = false;
				}
				break;
			case VK_SPACE:
				{
					m_tInput.bSpace = false;
				}
				break;
			case VK_CONTROL:
				{
					m_tInput.bCtrl = false;
				}
				break;
			case VK_MENU:
				{
					m_tInput.bAlt = false;
				}
				break;
			case '1':
				{
					m_tInput.b1 = false;
				}
				break;
			case '2':
				{
					m_tInput.b2 = false;
				}
				break;
			case '3':
				{
					m_tInput.b3 = false;
				}
				break;
			case VK_OEM_3:
				{
					m_tInput.bTilde = false;
				}
				break;
			case VK_ADD:
				{
					m_tInput.bPlus = false;
				}
				break;
			case VK_SUBTRACT:
				{
					m_tInput.bMinus = false;
				}
				break;
			default:
				break;
			}
		}
		break;
	}
	return DefWindowProc(m_hMainWnd, msg, wParam, lParam);
}