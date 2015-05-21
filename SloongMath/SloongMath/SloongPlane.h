#pragma once
#include "SloongVector.h"
#include "SloongParmLine.h"
namespace Sloong
{
	namespace Math
	{
		using namespace Vector;

		typedef class SLOONGMATH_API CPlane
		{
		public:
			CPlane();
			CPlane(float A, float B, float C, float D);
			CPlane(CVector3 &n, float D);
			virtual ~CPlane();

			void CreatePlaneFromTri(CVector3 &v1, CVector3 &v2,
				CVector3 &v3);

			void SetPlaneNormal(float A, float B, float C)
			{
				a = A; b = B; c = C;
			}
			void SetPlaneIntercept(float D) { d = D; }

			bool Intersect(CPlane &p2, CPlane &p3,
				CVector3 *intersectPoint);
			bool Intersect(CPlane &pl, CVector3 *intersectPoint);
			bool Intersect(CPlane &pl, CRay *intersect);
			bool Intersect(CVector3 &p1, CVector3 &p2, CVector3 &p3);

			int ClassifyPolygon(CPolygon &pol);
			int ClassifyPoint(CVector3 &v);
			int ClassifyPoint(float x, float y, float z);

			void Lerp(CPlane &p1, CPlane &p2, float amount);

			float GetDistance(CVector3 &v)
			{
				return a * v.x + b * v.y + c * v.z + d;
			}

			float GetDistance(float x, float y, float z)
			{
				return a * x + b * y + c * z + d;
			}

			float a, b, c, d;

		

			void Initialize(const POINT3D& p0,const VECTOR3D& normal, bool normalize);
			float ComputePointInPlane(POINT3D_PTR pt);
			INTERSECT_TYPE Intersect_Parm_Line3D_Plane3D(PARMLINE3D_PTR pline,float *t, POINT3D_PTR pt);
		public:
			POINT3D p0; // point on the plane
			VECTOR3D n; // normal to the plane (not necessarily a unit vector)

		}PLANE3D, *PLANE3D_PTR;
	}
}
