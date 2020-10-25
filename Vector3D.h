#pragma once

// A class for performing basic operations with homogeneous vectors in 3D space.
// Feel free to edit/extend!
class Vector3D
{
public:
	Vector3D(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f) : x(x_), y(y_), z(z_), w(0.0f) {}

	float x, y, z, w;

	// Get the length of the vector
	float magnitude() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	// Make the vector unit length
	void normalise()
	{
		float mag = magnitude();
		x /= mag;
		y /= mag;
		z /= mag;
	}

	// Vector dot product
	float dot(const Vector3D& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	// Vector cross product
	Vector3D cross(const Vector3D& other) const
	{
		Vector3D result;
		result.x = y * other.z - z * other.y;
		result.y = z * other.x - x * other.z;
		result.z = x * other.y - y * other.x;
		return result;
	}

	// Multiply the vector by as scalar
	Vector3D operator*(float scalar) const
	{
		return Vector3D(x * scalar, y * scalar, z * scalar);
	}
};