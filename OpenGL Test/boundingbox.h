//
//  File Name   :   boundingbox.h
//  Description :   Class of CBoundingBox
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __BOUNDINGBOX_H__
#define __BOUNDINGBOX_H__

// Library Includes

// Local Includes

// Types
struct TBoundingBox
{
	TBoundingBox()
	{
		fLeft = 0.0f;
		fRight = 0.0f;
		fTop = 0.0f;
		fBottom = 0.0f;
	}
	float fLeft;
	float fRight;
	float fTop;
	float fBottom;
};

// Constants

// Prototypes
struct TVertexType;

class CBoundingBox
{
public:
	CBoundingBox();
	virtual ~CBoundingBox();

	virtual bool Initialise( TVertexType* _pObjectVertices, int _iVertexCount );
	virtual float GetRadius() const;
	TBoundingBox* GetRect();

private:
	CBoundingBox(const CBoundingBox& _krInstanceToCopy);
	const CBoundingBox& operator =(const CBoundingBox& _krInstanceToCopy);

private:
	glm::vec3 m_vecPosition;
	glm::vec3 m_vecScale;
	float m_fRadius;
	TBoundingBox m_tBoundingBox;

};

#endif // __BOUNDINGBOX_H__