//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   CWindow.h
//  Description :   Class of Window App
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __WINDOW_H__
#define __WINDOW_H__

// Library Includes
#include <Windows.h>

// Local Includes

// Types
enum ERendererType
{
	RENDERER_INVALID = -1,
	RENDERER_OPENGL,
	RENDERER_DIRECTX,
	RENDERER_MAX
};

// Constants

// Prototypes
class IRenderer;
class Clock;

class Window
{
public:
	Window();
	virtual ~Window();

	virtual bool Initialise(HINSTANCE _hInstance, ERendererType _eRenderer);
	void Run();
	void ExecuteOneFrame();

	HINSTANCE GetInstance();
	HWND GetHandle();

	const unsigned int GetWindowWidth( ) const { return m_iWindowWidth; }
	const unsigned int GetWindowHeight( ) const { return m_iWindowHeight; }

	//This windows message proc
	virtual LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool InitialiseMainWindow( wchar_t* _pTitle, int _iX, int _iY, int _iWidth, int _iHeight, bool _bFullscreen, bool _bVSync );
private:
	Window(const Window& _kr);
	Window& operator= (const Window& _rhs);

	
protected:
	IRenderer* m_pRenderer;
	TInputStruct m_tInput;

	HINSTANCE m_hInstance;
	HWND m_hMainWnd;
	bool m_bIsFullscreen;

	Clock* m_pClock;

	unsigned int m_iWindowWidth;
	unsigned int m_iWindowHeight;
};

#endif //__WINDOW_H__