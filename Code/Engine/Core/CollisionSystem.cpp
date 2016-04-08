#include "CollisionSystem.h"

#include "../Graphics/DebugShape.h"

#include <cstdint>
#include <cstdio>
#include <sstream>

namespace eae6320
{
	namespace Core
	{
		namespace CollisionSystem
		{
			struct InputPoint
			{
				float x, y, z;
				uint8_t b, g, r, a;
				float u, v;
				float nx, ny, nz;
			};
			struct CollisionPoint
			{
				float x, y, z;
				float nx, ny, nz;
			};
			struct CollisionTriangle
			{
				CollisionPoint a, b, c;
			};

			CollisionTriangle * triangleList = NULL;
			uint32_t triangleCount = 0;

			void Initialize(const char * fileName)
			{
				FILE * iFile;
				void * buffer;
				size_t fileSize;

				uint32_t vertexCount, indexCount;
				InputPoint * vertexData;
				uint32_t * indexData;

				fopen_s(&iFile, fileName, "rb");
				if (iFile != NULL)
				{
					fseek(iFile, 0, SEEK_END);
					fileSize = ftell(iFile);
					rewind(iFile);

					buffer = (void *)malloc(fileSize);

					size_t result = fread(buffer, 1, fileSize, iFile);

					char * iPointer = reinterpret_cast<char *>(buffer);
					vertexCount = *reinterpret_cast<uint32_t *>(iPointer);
					iPointer += sizeof(vertexCount);
					indexCount = *reinterpret_cast<uint32_t *>(iPointer);
					iPointer += sizeof(indexCount);


					vertexData = reinterpret_cast<InputPoint *>(iPointer);
					iPointer += sizeof(InputPoint) * vertexCount;
					indexData = reinterpret_cast<uint32_t *>(iPointer);
					iPointer += sizeof(uint32_t) * indexCount;

					fclose(iFile);

					triangleCount = indexCount / 3;
					triangleList = new CollisionTriangle[triangleCount];

					for (uint32_t i = 0; i < triangleCount; i++)
					{
						triangleList[i].a.x = vertexData[indexData[i * 3]].x;
						triangleList[i].a.y = vertexData[indexData[i * 3]].y;
						triangleList[i].a.z = vertexData[indexData[i * 3]].z;
						triangleList[i].a.nx = vertexData[indexData[i * 3]].nx;
						triangleList[i].a.ny = vertexData[indexData[i * 3]].ny;
						triangleList[i].a.nz = vertexData[indexData[i * 3]].nz;

						triangleList[i].b.x = vertexData[indexData[i * 3 + 1]].x;
						triangleList[i].b.y = vertexData[indexData[i * 3 + 1]].y;
						triangleList[i].b.z = vertexData[indexData[i * 3 + 1]].z;
						triangleList[i].b.nx = vertexData[indexData[i * 3 + 1]].nx;
						triangleList[i].b.ny = vertexData[indexData[i * 3 + 1]].ny;
						triangleList[i].b.nz = vertexData[indexData[i * 3 + 1]].nz;

						triangleList[i].c.x = vertexData[indexData[i * 3 + 2]].x;
						triangleList[i].c.y = vertexData[indexData[i * 3 + 2]].y;
						triangleList[i].c.z = vertexData[indexData[i * 3 + 2]].z;
						triangleList[i].c.nx = vertexData[indexData[i * 3 + 2]].nx;
						triangleList[i].c.ny = vertexData[indexData[i * 3 + 2]].ny;
						triangleList[i].c.nz = vertexData[indexData[i * 3 + 2]].nz;
					}
				}
			}

			Math::cVector CheckCollision(Math::cVector& p, Math::cVector& q)
			{
				Graphics::DebugShapes::AddLine(p, q, Math::cVector(0, 255, 0));
				Math::cVector CollisionNormal;
				for (uint32_t i = 0; i < triangleCount; i++)
				{
					Math::cVector a(triangleList[i].a.x, triangleList[i].a.y, triangleList[i].a.z);
					Math::cVector b(triangleList[i].b.x, triangleList[i].b.y, triangleList[i].b.z);
					Math::cVector c(triangleList[i].c.x, triangleList[i].c.y, triangleList[i].c.z);

					Math::cVector ab = b - a;
					Math::cVector ac = c - a;
					Math::cVector pq = q - p;
					Math::cVector n(triangleList[i].a.nx, triangleList[i].a.ny, triangleList[i].a.nz);


					float d = -Math::Dot(pq, n);
					if (d <= 0.0f)
						continue;
					Math::cVector ap = p - a;
					float t = Math::Dot(ap, n);
					if (t < 0)
						continue;
					if (t > d)
						continue;

					Math::cVector w = ap + pq * t / d;

					float uu, uv, vv, wu, wv, D;

					uu = Math::Dot(ab, ab);
					uv = Math::Dot(ab, ac);
					vv = Math::Dot(ac, ac);
					wu = Math::Dot(w, ab);
					wv = Math::Dot(w, ac);

					D = uv * uv - uu * vv;

					float si, ti;
					si = (uv * wv - vv * wu) / D;
					if (si < 0.0 || si > 1.0)         // I is outside T
						continue;
					ti = (uv * wu - uu * wv) / D;
					if (ti < 0.0 || (si + ti) > 1.0)  // I is outside T
						continue;

					return n;

					////Math::cVector e = Math::Cross(qp, ap);

					

					//Graphics::DebugShapes::AddLine(a, p, Math::cVector(0, 128, 128));
					//Graphics::DebugShapes::AddLine(b, p, Math::cVector(0, 128, 128));
					//Graphics::DebugShapes::AddLine(c, p, Math::cVector(0, 128, 128));

					//Graphics::DebugShapes::AddLine(a, b, Math::cVector(0, 128, 0));
					//Graphics::DebugShapes::AddLine(a, c, Math::cVector(0, 128, 0));
					//Graphics::DebugShapes::AddLine(b, c, Math::cVector(0, 128, 0));

					//Graphics::DebugShapes::AddLine(a, a+w, Math::cVector(0, 0, 255));

					////Graphics::DebugShapes::AddLine(p, p + e, Math::cVector(0, 255, 0));


					///*float v = Math::Dot(ac, e);
					//if (v < 0.0f)
					//	continue;
					//float w = -Dot(ab, e);     
					//if (w < 0.0f)
					//	continue;*/
					////return n;
				}
				return CollisionNormal;
			}
		}
	}
}

