#include "GameObject.h"

eae6320::Core::GameObject::GameObject()
{
	Renderable = new eae6320::Graphics::Renderable();
	Position.x = 0.0f;
	Position.y = 0.0f;
}

bool eae6320::Core::GameObject::Initialize(const char * i_MeshPath, const char * i_EffectPath)
{
	if (!this->Renderable->Initialize(i_MeshPath,i_EffectPath))
	{
		ShutDown();
		return false;
	}
	return true;
}

void eae6320::Core::GameObject::Update(Camera * i_Camera)
{
	Renderable->SetTransformations(Position, Orientation, i_Camera->Position, i_Camera->Orientation, i_Camera->AspectRatio);
}

void eae6320::Core::GameObject::ShutDown()
{
	if (Renderable)
	{
		Renderable->ShutDown();
		delete Renderable;
	}
}