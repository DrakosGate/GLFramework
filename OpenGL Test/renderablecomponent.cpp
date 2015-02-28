

#include "openglrenderer.h"

#include "renderablecomponent.h"

namespace Component
{
	Renderable::Renderable( )
	{

	}

	Renderable::~Renderable( )
	{

	}

	bool
	Renderable::Initialise( GameObject* _pOwner, Model* _pModel, Texture* _pTexture, Shader* _pShader )
	{
		m_pModel = _pModel;
		m_pShader = _pShader;
		m_pTexture = _pTexture;

		BaseComponent::Initialise( _pOwner );
		return true;
	}
	void 
	Renderable::Process( float _fDeltaTime )
	{
		
	}
	void 
	Renderable::Draw( )
	{
		glBindVertexArray( m_pModel->GetVertexBuffer() );
		glDrawElements( GL_TRIANGLES, m_pModel->GetIndexCount(), GL_UNSIGNED_INT, 0 );
	}

}//end namespace