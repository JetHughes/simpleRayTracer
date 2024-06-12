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

	Ray inverseRay = transform.applyInverse(ray);

	const Point& p = inverseRay.point;
	const Direction& d = inverseRay.direction/inverseRay.direction.norm();

	double sideLength = 1;

	Normal N(0, 0, 1);

	// Made with reference to https://www.cs.princeton.edu/courses/archive/fall00/cs426/lectures/raycast/sld017.htm
	// and http://cosinekitty.com/raytrace/chapter11_reorientable.html#sect_11_7
	if(fabs(d(2)) > epsilon) { // Check that the ray is not parallel to the plane
		double t = -(p.dot(N)) / d.dot(N);
		if (t > epsilon) {
			RayIntersection hit;
			hit.point = p + t * d;
			double x = hit.point(0);
			double y = hit.point(1);
			// Check that the intersection point is within the bounds of the plane
			if (fabs(x) <= sideLength + epsilon && 
				fabs(y) <= sideLength + epsilon) {
				hit.point = transform.apply(hit.point);
				hit.normal = transform.apply(N);
				if (hit.normal.dot(ray.direction) > 0) {
					hit.normal = -hit.normal;
				}
				hit.normal = hit.normal / hit.normal.norm();
				hit.distance = (hit.point - ray.point).norm();
				hit.material = material;
				result.push_back(hit);
			}
		}	
	}
	return result;
}