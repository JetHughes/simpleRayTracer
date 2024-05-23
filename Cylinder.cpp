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

	double a = d(0) * d(0) + d(1) * d(1);
	double b = 2 * (p(0) * d(0) + p(1) * d(1));
	double c = p(0) * p(0) + p(1) * p(1) - 1;

	std::vector<double> roots = solveQuadratic(a, b, c);

	for (double t : roots) {
		if (t > epsilon) {
			Point hitPoint = p + t * d;
			double z = hitPoint(2);
			if ((z * z) <= 1)
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

	// caps
	/*
	for (int i = -1; i <=1; i+=2)
	{
		Point centre(0, 0, i);
		Normal normal(0, 0, -i);
			
		// ray is parallel to the plane
		if (abs(d.dot(normal)) < epsilon) {
			return result;
		}

		double t = (centre - p).dot(normal) / d.dot(normal);
		Point hitPoint = p + t * d;

		double x = hitPoint(0);
		double y = hitPoint(1);

		if ((x * x + y * y) <= 1)
		{
			RayIntersection hit;
			hit.point = transform.apply(hitPoint);
			hit.normal = transform.apply(normal);
			if (hit.normal.dot(ray.direction) > 0) {
				hit.normal = -hit.normal;
			}
			hit.normal = hit.normal / hit.normal.norm();
			hit.distance = (hit.point - ray.point).norm();
			hit.material = material;
			result.push_back(hit);
		}
	}*/


	return result;
}
