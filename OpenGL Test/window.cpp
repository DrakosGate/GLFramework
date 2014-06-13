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
	static CWindow* pThisWindow = 0;

	switch( _uiMsg )
	{
		case WM_CREATE:
			{
			
			}
			break;
		case WM_WINDOW_CREATION:
			{
				pThisWindow = (CWindow*)_wParam;
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
CWindow::CWindow()
:	m_pRenderer(0)
,	m_hMainWnd(0)
,	mMinimized(false)
,	mMaximized(false)
,	mResizing(false)	
,	m_pClock(0)
{
	m_iClientWidth    = static_cast<int>(WINDOW_WIDTH);
	m_iClientHeight   = static_cast<int>(WINDOW_HEIGHT);
}
/**
*
* CWindow Destructor
*
* @author Christopher Howlett
*
*/
CWindow::~CWindow()
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
CWindow::GetInstance()
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
CWindow::GetWindowHandle()
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
CWindow::Initialise(HINSTANCE _hInstance, ERendererType _eRenderer)
{
	//Setup game clock
	m_pClock = new CClock();
	m_pClock->Initialise();
	m_pClock->LimitFramesPerSecond(60.0f);

	ZeroMemory(&m_tInput, sizeof(TInputStruct));
	m_hInstance = _hInstance;
	InitialiseMainWindow();
	SendMessage(m_hMainWnd, WM_WINDOW_CREATION, (WPARAM)this, NULL);

	//Create renderer type specified
	switch(_eRenderer)
	{
	case RENDERER_OPENGL:
		{
			m_pRenderer = new COpenGLRenderer();
		}
		break;
	case RENDERER_DIRECTX:
		{

		}
		break;
	};

	m_pRenderer->Initialise(m_hMainWnd, m_iClientWidth, m_iClientHeight, &m_tInput);

	return true;
}
/**
*
* CWindow Run - Runs infinite loop for program
*
* @author Christopher Howlett
*
*/
void 
CWindow::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
 
	while(msg.message != WM_QUIT)
	{
		// Process windows messages
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
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
CWindow::ExecuteOneFrame()
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
CWindow::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	// ======================================================= 
	//				MOUSE CONTROLS
	// =======================================================  
	case WM_MOUSEMOVE:
		{
			TVector3 vecThisMouse(static_cast<float>(LOWORD(lParam) - (m_iClientWidth * 0.5f)), static_cast<float>(-(HIWORD(lParam) - (m_iClientHeight * 0.25f))), 0.0f);
			m_tInput.vecMouseDir = vecThisMouse - TVector3(m_tInput.fMouseX, m_tInput.fMouseY, 0.0f);
			m_tInput.fMouseX = vecThisMouse.fX;
			m_tInput.fMouseY = vecThisMouse.fY;
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
/**
*
* CWindow Initialises Main Window
*
* @author Christopher Howlett
*
*/
void 
CWindow::InitialiseMainWindow()
{
	wchar_t cClassName[] = L"OPENGL";
	WNDCLASS winClass;
	
	//Create window class
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	winClass.hCursor = LoadCursor(m_hInstance, IDC_ARROW);
	winClass.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
	winClass.hInstance = m_hInstance;
	winClass.lpfnWndProc = WindowProc;
	winClass.lpszClassName = cClassName;
	winClass.lpszMenuName = NULL;
	winClass.style = CS_VREDRAW | CS_HREDRAW;

	if(!RegisterClass(&winClass))
	{
		Error(L"Window creation FAILED"); 
	}

	//Initialise window
	m_hMainWnd = CreateWindow(	cClassName, 
								L"OpenGL Test", 
								WS_MINIMIZEBOX | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
								0,
								0,
								WINDOW_WIDTH,
								WINDOW_HEIGHT,
								NULL,
								NULL,
								m_hInstance, 
								NULL);
	if(!m_hMainWnd)
	{
		Error(L"Window creation failed2");
	}
}