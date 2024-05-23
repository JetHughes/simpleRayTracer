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

Vector clampVec(Vector& n, const Vector& lower, const Vector& upper) {
	for (int i = 0; i < 3; i++) {
		n(i) = std::max(lower(i), std::min(n(i), upper(i)));
	}
	return n;
}

Vector floorVec(Vector& n) {
	for (int i = 0; i < 3; i++) {
		n(i) = floor(n(i));
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

	//hit.normal = Normal(1, 0, 0);

	//if (abs(hitPoint(0)) < epsilon) {
	//	std::cout << "hit" << std::endl;
	//} else if (abs(hitPoint(1) < epsilon)) {
	//	std::cout << "hit" << std::endl;
	//} else if (abs(hitPoint(2) < epsilon)) {
	//	std::cout << "hit" << std::endl;
	//} else if (abs(hitPoint(0)) > 1-epsilon) {
	//	std::cout << "hit" << std::endl;
	//} else if (abs(hitPoint(1)) > 1-epsilon) {
	//	std::cout << "hit" << std::endl;
	//} else if (abs(hitPoint(2)) > 1-epsilon) {
	//	std::cout << "hit" << std::endl;
	//}

	//hit.normal = hitPoint / fmax(fmax(abs(hitPoint(0)), abs(hitPoint(1))), abs(hitPoint(2)));
	//hit.normal = clampVec(hit.normal, Point(0, 0, 0), Point(1, 1, 1));
	//hit.normal = floorVec(hit.normal * 1.000000001);
	//hit.normal = transform.apply(hit.normal);
	//hit.normal = hit.normal / hit.normal.norm();

	//hitPoint = clampVec(hitPoint, Point(0, 0, 0), Point(1, 1, 1));
	//hitPoint = floorVec(hitPoint * 1.000000001);

	//if (hitPoint(0) < epsilon) {
	//	hit.normal = Normal(0, 0, 1);
	//}
	//else if (hitPoint(0) > 1 - epsilon) {
	//	hit.normal = Normal(0, 0, -1);
	//}
	//else if (hitPoint(1) < epsilon) {
	//	hit.normal = Normal(0, 1, 0);
	//}
	//else if (hitPoint(1) > 1 - epsilon) {
	//	hit.normal = Normal(0, -1, 0);
	//}
	//else if (hitPoint(2) < epsilon) {
	//	hit.normal = Normal(1, 0, 0);
	//}
	//else if (hitPoint(2) > 1 - epsilon) {
	//	hit.normal = Normal(-1, 0, 0);
	//}
	//else
	//{
	//	return result;
	//}

	//hit.normal = hitPoint;


	if (hit.normal.dot(ray.direction) > 0) {
		hit.normal = -hit.normal;
	}
	hit.normal = transform.apply(hit.normal);
	hit.normal = hit.normal/hit.normal.norm();

	hit.distance = (hit.point - ray.point).norm();
	hit.material = material;
	result.push_back(hit);

	return result;
}