#include "Sphere.h"

#include "utility.h"

Sphere::Sphere() : Object() {

}

Sphere::Sphere(const Sphere& sphere) : Object(sphere) {

}

Sphere::~Sphere() {

}

Sphere& Sphere::operator=(const Sphere& sphere) {
	if (this != &sphere) {
		Object::operator=(sphere);
	}
	return *this;
}

std::vector<RayIntersection> Sphere::intersect(const Ray& ray) const {

	std::vector<RayIntersection> result;

	Ray inverseRay = transform.applyInverse(ray);

	// Intersection is of the form ad^2 + bd + c, where d = distance along the ray

	const Point& p = inverseRay.point;
	const Direction& d = inverseRay.direction;
	double a = d.dot(d);
	double b = 2 * d.dot(p);
	double c = p.dot(p) - 1;

	std::vector<double> roots = solveQuadratic(a, b, c);

	for (double t : roots) {
		if (t > epsilon) {
			RayIntersection hit;
			hit.point = transform.apply(Point(p + t * d));
			hit.normal = transform.apply(Normal(p + t * d));
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