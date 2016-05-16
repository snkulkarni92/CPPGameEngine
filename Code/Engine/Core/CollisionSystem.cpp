#include "CollisionSystem.h"

#include "../Graphics/DebugShape.h"

#include <cstdint>
#include <cstdio>
#include <sstream>
#include <vector>
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
			struct Quadrant
			{
				float Xp, Xn, Yp, Yn, Zp, Zn;
				Quadrant() {};
				Quadrant(float xp, float xn, float yp, float yn, float zp, float zn) : Xp(xp), Xn(xn), Yp(yp), Yn(yn), Zp(zp), Zn(zn) {};
			};
			struct OctreeNode
			{
				OctreeNode() {};
				bool hasChildren = true;
				OctreeNode* m_children[8] = { nullptr };
				std::vector<CollisionTriangle *> triangles;
				Quadrant m_bounds;
				Math::cVector m_center;
				Math::cVector m_halfLength;
				int depth;
			};
			struct DebugData
			{
				DebugData(Math::cVector i_center, Math::cVector i_extents, int d) : m_center(i_center), m_extents(i_extents), depth(d) {};
				Math::cVector m_center;
				Math::cVector m_extents;
				int depth;
			};
			OctreeNode * root;
			CollisionTriangle * triangleList = NULL;
			uint32_t triangleCount = 0;
			OctreeNode* BuildOctree(Quadrant i_bounds, int i_depth, std::vector<CollisionTriangle*> triangleList);
			void Initialize(const char * fileName)
			{
				FILE * iFile;
				void * buffer;
				size_t fileSize;

				uint32_t vertexCount, indexCount;
				InputPoint * vertexData;
				uint32_t * indexData;

				float minX, maxX, minY, maxY, minZ, maxZ;
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
					minX = minY = minZ = 99999;
					maxX = maxY = maxZ = -99999;
					for (uint32_t i = 0; i < vertexCount; i++)
					{
						if (vertexData[i].x < minX)
							minX = vertexData[i].x;
						if (vertexData[i].y < minY)
							minY = vertexData[i].y;
						if (vertexData[i].z < minZ)
							minZ = vertexData[i].z;
						if (vertexData[i].x > maxX)
							maxX = vertexData[i].x;
						if (vertexData[i].y > maxY)
							maxY = vertexData[i].y;
						if (vertexData[i].z > maxZ)
							maxZ = vertexData[i].z;
					}

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
				Quadrant rootQuad(minX, maxX, minY, maxY, minZ, maxZ);
				std::vector<CollisionTriangle *> triangleVector;
				for (int i = 0; i < triangleCount; i++)
					triangleVector.push_back(&triangleList[i]);
				root = BuildOctree(rootQuad, 4, triangleVector);
				//DrawTree();
			}
			void CalcCenterAndExtent(OctreeNode& i_node)
			{
				i_node.m_center = Math::cVector(((i_node.m_bounds.Xp + i_node.m_bounds.Xn) / 2), ((i_node.m_bounds.Yp + i_node.m_bounds.Yn) / 2), ((i_node.m_bounds.Zp + i_node.m_bounds.Zn) / 2));
				i_node.m_halfLength = Math::cVector(fabs(i_node.m_bounds.Xp - i_node.m_bounds.Xn) / 2, fabs(i_node.m_bounds.Yp - i_node.m_bounds.Yn) / 2, fabs(i_node.m_bounds.Zp - i_node.m_bounds.Zn) / 2);
			}

			void GenerateQuadrants(Math::cVector i_center, Math::cVector i_extents, Quadrant o_quads[])
			{
				o_quads[0] = Quadrant((i_center.x + i_extents.x), (i_center.x), (i_center.y + i_extents.y), (i_center.y), (i_center.z), (i_center.z - i_extents.z));
				o_quads[1] = Quadrant((i_center.x + i_extents.x), (i_center.x), (i_center.y + i_extents.y), (i_center.y), (i_center.z + i_extents.z), (i_center.z));
				o_quads[2] = Quadrant((i_center.x), (i_center.x - i_extents.x), (i_center.y + i_extents.y), (i_center.y), (i_center.z + i_extents.z), (i_center.z));
				o_quads[3] = Quadrant((i_center.x), (i_center.x - i_extents.x), (i_center.y + i_extents.y), (i_center.y), (i_center.z), (i_center.z - i_extents.z));
				o_quads[4] = Quadrant((i_center.x + i_extents.x), (i_center.x), (i_center.y), (i_center.y - i_extents.y), (i_center.z), (i_center.z - i_extents.z));
				o_quads[5] = Quadrant((i_center.x + i_extents.x), (i_center.x), (i_center.y), (i_center.y - i_extents.y), (i_center.z + i_extents.z), (i_center.z));
				o_quads[6] = Quadrant((i_center.x), (i_center.x - i_extents.x), (i_center.y), (i_center.y - i_extents.y), (i_center.z + i_extents.z), (i_center.z));
				o_quads[7] = Quadrant((i_center.x), (i_center.x - i_extents.x), (i_center.y), (i_center.y - i_extents.y), (i_center.z), (i_center.z - i_extents.z));
			}
			bool IsInQuad(const Quadrant& i_quad, const Math::cVector& i_point)
			{
				if ((i_quad.Xn <= i_point.x && i_point.x <= i_quad.Xp) && (i_quad.Yn <= i_point.y && i_point.y <= i_quad.Yp) && (i_quad.Zn <= i_point.z && i_point.z <= i_quad.Zp))
					return true;
				else
					return false;
			}
			OctreeNode* BuildOctree(Quadrant i_bounds, int i_depth, std::vector<CollisionTriangle*> triangleList)
			{
				//Initialize Node to return
				OctreeNode* node = new OctreeNode;
				node->m_bounds = i_bounds;
				CalcCenterAndExtent((*node));
				node->triangles = triangleList;
				node->depth = 4 - i_depth;
				//Check for Recursion end.
				if (i_depth == 0 || triangleList.size() < 5)
				{
					node->hasChildren = false;
					return node;
				}

				//Calculate the leaves.
				Quadrant m_quads[8];
				GenerateQuadrants(node->m_center, node->m_halfLength, m_quads);

				//Calling BuildOctree Recursively on each Quad
				{
					--i_depth;
					for (int i = 0; i < 8; ++i)
					{
						std::vector<CollisionTriangle *> childList;
						for (int j = 0; j < triangleList.size(); j++)
						{
							bool a, b, c;
							a = IsInQuad(m_quads[i], Math::cVector(triangleList[j]->a.x, triangleList[j]->a.y, triangleList[j]->a.z));
							b = IsInQuad(m_quads[i], Math::cVector(triangleList[j]->b.x, triangleList[j]->b.y, triangleList[j]->b.z));
							c = IsInQuad(m_quads[i], Math::cVector(triangleList[j]->c.x, triangleList[j]->c.y, triangleList[j]->c.z));
							if (a || b || c)
							{
								childList.push_back(triangleList[j]);
							}
						}
						node->m_children[i] = BuildOctree(m_quads[i], i_depth, childList);
					}
				}

				return node;
			}
			
			void DrawDataComplete(OctreeNode i_node)
			{
				Math::cVector colors[] = { Math::cVector(255,0,0), Math::cVector(0, 255, 0), Math::cVector(0, 0, 255), Math::cVector(255, 0, 255), Math::cVector(255, 255, 0) };
				
				Graphics::DebugShapes::AddBox(i_node.m_center, i_node.m_halfLength * 2 - Math::cVector(1, 1, 1) * i_node.depth * 10, colors[i_node.depth]);
				if (!(i_node.hasChildren))
				{
					return;
				}

				//Recurse down the tree.
				{
					for (int i = 0; i < 8; i++)
					{
						DrawDataComplete((*(i_node.m_children[i])));
					}
				}

				return;
			}
			void GetCurrentOctree(OctreeNode i_node, Math::cVector i_point)
			{
				Math::cVector colors[] = { Math::cVector(255,0,0), Math::cVector(0, 255, 0), Math::cVector(0, 0, 255), Math::cVector(255, 0, 255), Math::cVector(255, 255, 0) };

				Graphics::DebugShapes::AddBox(i_node.m_center, i_node.m_halfLength * 2, colors[i_node.depth]);
				if (!i_node.hasChildren)
				{
					return;
				}

				for (int i = 0; i < 8; i++)
				{
					if (IsInQuad(i_node.m_children[i]->m_bounds, i_point))
						GetCurrentOctree((*(i_node.m_children[i])), i_point);
					else
						continue;
				}
				return;
			}
			void DrawTree(Math::cVector point, Math::cVector dir)
			{
				Graphics::DebugShapes::AddLine(point, point + dir, Math::cVector(255, 0, 0));
				GetCurrentOctree(*root, point);
				GetCurrentOctree(*root, point - dir);
				//DrawDataComplete(*root);
			}
			CollidedPoint CheckCollision(Math::cVector& p, Math::cVector& q)
			{
				Math::cVector ZeroVector;
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
					Math::cVector pnt = p + pq * t / d;
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

					CollidedPoint cp;
					cp.isCollided = true;
					cp.Normal = n;
					cp.Point = pnt;
					return cp;


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
				CollidedPoint cp;
				cp.isCollided = false;
				cp.Normal = cp.Point = ZeroVector;
				return cp;
			}
		}
	}
}

