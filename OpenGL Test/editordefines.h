//  File Name   :   editordefines.h
//  Description :   Definitions used for the Level Editor
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//


#ifndef __EDITORDEFINES_H__
#define __EDITORDEFINES_H__


// Library Includes
#include <sstream>

// Local Includes

// Types
#define SAFEDELETE(_object) { if(_object){ delete _object;_object = 0; } }

template<class T> T ReadFromString(std::string _sInput)
{
	std::stringstream sStream;
	sStream << _sInput;
	T result;
	sStream >> result;
	return result;
}



#endif//EDITORDEFINES_H__