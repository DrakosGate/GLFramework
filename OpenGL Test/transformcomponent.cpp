

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "transformcomponent.h"

namespace Component
{
	Transform::Transform( )
	{
		SetPosition( glm::vec3( 0.1f, 0.1f, 0.1f ) );
		SetScale( glm::vec3( 1.0f, 1.0f, 1.0f ) ); 
		SetRotation( glm::vec3( 0.0f, 0.0f, 0.0f ) );

		SetForward( glm::vec3( 0.0f, 0.0f, 1.0f ) );
		SetRight( glm::vec3( 1.0f, 0.0f, 0.0f ) );
		SetUp( glm::vec3( 0.0f, 1.0f, 0.0f ) );
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
			glm::mat4 matTranslate = glm::translate( glm::mat4( 1.0f ), VecConvert( m_vecPosition ) );

			glm::vec3 convertedRotation = VecConvert( m_vecRotation );
			glm::mat4 matRotate = glm::rotate( glm::mat4( 1.0f ), convertedRotation.x, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			matRotate = glm::rotate( matRotate, convertedRotation.y, glm::vec3( 0.0f, 1.0f, 0.0f ) );
			matRotate = glm::rotate( matRotate, convertedRotation.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );

			//glm::quat tempRot = glm::toQuat( );
			//glm::mat4 matRotate = glm::toMat4( normalize( tempRot ) );

			glm::mat4 matScale = glm::scale( glm::mat4( 1.0f ), m_vecScale );
			
			m_matWorld = matTranslate * matRotate * matScale;
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

	const glm::mat4& 
	Transform::GetMatrix( ) const
	{
		return m_matWorld;
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