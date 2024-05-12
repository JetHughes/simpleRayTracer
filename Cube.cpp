/* $Rev: 250 $ */
#include "Cube.h"
#include <algorithm>
#include "utility.h"

Cube::Cube() : Object() {

}

Cube::Cube(const Cube& cube) : Object(cube) {

}

Cube::~Cube() {

}

Cube& Cube::operator=(const Cube& cube) {
	if (this != &cube) {
		Object::operator=(cube);
	}
	return *this;
}

Vector clamp(Vector& n, const Vector& lower, const Vector& upper) {
	for (int i = 0; i < 3; i++) {
		n(i) = std::max(lower(i), std::min(n(i), upper(i)));
	}
	return n;
}

std::vector<RayIntersection> Cube::intersect(const Ray& ray) const {
	std::vector<RayIntersection> result;

	/***********************************************
	 * Ray-Cube Intersection is not implemented.   *
	 * You need to implement it for the assignment.*
	 ***********************************************/

	Ray inverseRay = transform.applyInverse(ray);

	const Point& p = inverseRay.point;
	const Direction& d = inverseRay.direction;

	double min = -1;
	double max = 1;

	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	if (d(1) >= 0) {
		tmin = (min - p(1)) / d(1);
		tmax = (max - p(1)) / d(1);
	}
	else {
		tmin = (max - p(1)) / d(1);
		tmax = (min - p(1)) / d(1);
	}
	if (d(2) >= 0) {
		tymin = (min - p(1)) / d(2);
		tymax = (max - p(1)) / d(2);
	}
	else {
		tymin = (max - p(1)) / d(2);
		tymax = (min - p(1)) / d(2);
	}
	if ((tmin > tymax) || (tymin > tmax))
		return result;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	if (d(2) >= 0) {
		tzmin = (min - p(2)) / d(2);
		tzmax = (max - p(2)) / d(2);
	}
	else {
		tzmin = (max - p(2)) / d(2);
		tzmax = (min - p(2)) / d(2);
	}
	if ((tmin > tzmax) || (tzmin > tmax))
		return result;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
	if ((tmin < 3) && (tmax > 0.1))
	{
		RayIntersection hit;
		hit.point = transform.apply(Point(p + tmin * d));
		//hit.normal = Normal(0, 1, 0); 
		/*hit.normal = hit.point / fmax(fmax(abs(hit.point(0)), abs(hit.point(1))), abs(hit.point(2)));
		hit.normal = clamp(Point(hit.normal), Point(0.0, 0.0, 0.0), Point(1.0, 1.0, 1.0));
		hit.normal = hit.normal / hit.normal.norm();*/

		// compute normal


		hit.distance = tmin;
		hit.material = material;
		result.push_back(hit);
	}
	return result;
}