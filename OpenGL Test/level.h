//
// (c) 2013 Media Design School
//
//  File Name   :   Level.h
//  Description :   Level Interface
//  Author      :   Chris Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __LEVEL_H__
#define __LEVEL_H__

// Library Includes
#include <Windows.h>

// Local Includes

// Types

// Constants

// Prototypes
class Window;
class OpenGLRenderer;
class Camera;

class Level
{
    // Member Functions
public:
	Level();
	~Level();
	bool Initialise( OpenGLRenderer* _pRenderer, Window* _pWindow );
	void Process( const float _fDeltaTime );


	virtual const Camera* GetPerspectiveCamera( ) const { return m_pPerspectiveCamera; }
	virtual const Camera* GetOrthographicCamera( ) const { return m_pOrthographicCamera; }

protected:

private:
	Level(const Level& _kr);
	Level& operator= (const Level& _rHS);

    // Member Variables
protected:
	//Cameras
	Camera* m_pPerspectiveCamera;
	Camera* m_pOrthographicCamera;
	bool m_bIsFirstPerson;

};

#endif // __Level_H__
