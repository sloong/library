#include "stdafx.h"
#include "SloongPlane.h"

using namespace Sloong::Math;

void CPlane::Initialize(const POINT3D& p0,const VECTOR3D& normal, bool normalize)
{
	// this function initializes a 3d plane

	// copy the point
	this->p0 = p0;

	// if normalize is 1 then the normal is made into a unit vector
	if (!normalize)
		this->n = normal;
	else
	{
		// make normal into unit vector
		this->n.Normalize(normal);
	} // end else

}

float Sloong::Math::CPlane::ComputePointInPlane(POINT3D_PTR pt)
{
	// test if the point in on the plane, in the positive halfspace
	// or negative halfspace
	float hs = this->n.x*(pt->x - this->p0.x) +
		this->n.y*(pt->y - this->p0.y) +
		this->n.z*(pt->z - this->p0.z);

	// return half space value
	return(hs);
}

INTERSECT_TYPE Sloong::Math::CPlane::Intersect_Parm_Line3D_Plane3D(PARMLINE3D_PTR pline, float *t, POINT3D_PTR pt)
{
	// this function determines where the sent parametric line 
	// intersects the plane the function will project the line 
	// infinitely in both directions, to compute the intersection, 
	// but the line segment defined by p intersected the plane iff t e [0,1]
	// also the function returns 0 for no intersection, 1 for 
	// intersection of the segment and the plane and 2 for intersection 
	// of the line along the segment and the plane 3, the line lies 
	// in the plane

	// first test of the line and the plane are parallel, if so 
	// then they can't intersect unless the line lies in the plane!

	float plane_dot_line = pt->VECTOR3D_Dot(&pline->v, &this->n);

	if (fabs(plane_dot_line) <= EPSILON_E5)
	{
		// the line and plane are co-planer, does the line lie 
		// in the plane?
		if (fabs(ComputePointInPlane(&pline->p0)) <= EPSILON_E5)
			return(PARM_LINE_INTERSECT_EVERYWHERE);
		else
			return(PARM_LINE_NO_INTERSECT);
	} // end if

	// from chapter 4 we know that we can solve for the t where 
	// intersection occurs by
	// a*(x0+vx*t) + b*(y0+vy*t) + c*(z0+vz*t) + d =0
	// t = -(a*x0 + b*y0 + c*z0 + d)/(a*vx + b*vy + c*vz)
	// x0,y0,z0, vx,vy,vz, define the line
	// d = (-a*xp0 - b*yp0 - c*zp0), xp0, yp0, zp0, define the point on the plane 

	*t = -(this->n.x*pline->p0.x +
		this->n.y*pline->p0.y +
		this->n.z*pline->p0.z -
		this->n.x*this->p0.x -
		this->n.y*this->p0.y -
		this->n.z*this->p0.z) / (plane_dot_line);

	// now plug t into the parametric line and solve for x,y,z
	pt->x = pline->p0.x + pline->v.x*(*t);
	pt->y = pline->p0.y + pline->v.y*(*t);
	pt->z = pline->p0.z + pline->v.z*(*t);

	// test interval of t
	if (*t >= 0.0 && *t <= 1.0)
		return(PARM_LINE_INTERSECT_IN_SEGMENT);
	else
		return(PARM_LINE_INTERSECT_OUT_SEGMENT);


}

Sloong::Math::CPlane::~CPlane()
{

}


CPlane::CPlane()
{
	a = 0; b = 0; c = 0; d = 0;
}


CPlane::CPlane(float A, float B, float C, float D)
{
	a = A; b = B; c = C; d = D;
}


CPlane::CPlane(CVector3 &n, float D)
{
	a = n.x; b = n.y; c = n.z; d = D;
}


void CPlane::CreatePlaneFromTri(CVector3 &v1, CVector3 &v2, CVector3 &v3)
{
	// Get triangle normal.
	CVector3 normal, e1, e2;

	e1 = v3 - v1;
	e2 = v2 - v1;

	e1.Normal();
	e2.Normal();

	normal.CrossProduct(e1, e2);
	normal.Normal();

	// Save normal and calculate d.
	a = normal.x;
	b = normal.y;
	c = normal.z;
	d = -(a * v1.x + b * v1.y + c * v1.z);
}


bool CPlane::Intersect(CPlane &p2, CPlane &p3, CVector3 *intersectPoint)
{
	CVector3 temp;
	CVector3 normal(a, b, c);
	CVector3 p2Normal(p2.a, p2.b, p2.c);
	CVector3 p3Normal(p3.a, p3.b, p3.c);

	// Get cross product of p2 and p3.
	temp.CrossProduct(p2Normal, p3Normal);

	// Get the dot prod of this plane with cross of p2 and p3.
	float dp = normal.DotProduct3(temp);
	if (dp == 0.0f) return false;

	// Else we must find intersect point.
	if (intersectPoint)
	{
		*intersectPoint = temp * d;

		temp.CrossProduct(p3Normal, normal);
		*intersectPoint = *intersectPoint + (temp * p2.d);

		temp.CrossProduct(normal, p2Normal);
		*intersectPoint = *intersectPoint + (temp * p3.d);

		*intersectPoint = *intersectPoint / -dp;
	}

	return true;
}


bool CPlane::Intersect(CPlane &pl, CVector3 *intersectPoint)
{
	CVector3 cross;
	CVector3 normal(a, b, c);
	CVector3 plNormal(pl.a, pl.b, pl.c);
	float length = 0;

	cross.CrossProduct(normal, plNormal);
	length = cross.DotProduct3(cross);

	if (length < 1e-08f) return false;

	if (intersectPoint)
	{
		float l0 = normal.DotProduct3(normal);
		float l1 = normal.DotProduct3(plNormal);
		float l2 = plNormal.DotProduct3(plNormal);
		float det = l0 * l2 - l1 * l1;
		float invDet = 0;

		if (fabs(det) < 1e-08f) return false;

		invDet = 1 / det;
		float d0 = (l2 * d - l1 * pl.d) * invDet;
		float d1 = (l0 * pl.d - l1 * d) * invDet;

		(*intersectPoint) = normal * d0 + plNormal * d1;
	}

	return true;
}


bool CPlane::Intersect(CPlane &pl, CRay *intersect)
{
	CVector3 cross;
	CVector3 normal(a, b, c);
	CVector3 plNormal(pl.a, pl.b, pl.c);
	float length = 0;

	cross.CrossProduct(normal, plNormal);
	length = cross.DotProduct3(cross);

	if (length < 1e-08f) return false;

	if (intersect)
	{
		float l0 = normal.DotProduct3(normal);
		float l1 = normal.DotProduct3(plNormal);
		float l2 = plNormal.DotProduct3(plNormal);
		float det = l0 * l2 - l1 * l1;
		float invDet = 0;

		if (fabs(det) < 1e-08f) return false;

		invDet = 1 / det;
		float d0 = (l2 * d - l1 * pl.d) * invDet;
		float d1 = (l0 * pl.d - l1 * d) * invDet;

		(*intersect).m_origin = normal * d0 + plNormal * d1;
		(*intersect).m_direction = cross;
	}

	return true;
}


int CPlane::ClassifyPolygon(CPolygon &pol)
{
	int frontPolys = 0;
	int backPolys = 0;
	int planePolys = 0;
	int type = 0;

	for (int i = 0; i < 3; i++)
	{
		type = ClassifyPoint(pol.m_vertexList[i]);

		switch (type)
		{
		case UGP_FRONT:
			frontPolys++;
			break;

		case UGP_BACK:
			backPolys++;
			break;

		default:
			frontPolys++;
			backPolys++;
			planePolys++;
			break;
		}
	}

	if (planePolys == 3) return UGP_ON_PLANE;
	else if (frontPolys == 3) return UGP_FRONT;
	else if (backPolys == 3) return UGP_BACK;

	return UGP_CLIPPED;
}


int CPlane::ClassifyPoint(CVector3 &v)
{
	float distance = a * v.x + b * v.y + c * v.z + d;

	if (distance > 0.001) return UGP_FRONT;
	if (distance < -0.001) return UGP_BACK;

	return UGP_ON_PLANE;
}


int CPlane::ClassifyPoint(float x, float y, float z)
{
	float distance = a * x + b * y + c * z + d;

	if (distance > 0.001) return UGP_FRONT;
	if (distance < -0.001) return UGP_BACK;

	return UGP_ON_PLANE;
}


void CPlane::Lerp(CPlane &p1, CPlane &p2, float amount)
{
	// First calculate a, b, c, d.
	a = p1.a * (1.0f - amount) + p2.a * amount;
	b = p1.b * (1.0f - amount) + p2.b * amount;
	c = p1.c * (1.0f - amount) + p2.c * amount;
	d = p1.d * (1.0f - amount) + p2.d * amount;

	// Re-normalize plane.
	float length = (float)sqrt((a * a + b * b + c * c));

	if (length == 0.0f) length = 1.0f;
	length = 1 / length;

	// Normalize all but d.
	a = a * length;
	b = b * length;
	c = c * length;
}