

#include <glm/gtc/matrix_transform.hpp>
#include "transformcomponent.h"

namespace Component
{
	Transform::Transform( )
	{

	}

	Transform::~Transform( )
	{

	}

	bool
	Transform::Initialise( GameObject* _pOwner )
	{
		return true;
	}

	void
	Transform::Process( float _fDeltaTime )
	{
			m_matWorld = glm::translate( glm::mat4( 1.0f ), m_vecPosition );

			m_matWorld = glm::rotate( m_matWorld, m_vecRotation.x, glm::vec3( 0.0f, 1.0f, 0.0f ) );
			m_matWorld = glm::rotate( m_matWorld, m_vecRotation.y, glm::vec3( -1.0f, 0.0f, 0.0f ) );
			m_matWorld = glm::rotate( m_matWorld, m_vecRotation.z, glm::vec3( 0.0f, 1.0f, 0.0f ) );

			m_matWorld = glm::scale( m_matWorld, m_vecScale );
		}

	void
	Transform::AddChild( GameObject* _pChild )
	{
		m_vecChildren.push_back( _pChild );
	}

	void
	Transform::SetParent( GameObject* _pParent )
	{
		m_pParent = _pParent;
	}

	/**
	*
	* Sets CTransform position
	*
	* @author Christopher Howlett
	* @param position of Transform
	*
	*/
	void 
	Transform::SetPosition(glm::vec3& _rVecPosition)
	{
		m_vecPosition = _rVecPosition;
	}
	/**
	*
	* Returns CTransform position
	*
	* @author Christopher Howlett
	* @return Transform position
	*
	*/
	glm::vec3& 
	Transform::GetPosition()
	{
		return m_vecPosition;
	}
	/**
	*
	* Sets Transform rotation
	*
	* @author Christopher Howlett
	* @param Transform rotation
	*
	*/
	void 
	Transform::SetRotation(glm::vec3& _rRotation)
	{
		m_vecRotation = _rRotation;
	}
	/**
	*
	* Returns Transform rotation
	*
	* @author Christopher Howlett
	* @return Transform rotation
	*
	*/
	glm::vec3& 
	Transform::GetRotation()
	{
		return m_vecRotation;
	}
	/**
	*
	* Sets Transform scale
	*
	* @author Christopher Howlett
	* @param Transform scale
	*
	*/
	void 
	Transform::SetScale(glm::vec3& _rScale)
	{
		m_vecScale = _rScale;
	}
	/**
	*
	* Returns Transform scale
	*
	* @author Christopher Howlett
	* @return Transform scale
	*
	*/
	glm::vec3& 
	Transform::GetScale()
	{
		return m_vecScale;
	}
	/**
	*
	* Sets Transform Forward
	*
	* @author Christopher Howlett
	* @param Transform Forward
	*
	*/
	void 
	Transform::SetForward(glm::vec3& _rForward)
	{
		m_vecForward = _rForward;
		glm::normalize( m_vecForward );
	}
	/**
	*
	* Returns Transform Forward
	*
	* @author Christopher Howlett
	* @return Transform Forward
	*
	*/
	glm::vec3& 
	Transform::GetForward()
	{
		glm::normalize( m_vecForward );
		return m_vecForward;
	}
	/**
	*
	* Sets Transform Right
	*
	* @author Christopher Howlett
	* @param Transform Forward
	*
	*/
	void 
	Transform::SetRight(glm::vec3& _rRight)
	{
		m_vecRight = _rRight;
		glm::normalize( m_vecRight );
	}
	/**
	*
	* Returns Transform Right
	*
	* @author Christopher Howlett
	* @return Transform Right
	*
	*/
	glm::vec3& 
	Transform::GetRight()
	{
		glm::normalize( m_vecRight );
		return m_vecRight;
	}
	/**
	*
	* Sets Transform Up
	*
	* @author Christopher Howlett
	* @param Transform Up
	*
	*/
	void 
	Transform::SetUp(glm::vec3& _rUp)
	{
		m_vecUp = _rUp;
		glm::normalize( m_vecUp );
	}
	/**
	*
	* Returns Transform Up
	*
	* @author Christopher Howlett
	* @return Transform Up
	*
	*/
	glm::vec3& 
	Transform::GetUp()
	{
		glm::normalize( m_vecUp );
		return m_vecUp;
	}

}//end namespace