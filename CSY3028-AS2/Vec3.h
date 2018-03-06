#pragma once

#include <string>

class Vec3 {
public:
	// Object properties
	double x, y, z;

	// Constructors
	Vec3();
	Vec3(double x, double y, double z);

	// Operator overloads
	friend Vec3 operator*(Vec3 lhs, const double& rhs) {
		return Vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
	}
	friend Vec3 operator*(Vec3 lhs, const Vec3& rhs) {
		return Vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
	}
	friend Vec3 operator/(Vec3 lhs, const double& rhs) {
		return Vec3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
	}
	friend Vec3 operator+(Vec3 lhs, const Vec3& rhs) {
		return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}
	friend Vec3 operator-(Vec3 lhs, const Vec3& rhs) {
		return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	// Object methods
	double Magnitude();
	double Distance(Vec3 other);
	double DotProduct(Vec3 other);
	Vec3 Normalise();
	Vec3 Direction(Vec3 other);
	void Set(Vec3 newVals = Vec3());
	void Set(double x = 0.0, double y = 0.0, double z = 0.0);
	std::string ToString();
};

