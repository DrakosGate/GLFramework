

#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "component.h"
#include "model.h"
#include "texture.h"
#include "shader.h"

class GameObject;

namespace Component
{
	class Renderable : public BaseComponent
	{
	public:
		Renderable( );
		~Renderable( );

		virtual bool Initialise( GameObject* _pOwner, Model* _pModel, Texture* _pTexture, Shader* _pShader );
		virtual void Process( float _fDeltaTime );
		virtual void Draw( );

		const Model* GetModel( ) const { return m_pModel; }
		Shader* GetShader( ) const { return m_pShader; }
		const Texture* GetTexture( ) const { return m_pTexture; }

	protected:
		Model* m_pModel;
		Shader* m_pShader;
		Texture* m_pTexture;

	};
}