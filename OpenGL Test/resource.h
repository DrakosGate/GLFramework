//
//  File Name   :   Resource.h
//  Description :   Class of CResource
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __RESOURCE_H__
#define __RESOURCE_H__

// Library Includes
#include <string>

// Local Includes

// Types

// Constants

// Prototypes

class Resource
{
public:
	Resource( ){}
	virtual ~Resource( ){}

	const std::string& GetName( ) const { return m_sName; }
	void SetName( const std::string& _sName ) { m_sName = _sName; }

	const unsigned int GetResourceID( ) const { return m_uiResourceID; }
	void SetResourceID( const unsigned int _uiID ) { m_uiResourceID = _uiID; }

private:
	Resource(const Resource& _krInstanceToCopy);
	const Resource& operator =(const Resource& _krInstanceToCopy);
	
protected:
	std::string m_sName;
	unsigned int m_uiResourceID;
	
};

#endif //__Resource_H__