#include "Plane.h"

#include "utility.h"

Plane::Plane() : Object() {

}

Plane::Plane(const Plane& plane) : Object(plane) {

}

Plane::~Plane() {

}

Plane& Plane::operator=(const Plane& plane) {
	if (this != &plane) {
		Object::operator=(plane);
	}
	return *this;
}

std::vector<RayIntersection> Plane::intersect(const Ray& ray) const {

	std::vector<RayIntersection> result;

	/***********************************************
	 * Ray-Plane Intersection is not implemented.  *
	 * You need to implement it for the assignment.*
	 ***********************************************/


	Ray inverseRay = transform.applyInverse(ray);

	const Point& p = inverseRay.point;
	const Direction& d = inverseRay.direction/inverseRay.direction.norm();

	// Point p is on the plane if (p - a) dot n = 0
	// Ray: r = r0 + td
	// Substitute r into plane equation:
	// (r0 + td - a) dot n = 0
	// solve for t:
	// t = (a - r0) dot n / d dot n
	
	// Plane at origin facing the positive y-axis
	Point a(0.0, 0.0, 0.0);
	Normal n(0.0, 1.0, 0.0);

	// ray is parallel to the plane
	if (abs(d.dot(n)) < epsilon) {
		return result;
	}

	double t = (a - p).dot(n) / d.dot(n);

	if (t > epsilon) {
		RayIntersection hit;
		Vector point = p + t * d;
		
		double x = point(0);
		double z = point(2);
		if ((x * x) <= 1 && (z*z) <= 1)
		{
			hit.point = transform.apply(Point(point));
			hit.normal = transform.apply(Normal(n));
			if (hit.normal.dot(ray.direction) > 0) {
				hit.normal = -hit.normal;
			}
			hit.distance = (hit.point - ray.point).norm();
			hit.material = material;
			result.push_back(hit);
		}
	}

	return result;
}