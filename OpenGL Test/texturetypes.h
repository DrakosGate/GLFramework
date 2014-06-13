//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
#pragma once

#ifndef __TEXTURETYPES_H__
#define __TEXTURETYPES_H__

struct TTargaTexture
{
	unsigned char data[12];
	unsigned short width;
	unsigned short height;
	unsigned char bpp;
	unsigned char data2;
};

#endif //__TEXTURETYPES_H__