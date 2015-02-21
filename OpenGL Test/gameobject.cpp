
// Library Includes
#include <glm/gtc/matrix_transform.hpp>

// Local Includes

// This Include
#include "gameobject.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CGameObject constructor
*
* @author Christopher Howlett
*
*/
CGameObject::CGameObject()
:	m_pParent(0)
,	m_bHasParent(false)
{
	m_matWorld = glm::mat4(1.0f);
	m_vecPosition *= 0.0f;
	m_vecScale *= 0.0f;
	m_vecRotation *= 0.0f;
	
	m_vecForward = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vecRight = glm::vec3(1.0f, 0.0f, 0.0f);
	m_vecUp = glm::vec3(0.0f, 1.0f, 0.0f);

	m_vecParentOffset *= 0.0f;
}
/**
*
* CGameObject destructor
*
* @author Christopher Howlett
*
*/
CGameObject::~CGameObject()
{
	//Do nothing for this class
}
/**
*
* CGameObject Initialise
*
* @author Christopher Howlett
*
*/
bool 
CGameObject::Initialise()
{
	//Do nothing for this class
	return true;
}
/**
*
* CGameObject Draw
*
* @author Christopher Howlett
* @param _pRenderer OpenGL Renderer
* @param _pCamera Camera used for rendering
*
*/
void 
CGameObject::Draw(COpenGLRenderer* _pRenderer, CCamera* _pCamera)
{
	//Do nothing for this class
}
/**
*
* CGameObject Process
*
* @author Christopher Howlett
* @param _fDeltaTime Game Time elapsed
*
*/
void 
CGameObject::Process(float _fDeltaTime)
{
	m_matWorld = glm::translate(glm::mat4(1.0f), m_vecPosition);

	m_matWorld = glm::rotate(m_matWorld, m_vecRotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
	m_matWorld = glm::rotate(m_matWorld, m_vecRotation.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	m_matWorld = glm::rotate(m_matWorld, m_vecRotation.z, glm::vec3(0.0f, 1.0f, 0.0f));

	m_matWorld = glm::scale(m_matWorld, m_vecScale);
}
/**
*
* Processes Parent functionality on this gameobject
*
* @author Christopher Howlett
* @param Game time elapsed
*
*/
void 
CGameObject::ProcessParent(float _fDeltaTime)
{
	if(m_bHasParent)
	{
		//Move object relative to parent
		m_vecPosition = m_pParent->GetPosition();
		m_vecPosition += m_vecRight * m_vecParentOffset.x;
		m_vecPosition += m_vecUp * m_vecParentOffset.y;
		m_vecPosition += m_vecForward * m_vecParentOffset.z;
	}
}
/**
*
* Sets the parent object of this gameobject - Pass NULL or 0 to remove parent object
*
* @author Christopher Howlett
* @param Parent gameobject assigned
*
*/
void 
CGameObject::SetParent(CGameObject* _pParent, glm::vec3* _pTargetOffset)
{
	if(_pParent)
	{
		m_bHasParent = true;
		m_pParent = _pParent;
		m_vecParentOffset = *_pTargetOffset;
	}
	else
	{
		m_bHasParent = false;
		m_pParent = 0;
		m_vecParentOffset *= 0.0f;
	}
}
/**
*
* Sets CGameObject position
*
* @author Christopher Howlett
* @param position of gameobject
*
*/
void 
CGameObject::SetPosition(glm::vec3& _rVecPosition)
{
	m_vecPosition = _rVecPosition;
}
/**
*
* Returns CGameObject position
*
* @author Christopher Howlett
* @return Gameobject position
*
*/
glm::vec3& 
CGameObject::GetPosition()
{
	return m_vecPosition;
}
/**
*
* Sets GameObject rotation
*
* @author Christopher Howlett
* @param Gameobject rotation
*
*/
void 
CGameObject::SetRotation(glm::vec3& _rRotation)
{
	m_vecRotation = _rRotation;
}
/**
*
* Returns GameObject rotation
*
* @author Christopher Howlett
* @return Gameobject rotation
*
*/
glm::vec3& 
CGameObject::GetRotation()
{
	return m_vecRotation;
}
/**
*
* Sets GameObject scale
*
* @author Christopher Howlett
* @param Gameobject scale
*
*/
void 
CGameObject::SetScale(glm::vec3& _rScale)
{
	m_vecScale = _rScale;
}
/**
*
* Returns GameObject scale
*
* @author Christopher Howlett
* @return Gameobject scale
*
*/
glm::vec3& 
CGameObject::GetScale()
{
	return m_vecScale;
}
/**
*
* Sets GameObject Forward
*
* @author Christopher Howlett
* @param Gameobject Forward
*
*/
void 
CGameObject::SetForward(glm::vec3& _rForward)
{
	m_vecForward = _rForward;
	glm::normalize( m_vecForward );
}
/**
*
* Returns GameObject Forward
*
* @author Christopher Howlett
* @return Gameobject Forward
*
*/
glm::vec3& 
CGameObject::GetForward()
{
	glm::normalize( m_vecForward );
	return m_vecForward;
}
/**
*
* Sets GameObject Right
*
* @author Christopher Howlett
* @param Gameobject Forward
*
*/
void 
CGameObject::SetRight(glm::vec3& _rRight)
{
	m_vecRight = _rRight;
	glm::normalize( m_vecRight );
}
/**
*
* Returns GameObject Right
*
* @author Christopher Howlett
* @return Gameobject Right
*
*/
glm::vec3& 
CGameObject::GetRight()
{
	glm::normalize( m_vecRight );
	return m_vecRight;
}
/**
*
* Sets GameObject Up
*
* @author Christopher Howlett
* @param Gameobject Up
*
*/
void 
CGameObject::SetUp(glm::vec3& _rUp)
{
	m_vecUp = _rUp;
	glm::normalize( m_vecUp );
}
/**
*
* Returns GameObject Up
*
* @author Christopher Howlett
* @return Gameobject Up
*
*/
glm::vec3& 
CGameObject::GetUp()
{
	glm::normalize( m_vecUp );
	return m_vecUp;
}