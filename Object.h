#pragma once
#include "Matrix3D.h"
#include "Image.h"

// Base class for all objects in the scene.
class Object
{
public:
	Object(Point3D centrePoint = Point3D()) : m_centre(centrePoint) {}
	virtual ~Object() {}

	// Returns true if the ray intersects with this object.
	// Params:
	//	raySrc					starting point of the ray (input)
	//	rayDir					direction of the ray (input)
	//	distToFirstIntersection	distance along the ray from the starting point of the first intersection with the object (output)
	virtual bool getIntersection(const Point3D& raySrc, const Vector3D& rayDir, float& distToFirstIntersection) const = 0;

	// Transforms the object using the given matrix.
	virtual void applyTransformation(const Matrix3D& matrix) = 0;
	
	// The object's RGBA colour
	Colour	m_colour = Colour(126, 126, 126);

protected:
	Point3D m_centre;	// The coordinates of the object's centre in world space.
};

// A plane is a 2D surface defined by its normal and 'centre' point,
// plus (optionally) height and width limits (to form a rectangle).
class Plane : public Object
{
public:
	Plane(Point3D centrePoint = Point3D(),
		Vector3D n = Vector3D(0.0f, 1.0f, 0.0f),
		Vector3D up = Vector3D(0.0f, 0.0f, 1.0f),
		float w = 0.0f, float h = 0.0f);
	virtual ~Plane() {}

	virtual bool getIntersection(const Point3D& raySrc, const Vector3D& rayDir, float& distToFirstIntersection) const;
	virtual void applyTransformation(const Matrix3D& matrix);

private:
	// The plane's orientation is defined by its normal and the directions of its width and height in world space.
	Vector3D	m_normal = Vector3D(0.0f, 1.0f, 0.0f),
				m_wDir = Vector3D(1.0f, 0.0f, 0.0f),
				m_hDir = Vector3D(0.0f, 0.0f, 1.0f);

	// The plane's size is given by its dimensions along the width and height axes (if these are zero or less, the plane is infinite).
	float		m_halfWidth, m_halfHeight;
};

// A sphere is defined by its centre and radius
class Sphere : public Object
{
public:
	Sphere(Point3D centrePoint = Point3D(), float r = 1.0f) : Object(centrePoint), m_radius2(r * r) {}
	virtual ~Sphere() {}

	virtual bool getIntersection(const Point3D& raySrc, const Vector3D& rayDir, float& distToFirstIntersection) const;
	virtual void applyTransformation(const Matrix3D& matrix);

private:
	float	m_radius2;	// The squared radius of the sphere
};