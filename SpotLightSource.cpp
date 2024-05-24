#include "SpotLightSource.h"

#include "utility.h"

SpotLightSource::SpotLightSource(const Colour& colour, const Point& location, const Direction& direction, double angle) :
	LightSource(colour), location_(location), direction_(direction), angle_(angle) {

}

SpotLightSource::SpotLightSource(const SpotLightSource& lightSource) :
LightSource(lightSource),  location_(lightSource.location_), direction_(lightSource.direction_), angle_(lightSource.angle_) {

}

SpotLightSource::~SpotLightSource() {

}

SpotLightSource& SpotLightSource::operator=(const SpotLightSource& lightSource) {
	if (this != &lightSource) {
		LightSource::operator=(lightSource);
		location_ = lightSource.location_;
		direction_ = lightSource.direction_;
		angle_ = lightSource.angle_;
	}
	return *this;
}

Colour SpotLightSource::getIlluminationAt(const Point& point) const {
	// made with reference to https://github.com/GriffinSchneider/raytracer/blob/master/src/SpotLight.java
	double angleRad = deg2rad(angle_);
	double distance = (location_ - point).norm();
	if (distance < epsilon) distance = epsilon;
	Direction toHit = (point - location_)/(point - location_).norm();

	double angle = acos(toHit.dot(direction_/direction_.norm()));

	if (angle > angleRad) return Colour(0, 0, 0);

	return (1.0 / (distance*distance)) * colour_;
}

double SpotLightSource::getDistanceToLight(const Point& point) const {
	return (location_ - point).norm();
}

Direction SpotLightSource::getLightDirection(const Point& point) const {
	Direction lightDirection;

	return point - location_;

	return lightDirection;
}
