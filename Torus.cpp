#include "Torus.h"

#include "utility.h"


Torus::Torus(double radius) : Object(), radius_(radius) {

	std::cout << "Radius " << radius_ << std::endl;
}

Torus::Torus(const Torus& torus) : Object(torus), radius_(torus.radius_) {

}

Torus::~Torus() {

}

Torus& Torus::operator=(const Torus& torus) {
	if (this != &torus) {
		Object::operator=(torus);
		radius_ = torus.radius_;
	}
	return *this;
}

std::vector<RayIntersection> Torus::intersect(const Ray& ray) const {
	std::vector<RayIntersection> result;
	
	/***********************************************
	 * Ray-Torus Intersection is not implemented.  *
	 * You need to implement it for the assignment.*
	 ***********************************************/

	// Note - the utility function solveQuartic will be useful here
	
	return result;
}
