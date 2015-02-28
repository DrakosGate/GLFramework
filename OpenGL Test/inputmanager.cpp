


#include "inputmanager.h"
#include "inputlistener.h"

InputManager* InputManager::s_pInputManager = 0;

InputManager* 
InputManager::GetInstance( )
{
	if ( s_pInputManager == nullptr )
	{
		s_pInputManager = new InputManager();
	}
	return s_pInputManager;
}
void
InputManager::Delete( )
{
	delete s_pInputManager;
}

InputManager::~InputManager( )
{
	m_vecListeners.clear( );
}

void
InputManager::ProcessInput( HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam )
{
	switch ( _uiMsg )
	{
	case WM_KEYDOWN:
	{
					   for ( auto iter = m_vecListeners.begin( ); iter != m_vecListeners.end( ); ++iter )
					   {
						   ( *iter )->OnKeyDown( LOWORD( _wParam ), 0.1f );
					   }
					   break;
	}
	default:
	break;
	}
					   
}

void
InputManager::AddListener( Listeners::InputListener* _pNewListener )
{
	m_vecListeners.push_back( _pNewListener );
}

void
InputManager::RemoveListener( Listeners::InputListener* _pListener )
{
	for ( auto iter = m_vecListeners.begin( ); iter != m_vecListeners.end( ); ++iter )
	{
		if ( ( *iter ) == _pListener )
		{
			m_vecListeners.erase( iter );
			break;
		}
	}
}


InputManager::InputManager( )
{
	
}
