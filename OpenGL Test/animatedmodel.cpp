//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   CAnimatedModel.cpp
//  Description :   Code for Class CAnimatedModel
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <fstream>
#include <rapidxml_utils.hpp>

// Local Includes
#include "openglrenderer.h"
#include "texture.h"
#include "shader.h"

// This Include
#include "AnimatedModel.h"

// Static Variables

// Static Function Prototypes

// Implementation
using namespace std;
/**
*
* CAnimatedModel constructor
*
* @author Christopher Howlett
*
*/
AnimatedModel::AnimatedModel()
{
	
}
/**
*
* CAnimatedModel Destructor
*
* @author Christopher Howlett
*
*/
AnimatedModel::~AnimatedModel()
{
	
}

void
AnimatedModel::LoadFromCollada( char* _pcFilename )
{
	//Open file containing resource information
	rapidxml::file<> xmlFile( _pcFilename );
	rapidxml::xml_document<> xmlDoc;

	//Parse file string
	xmlDoc.parse<0>( xmlFile.data( ) );
	rapidxml::xml_node<>* pResourceNode = xmlDoc.first_node( "resources" );
}