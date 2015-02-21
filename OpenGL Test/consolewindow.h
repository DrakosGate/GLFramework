//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   consolewindow.h
//  Description :   Declaration of CConsoleWindow class
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __CONSOLEWINDOW_H__
#define __CONSOLEWINDOW_H__

// Library Includes

// Local Includes

// Types

// Constants

// Prototypes

class ConsoleWindow
{
//Member functions
public:
	ConsoleWindow();
	~ConsoleWindow();

	void InitialiseConsole();
	void ShutdownConsole();
	
private:
	ConsoleWindow(const ConsoleWindow& _kr);
	ConsoleWindow& operator= (const ConsoleWindow& _rhs);

//Member variables
private:

};

#endif //__CONSOLEWINDOW_H__
