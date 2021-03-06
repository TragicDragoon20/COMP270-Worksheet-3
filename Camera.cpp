#include "stdafx.h"
#include "Camera.h"
#include "Object.h"

// Initialises the camera at the given position
void Camera::init(const Point3D& pos)
{
	m_position = pos;
	m_pixelBuf.init(m_viewPlane.resolutionX, m_viewPlane.resolutionY);

//--------------------------------------------------------------------------------------------------------------------//
	// TODO: initialise these values to store the size (in screen units)
	// of each pixel based on the view plane resolution.

	// To calculate the pixel width and height we take the total width and height of the view plane
	// and divide it by the resolution
	m_pixelWidth = m_viewPlane.halfWidth * 2 / m_viewPlane.resolutionX;
	m_pixelHeight = m_viewPlane.halfHeight * 2 / m_viewPlane.resolutionY;
//--------------------------------------------------------------------------------------------------------------------//
}

// Cast rays through the view plane and set colours based on what they intersect with
bool Camera::updatePixelBuffer(const std::vector<Object*>& objects)
{
	if (m_pixelBuf.isInitialised())
	{
		m_pixelBuf.clear();

		// Make sure our cached values are up to date
		if (m_worldTransformChanged)
		{
			updateWorldTransform();
			m_worldTransformChanged = false;
		}

		// Transform the objects to the camera's coordinate system
		const Matrix3D worldToCameraTransform = m_cameraToWorldTransform.inverseTransform();
		for (auto obj : objects)
			obj->applyTransformation(worldToCameraTransform);

		// Fill the pixel buffer with pointers to the closest object for each pixel
		Point3D origin;
		Vector3D rayDir;
		float distToIntersection;
		for (const auto obj : objects)
		{
			// Find the pixel that's intersected by the line from the
			// camera to the object's centre
			Vector3D toCentre = obj->position().asVector();
			toCentre.normalise();
				
			// Centre line intersects the view plane when the z value
			// is the distance to the view plane
			const float t = m_viewPlane.distance / toCentre.z;
			const float viewPlaneX = toCentre.x * t + m_viewPlane.halfWidth,
					viewPlaneY = toCentre.y * t + m_viewPlane.halfHeight;
				
			// Find the pixel indices of the centre line intersection point
			const int pixelX = static_cast<int>(viewPlaneX / m_pixelWidth),
					pixelY = static_cast<int>(viewPlaneY / m_pixelHeight);

			// Find the largest range of pixels that the object might cover,
			// based on its maximum 'radius'.
			const float objectRad = fabsf(obj->getMaxRadius());
			const int pixelRadiusX = static_cast<int>(objectRad / m_pixelWidth) + 1,
					pixelRadiusY = static_cast<int>(objectRad / m_pixelHeight) + 1;
				
			// Find the range of pixels that might be covered by the object,
			// clamped to the view plane resolution
			const unsigned startX = max(pixelX - pixelRadiusX, 0), endX = min(pixelX + pixelRadiusX, m_viewPlane.resolutionX),
					startY = max(pixelY - pixelRadiusY, 0), endY = min(pixelY + pixelRadiusY, m_viewPlane.resolutionY);
			
			// For each of the pixels that might be covered by the object, find the direction
			// of the ray passing through it and test whether it intersects with the object
			for (unsigned i = startX; i < endX; ++i)
			{
				for (unsigned j = startY; j < endY; ++j)
				{
//--------------------------------------------------------------------------------------------------------------------//
					// TODO: if you want to pass through any extra information from the intersection test
					// for Task 4, this is the place to do so. 
					rayDir = getRayDirectionThroughPixel(i, j);

					// Perform the intersection test between the ray through this pixel and the object,
					// and check whether the intersection point is closer than that of previously tested objects
					if (obj->getIntersection(origin, rayDir, distToIntersection)	
						&& distToIntersection < m_pixelBuf.getObjectInfoForPixel(i, j).distanceToIntersection)
					{
						m_pixelBuf.setObjectInfoForPixel(i, j, ObjectInfo(obj, distToIntersection));
					}
//--------------------------------------------------------------------------------------------------------------------//
				}
			}
		}

		// Now put the objects back!
		for (auto obj : objects)
			obj->applyTransformation(m_cameraToWorldTransform);

		return true;
	}
	
	return false;
}

//--------------------------------------------------------------------------------------------------------------------//

// Calculates the normalised direction in camera space of a ray from
// the camera through the view-plane pixel at index (i, j),
// where 0 <= i < m_viewPlane.resolutionX and 0 <= j < m_viewPlane.resolutionY.
Vector3D Camera::getRayDirectionThroughPixel(int i, int j)
{
	// TODO: set rayDir to be the normalised ray direction through the pixel
	// with grid coordinates (i, j). You can use any of the member variables
	// in Camera.h, in particular the settings in m_viewPlane.


	// To get the world space coordinates we multiply the width and height by the iteration value
	// Since the plane is centered at (0, 0) and has a half width and height of 3 the plane therefore spans from (-3, -3) to (3, 3)
	// To make sure the rays pass through our view plane we need to take half the width and height
	// Whilst the z part is just the view plane distance
	const float cameraSpaceX = (static_cast<float>(i) * m_pixelWidth) - m_viewPlane.halfWidth;
	const float cameraSpaceY = (static_cast<float>(j) * m_pixelHeight) - m_viewPlane.halfHeight;

	Vector3D rayDir = Vector3D(cameraSpaceX, cameraSpaceY, m_viewPlane.distance);

	rayDir.normalise();
	return rayDir;
}

// Computes the transformation of the camera in world space, which is also the
// transform that will take objects from camera to world coordinates
// and stores it in m_cameraToWorldTransform
void Camera::updateWorldTransform()
{
	// TODO: the following code creates a transform for a camera with translation only
	// plus a flip on the z-axis to change from right- to left-handed coordinates;
	// update it to handle a generic transformation including rotations.
	// The Euler angle values for rotations about each axis are stored in m_rotation and should be
	// included in the Matrix3D m_cameraToWorldTransform; you can access an element of the matrix
	// at row i and column j using the () operator for both get and set operations, e.g.
	// matrix(i, j) = value or value = matrix(i, j) will both work.
	m_cameraToWorldTransform(0, 3) = m_position.x;
	m_cameraToWorldTransform(1, 3) = m_position.y;
	m_cameraToWorldTransform(2, 3) = m_position.z;
	m_cameraToWorldTransform(2, 2) = -1.0f;	// scale of -1 on the z-axis

}

// Gets the colour of a given pixel based on the closest object as stored in the pixel buffer
// Params:
//	i, j	Pixel x, y coordinates
Colour Camera::getColourAtPixel(unsigned i, unsigned j)
{
	Colour colour;
	// TODO: update this to make the colouring more interesting!

	// You can use the object information stored in m_pixelBuf
	// for the object and its intersection; if you want to add more information
	// from the intersection test, you'll need to:
	// 1. calculate and pass the values back from Object::getIntersection() (and the derived class overrides)
	// 2. update the ObjectInfo struct in PixelBuffer.h to store the appropriate value types
	// 3. update the marked section in updatePixelBuffer() to store the values in m_pixelBuf

	// You can also retrieve the direction of the ray that intersects the object
	// by calling getRayDirectionThroughPixel(i, j); remember that the ray originates
	// from the camera, whose position is just the origin in camera space.

	// The ObjectInfo struct (defined in PixelBuffer.h) contains
	// information about the closest object to the pixel 
	const ObjectInfo& objInfo = m_pixelBuf.getObjectInfoForPixel(i, j);

	const Object* object = objInfo.object;
	if (object != nullptr)
		colour = object->m_colour;

	return colour;
}