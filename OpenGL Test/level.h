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
#include "gameobject.h"
#include "camera.h"
#include "resourcemanager.h"
#include "shader.h"

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
	void Draw( );

	virtual const Camera* GetPerspectiveCamera( ) const { return &m_pPerspectiveCamera; }
	virtual const Camera* GetOrthographiCamera( ) const { return &m_pOrthographiCamera; }

protected:

private:
	Level(const Level& _kr);
	Level& operator= (const Level& _rHS);

    // Member Variables
protected:
	ResourceManager m_resourceManager;
	Shader pShader;
	//Cameras
	Camera m_pPerspectiveCamera;
	Camera m_pOrthographiCamera;
	bool m_bIsFirstPerson;

	GameObject testobject;
};

#endif // __Level_H__
