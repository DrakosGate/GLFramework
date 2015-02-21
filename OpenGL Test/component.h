

#pragma once


class GameObject;

class BaseComponent
{
public:
	BaseComponent( ){}
	~BaseComponent( ){}

	virtual bool Initialise( GameObject* _pOwner ) = 0;
	virtual void Process( float _fDeltaTime ) {}
	virtual const char* GetName( ) { return "BaseComponent"; }

protected:
	GameObject* m_pOwner;

};