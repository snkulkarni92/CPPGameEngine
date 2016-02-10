#include "Renderable.h"
#include "../Math/Functions.h"

eae6320::Graphics::Renderable::Renderable()
{
	Mesh = new eae6320::Graphics::Mesh();
	Material = new eae6320::Graphics::Material();
}

bool eae6320::Graphics::Renderable::Initialize(const char * i_MeshPath, const char * i_MaterialPath)
{
	void * buffer;
	buffer = this->Mesh->LoadMesh(i_MeshPath);
	if (!this->Mesh->Initialize(buffer))
	{
		ShutDown();
		return false;
	}
	Material->Load(i_MaterialPath);
	return true;
}

void eae6320::Graphics::Renderable::ShutDown()
{
	if (Material)
	{
		Material->ShutDown();
		delete Material;
	}
	if (Mesh)
	{
		Mesh->ShutDown();
		delete Mesh;
	}
}

void eae6320::Graphics::Renderable::Draw()
{
	Material->Bind();
	Material->SetUniforms();
	Material->m_Effect->SetDrawCallUniforms(localToWorldTransformation, worldToViewTransformation, viewToScreenTransformation, cameraPos);
	Mesh->Draw();
}


void eae6320::Graphics::Renderable::SetTransformations(eae6320::Math::cVector i_Position, eae6320::Math::cQuaternion i_Orientation, Math::cVector i_CameraPosition, Math::cQuaternion i_CameraOrientation, float i_AspectRatio)
{
	Offset = i_Position;
	localToWorldTransformation = eae6320::Math::cMatrix_transformation(i_Orientation, i_Position);
	worldToViewTransformation = eae6320::Math::cMatrix_transformation::CreateWorldToViewTransform(i_CameraOrientation, i_CameraPosition);
	viewToScreenTransformation = eae6320::Math::cMatrix_transformation::CreateViewToScreenTransform(eae6320::Math::ConvertDegreesToRadians(60.0f), i_AspectRatio, 0.1f, 10000);
	cameraPos[0] = i_CameraPosition.x;
	cameraPos[1] = i_CameraPosition.y;
	cameraPos[2] = i_CameraPosition.z;
}