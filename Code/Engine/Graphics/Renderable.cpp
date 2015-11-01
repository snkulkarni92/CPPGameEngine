#include "Renderable.h"

eae6320::Graphics::Renderable::Renderable()
{
	Mesh = new eae6320::Graphics::Mesh();
	Effect = new eae6320::Graphics::Effect();
}

bool eae6320::Graphics::Renderable::Initialize(const char * i_MeshPath, const char * i_EffectPath)
{
	void * buffer;
	buffer = this->Mesh->LoadMesh(i_MeshPath);
	if (!this->Mesh->Initialize(buffer))
	{
		ShutDown();
		return false;
	}
	buffer = this->Effect->LoadEffect(i_EffectPath);
	if (!this->Effect->Initialize(buffer))
	{
		ShutDown();
		return false;
	}
	return true;
}

void eae6320::Graphics::Renderable::ShutDown()
{
	if (Effect)
	{
		Effect->ShutDown();
		delete Effect;
	}
	if (Mesh)
	{
		Mesh->ShutDown();
		delete Mesh;
	}
}

void eae6320::Graphics::Renderable::Draw()
{
	Effect->Bind();
	float floatArray[] = { 0.0f, 0.0f };
	floatArray[0] = Offset.x;
	floatArray[1] = Offset.y;
	Effect->SetDrawCallUniforms(floatArray);
	Mesh->Draw();
}


void eae6320::Graphics::Renderable::SetPositionOffset(eae6320::Math::cVector i_PositionOffset)
{
	Offset = i_PositionOffset;
}