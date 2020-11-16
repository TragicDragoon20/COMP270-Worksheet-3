#include "stdafx.h"
#include "Object.h"

// Plane constructor. Params are:
//	centrePoint		The point on the plane from which the width and height limits are measured
//	n				The unit vector that is normal to the plane (in world space)
//	up				The unit vector along which the plane height is measured (in world space; should be orthogonal to the normal)
//	w, h			The width and height of the plane (zero/negative for an infinite plane)
Plane::Plane(Point3D centrePoint, Vector3D n, Vector3D up, float w, float h) :
	Object(centrePoint),
	m_heightDirection(up),
	m_normal(n),
	m_halfWidth(w / 2.0f),
	m_halfHeight(h / 2.0f)
{
	m_normal.normalise();
	m_isBounded = m_halfWidth > 0.0f && m_halfHeight > 0.0f;
	if (m_isBounded)
	{
		m_widthDirection = m_heightDirection.cross(m_normal);
		m_widthDirection.normalise();
		m_heightDirection.normalise();
		m_halfDiagonal = sqrt(m_halfWidth * m_halfWidth + m_halfHeight * m_halfHeight);
	}
}

//--------------------------------------------------------------------------------------------------------------------//

// Returns true if the ray intersects with this plane.
// Params:
//	raySrc					source/starting point of the ray (input) p1
//	rayDir					direction of the ray (input) v
//	distToFirstIntersection	distance along the ray from the starting point of the first intersection with the plane (output) t
bool Plane::getIntersection(const Point3D& raySrc, const Vector3D& rayDir, float& distToFirstIntersection) const
{

	
	// TODO: implement the ray-plane intersection test, returning true if the ray passes through the plane at a
	// point within the width/height bounds (if applicable) as defined by the values of m_halfWidth and m_halfHeight
	// in the directions m_widthDirection and m_heightDirection respectively. Make sure you set distToFirstIntersection
	// to be the distance along the ray from its starting point/source to the point of intersection.

	// using the equation t = (p2 - p1)dot(n) / vdot(n)
	// where p2 = m_centre, p1 = raySrc, n = m_normal and v = rayDir
	distToFirstIntersection = (m_centre - raySrc).dot(m_normal) / rayDir.dot(m_normal);

	// using the equation p = p1 + vt
	// where p is the intersection point, p1 = raySrc, v = rayDir and t = distToFirstIntersection
	Point3D intersectionPoint = raySrc + rayDir * distToFirstIntersection;

	Vector3D intersectvector = intersectionPoint - m_centre;
	// horizontalDot and verticalDot are used to check if the intersection point is withing the plane
	float horizontalDot = intersectvector.dot(m_widthDirection);
	float verticalDot = intersectvector.dot(m_heightDirection);

	
	if (horizontalDot < m_halfWidth && horizontalDot > -m_halfWidth && verticalDot < m_halfHeight && verticalDot > -m_halfHeight)
	{
		return true;
	}
	
	return false;
}

//--------------------------------------------------------------------------------------------------------------------//

// Transforms the object using the given matrix.
void Plane::applyTransformation(const Matrix3D & matrix)
{
	m_centre = matrix * m_centre;
	m_heightDirection = matrix * m_heightDirection;
	m_widthDirection = matrix * m_widthDirection;
	m_normal = matrix * m_normal;
}

// Returns true if the ray intersects with this sphere.
// Params:
//	raySrc					starting point of the ray (input)
//	rayDir					direction of the ray (input)
//	distToFirstIntersection	distance along the ray from the starting point of the first intersection with the sphere (output)
bool Sphere::getIntersection(const Point3D& raySrc, const Vector3D& rayDir, float& distToFirstIntersection) const
{
	// Find the point on the ray closest to the sphere's centre
	Vector3D srcToCentre = m_centre - raySrc; // p2 - p1
	float tc = srcToCentre.dot(rayDir);
	
	// Check whether the closest point is inside the sphere
	if (tc > 0.0f)
	{
		float distSq = srcToCentre.dot(srcToCentre) - tc * tc;
		if (distSq < m_radius2)
		{
			distToFirstIntersection = tc - sqrt(m_radius2 - distSq);
			return true;
		}
	}

	return false;
}

// Transforms the object using the given matrix.
void Sphere::applyTransformation(const Matrix3D & matrix)
{
	m_centre = matrix * m_centre;
}
