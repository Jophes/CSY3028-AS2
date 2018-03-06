#include "Vec3.h"
#include <cmath>

// Constructors
Vec3::Vec3() {
	x = 0;
	y = 0;
}

Vec3::Vec3(double _x, double _y, double _z) {
	x = _x;
	y = _y;
	z = _z;
}

// Object methods
double Vec3::Magnitude() {
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

double Vec3::Distance(Vec3 other) {
	return ((*this) - other).Magnitude();
}

double Vec3::DotProduct(Vec3 other) {
	return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::Normalise() {
	return (*this) / Magnitude();
}

Vec3 Vec3::Direction(Vec3 other) {
	return (other - (*this)).Normalise();
}

void Vec3::Set(Vec3 newVal) {
	x = newVal.x;
	y = newVal.y;
	z = newVal.z;
}

void Vec3::Set(double _x, double _y, double _z) {
	x = _x;
	y = _y;
	z = _z;
}


std::string Vec3::ToString() {
	return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
}