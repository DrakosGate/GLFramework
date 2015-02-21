//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   irenderer.h
//  Description :   Class of IRenderer
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//
#ifndef __IRENDERER_H__
#define __IRENDERER_H__

#include "defines.h"

//Renderer interface
class Window;

class IRenderer
{
public:
	IRenderer(){};
	virtual ~IRenderer(){};

	virtual bool Initialise( Window* _pWindow, int _iWindowWidth, int _iWindowHeight, TInputStruct* _pInput){ return true; };
	virtual bool InitialiseExtensions( HWND _hWnd ){ return false; }
	virtual void CleanUp(){};
	virtual void ExecuteOneFrame(float _fDeltaTick){};
	virtual void SetFPSCount(int _iFramesPerSecond){};

private:
	IRenderer(const IRenderer& _kr);
	IRenderer& operator= (const IRenderer& _rhs);

protected:
	int m_iWindowWidth;
	int m_iWindowHeight;
};

#endif //IRENDERER_H__