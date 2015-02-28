//
//  File Name   :   model.h
//  Description :   Class of CModel
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

// Library Includes

// Local Includes
#include "model.h"

// Types

// Constants

// Prototypes

class AnimatedModel : public Model
{
public:
	AnimatedModel( );
	virtual ~AnimatedModel( );

	virtual void LoadFromCollada( char* _pcFilename );

private:
	AnimatedModel( const AnimatedModel& _krInstanceToCopy );
	const AnimatedModel& operator =( const AnimatedModel& _krInstanceToCopy );
	
protected:
	
	
};
