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

Vector floorVec(Vector n) {
	for (int i = 0; i < 3; i++) {
		n(i) = floor(n(i));
	}
	return n;
}

// Adapted from http://cosinekitty.com/raytrace/chapter11_reorientable.html#sect_11_7
bool isInside(const Point& p, double halfSideLength) {
	return
		(fabs(p(0)) <= halfSideLength + epsilon) &&
		(fabs(p(1)) <= halfSideLength + epsilon) &&
		(fabs(p(2)) <= halfSideLength + epsilon);
}

std::vector<RayIntersection> Cube::intersect(const Ray& ray) const {
	std::vector<RayIntersection> result;

	Ray inverseRay = transform.applyInverse(ray);
	const Point& p = inverseRay.point;
	const Direction& d = inverseRay.direction/inverseRay.direction.norm();

	double halfSideLength = 0.5;

	// Adapted from http://cosinekitty.com/raytrace/chapter11_reorientable.html#sect_11_7
	double t;

	// Check for intersections with the six faces of the cube
	if (fabs(d(0)) > epsilon) {
		t=(halfSideLength-p(0))/d(0);
		if (t>epsilon) {
			RayIntersection hit;	
			hit.point = p + t*d;
			if (isInside(hit.point, halfSideLength)) {
				hit.point = transform.apply(hit.point);
				hit.normal = Normal(1, 0, 0);				
				if (hit.normal.dot(ray.direction) > 0) {
					hit.normal = -hit.normal;
				}
				hit.material = material;
				hit.distance = (hit.point - ray.point).norm();
				result.push_back(hit);
			}			
		}

		t=(-halfSideLength-p(0))/d(0);
		if (t>epsilon) {
			RayIntersection hit;	
			hit.point = p + t*d;
			if (isInside(hit.point, halfSideLength)) {
				hit.point = transform.apply(hit.point);
				hit.normal = Normal(-1, 0, 0);				
				if (hit.normal.dot(ray.direction) > 0) {
					hit.normal = -hit.normal;
				}
				hit.material = material;
				hit.distance = (hit.point - ray.point).norm();
				result.push_back(hit);
			}			
		}		
	}

	if (fabs(d(1)) > epsilon) {
		t=(halfSideLength-p(1))/d(1);
		if (t>epsilon) {
			RayIntersection hit;	
			hit.point = p + t*d;
			if (isInside(hit.point, halfSideLength)) {
				hit.point = transform.apply(hit.point);
				hit.normal = Normal(0, 1, 0);				
				if (hit.normal.dot(ray.direction) > 0) {
					hit.normal = -hit.normal;
				}
				hit.material = material;
				hit.distance = (hit.point - ray.point).norm();
				result.push_back(hit);
			}			
		}

		t=(-halfSideLength-p(1))/d(1);
		if (t>epsilon) {
			RayIntersection hit;	
			hit.point = p + t*d;
			if (isInside(hit.point, halfSideLength)) {
				hit.point = transform.apply(hit.point);
				hit.normal = Normal(0, -1, 0);				
				if (hit.normal.dot(ray.direction) > 0) {
					hit.normal = -hit.normal;
				}
				hit.material = material;
				hit.distance = (hit.point - ray.point).norm();
				result.push_back(hit);
			}			
		}		
	}

	if (fabs(d(2)) > epsilon){
		t=(halfSideLength-p(2))/d(2);
		if (t>epsilon) {
			RayIntersection hit;	
			hit.point = p + t*d;
			if (isInside(hit.point, halfSideLength)) {
				hit.point = transform.apply(hit.point);
				hit.normal = Normal(0, 0, 1);				
				if (hit.normal.dot(ray.direction) > 0) {
					hit.normal = -hit.normal;
				}
				hit.material = material;
				hit.distance = (hit.point - ray.point).norm();
				result.push_back(hit);
			}			
		}

		t=(-halfSideLength-p(2))/d(2);
		if (t>epsilon) {
			RayIntersection hit;	
			hit.point = p + t*d;
			if (isInside(hit.point, halfSideLength)) {
				hit.point = transform.apply(hit.point);
				hit.normal = Normal(0, 0, -1);				
				if (hit.normal.dot(ray.direction) > 0) {
					hit.normal = -hit.normal;
				}
				hit.material = material;
				hit.distance = (hit.point - ray.point).norm();
				result.push_back(hit);
			}			
		}		
	}

	return result;
}