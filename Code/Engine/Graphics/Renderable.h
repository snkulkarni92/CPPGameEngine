#ifndef _RENDERABLE_H
#define _RENDERABLE_H

#include "Mesh.h"
#include "Material.h"
#include "../Math/cVector.h"
#include "../Math/cMatrix_transformation.h"
#include "../Math/cQuaternion.h"
namespace eae6320
{
	namespace Graphics
	{
		class Renderable
		{
		public:
			Renderable();
			bool Initialize(const char * i_MeshPath, const char * i_EffectPath);
			void Draw();
			void ShutDown();

			void SetTransformations(Math::cVector i_Position, Math::cQuaternion i_Orientation, Math::cVector i_CameraPosition, Math::cQuaternion i_CameraOrientation, float AspectRatio);
		public:
			Mesh * Mesh;
			Material * Material;
			Math::cVector Offset;
			Math::cMatrix_transformation localToWorldTransformation;
			Math::cMatrix_transformation worldToViewTransformation;
			Math::cMatrix_transformation viewToScreenTransformation;
		};
	}
}
#endif //_RENDERABLE_H