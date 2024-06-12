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

	double coneAngleRad = deg2rad(angle_);
	Direction toHit = (point - location_)/(point - location_).norm();
	double anglePointToLight = acos(toHit.dot(direction_/direction_.norm()));

	// If the point is outside the cone of light, illumination is zero
	if (anglePointToLight > coneAngleRad) return Colour(0, 0, 0);

	// Otherwise, calculate the illumination using the inverse square law
	double distance = (location_ - point).norm();
	if (distance < epsilon) distance = epsilon;
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
