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

	Ray inverseRay = transform.applyInverse(ray);
	const Point& p = inverseRay.point;
	const Direction& d = inverseRay.direction/inverseRay.direction.norm();

	// adapted from https://psgraphics.blogspot.com/2016/02/new-simple-ray-box-test-from-andrew.html
	float tmin = -INFINITY;
	float tmax = INFINITY;
	std::vector<int> mins;
	std::vector<int> maxs;
	for (int a = 0; a < 3; a++) {
		float invD = 1.0f / d(a);
		float t0 = (0 - p(a)) * invD;
		float t1 = (1 - p(a)) * invD;
		if (invD < 0.0f) {
			std::swap(t0, t1);
		}
		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;
		if (tmax <= tmin) {
			return result;
		}
	}
	
	RayIntersection hit;
	Point hitPoint = p + tmin * d;
	hit.point = transform.apply(hitPoint);

	hit.normal = Normal(0, -1, 0);
	if (hit.normal.dot(ray.direction) > 0) {
		hit.normal = -hit.normal;
	}
	hit.normal = transform.apply(hit.normal);

	hit.distance = tmin;
	hit.material = material;
	result.push_back(hit);

	return result;

	

	

	/*
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
		hit.normal = hit.point / fmax(fmax(abs(hit.point(0)), abs(hit.point(1))), abs(hit.point(2)));
		hit.normal = clamp(Point(hit.normal), Point(0.0, 0.0, 0.0), Point(1.0, 1.0, 1.0));
		hit.normal = hit.normal / hit.normal.norm();
		hit.distance = tmin;
		hit.material = material;
		result.push_back(hit);
	}
	return result; */
}