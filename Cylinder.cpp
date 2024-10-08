#include "Cylinder.h"

#include "utility.h"

Cylinder::Cylinder() : Object() {

}

Cylinder::Cylinder(const Cylinder& cylinder) : Object(cylinder) {

}

Cylinder::~Cylinder() {

}

Cylinder& Cylinder::operator=(const Cylinder& cylinder) {
	if (this != &cylinder) {
		Object::operator=(cylinder);
	}
	return *this;
}

std::vector<RayIntersection> Cylinder::intersect(const Ray& ray) const {

	std::vector<RayIntersection> result;

	Ray inverseRay = transform.applyInverse(ray);
	const Point& p = inverseRay.point;
	const Direction& d = inverseRay.direction;

	// cylinder centered at origin with radius 1 and length 2 along z-axis
	// main body
	double r = 1;

	// With with reference to https://mrl.cs.nyu.edu/~dzorin/rendering/lectures/lecture3/lecture3.pdf
	double a = d(0) * d(0) + d(1) * d(1);
	double b = 2 * (p(0) * d(0) + p(1) * d(1));
	double c = p(0) * p(0) + p(1) * p(1) - 1;

	std::vector<double> roots = solveQuadratic(a, b, c);

	for (double t : roots) {
		if (t > epsilon) {
			Point hitPoint = p + t * d;
			double z = hitPoint(2);
			if ((z * z) <= r*r)
			{
				RayIntersection hit;
				hit.point = transform.apply(Point(p + t * d));				
				hit.normal = Normal(hitPoint(0), hitPoint(1), 0);
				hit.normal = transform.apply(hit.normal);
				hit.normal = hit.normal / hit.normal.norm();
				if (hit.normal.dot(ray.direction) > 0) {
					hit.normal = -hit.normal;
				}
				hit.distance = (hit.point - ray.point).norm();
				hit.material = material;
				result.push_back(hit);
			}
		}
	}

	// Adapted from http://cosinekitty.com/raytrace/chapter11_reorientable.html#sect_11_7
	double t;
	if (fabs(d(2)) > epsilon) {
		t = (r - p(2)) / d(2);
		if (t>epsilon) {
			RayIntersection hit;
			hit.point = p + t * d;
			double x = hit.point(0);
			double y = hit.point(1);

			if((x * x + y * y) <= r*r)
			{
				hit.point = transform.apply(hit.point);
				hit.normal = Normal(0, 0, 1);
				hit.normal = transform.apply(hit.normal);
				hit.normal = hit.normal / hit.normal.norm();
				if (hit.normal.dot(ray.direction) > 0) {
					hit.normal = -hit.normal;
				}
				hit.distance = (hit.point - ray.point).norm();
				hit.material = material;
				result.push_back(hit);
			}
		}

		t = (-r - p(2)) / d(2);
		if (t>epsilon) {
			RayIntersection hit;
			hit.point = p + t * d;
			double x = hit.point(0);
			double y = hit.point(1);

			if((x * x + y * y) <= r*r)
			{
				hit.point = transform.apply(hit.point);
				hit.normal = Normal(0, 0, -1);
				hit.normal = transform.apply(hit.normal);
				hit.normal = hit.normal / hit.normal.norm();
				if (hit.normal.dot(ray.direction) > 0) {
					hit.normal = -hit.normal;
				}
				hit.distance = (hit.point - ray.point).norm();
				hit.material = material;
				result.push_back(hit);
			}
		}
	}

	return result;
}
