#include "Renderable.h"
#include "../Math/Functions.h"

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
	Effect->SetDrawCallUniforms(localToWorldTransformation, worldToViewTransformation, viewToScreenTransformation);
	Mesh->Draw();
}


void eae6320::Graphics::Renderable::SetTransformations(eae6320::Math::cVector i_Position, eae6320::Math::cQuaternion i_Orientation, Math::cVector i_CameraPosition, Math::cQuaternion i_CameraOrientation, float i_AspectRatio)
{
	Offset = i_Position;
	localToWorldTransformation = eae6320::Math::cMatrix_transformation(i_Orientation, i_Position);
	worldToViewTransformation = eae6320::Math::cMatrix_transformation::CreateWorldToViewTransform(i_CameraOrientation, i_CameraPosition);
	viewToScreenTransformation = eae6320::Math::cMatrix_transformation::CreateViewToScreenTransform(eae6320::Math::ConvertDegreesToRadians(60.0f), i_AspectRatio, 0.1f, 100);
}