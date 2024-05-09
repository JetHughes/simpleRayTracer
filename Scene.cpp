#include "Scene.h"

#include "Colour.h"
#include "ImageDisplay.h"
#include "utility.h"

// For demos

Scene::Scene() : backgroundColour(0,0,0), ambientLight(0,0,0), maxRayDepth(3), renderWidth(800), renderHeight(600), filename("render.png"), camera_(), objects_(), lights_() {

}

Scene::~Scene() {

}

void Scene::render() const {
	ImageDisplay display("Render", renderWidth, renderHeight);

	const double w = double(renderWidth);
	const double h = double(renderHeight);

	/*************************************************************
	 * This casts a single ray per pixel.                        *
	 * Code changes here to cast multiple rays for anti-aliasing *
	 * Depending on how you do this, you may wish to add options *
	 * to the Scene object. If you do this you will also need to *
	 * update the SceneParser routines, probably parseSceneBlock *
	 *************************************************************/

	for (unsigned int v = 0; v < renderHeight; ++v) {
		for (unsigned int u = 0; u < renderWidth; ++u) {
			double cu = -1 + (u + 0.5)*(2.0 / w);
			double cv = -h/w + (v + 0.5)*(2.0 / w);
			Ray ray = camera_->castRay(cu, cv);
			display.set(u, v, computeColour(ray, maxRayDepth));
		}
		display.refresh();
	}

	display.save(filename);
	display.pause(5);
}

RayIntersection Scene::intersect(const Ray& ray) const {
	RayIntersection firstHit;
	firstHit.distance = infinity;
	std::vector<RayIntersection> hits;
	
	double delta = pow(epsilon, 0.25);

	for (const auto& obj: objects_) {
		for (const auto& hit: obj->intersect(ray)) {
			if (hit.distance > delta && hit.distance < firstHit.distance) {
				firstHit = hit;
			}
		}
	}
	return firstHit;
}

Colour Scene::computeColour(const Ray& ray, unsigned int rayDepth) const {
	RayIntersection hitPoint = intersect(ray);
	if (hitPoint.distance == infinity) {
		return backgroundColour;
	}

	Colour hitColour(0, 0, 0);
		
	/******************************************************************
	 * Code for better lighting, shadows, and reflections goes below. *
	 ******************************************************************/

	Normal N(hitPoint.normal/hitPoint.normal.norm()); // Unit normal
	Direction V(-ray.direction/ray.direction.norm()); // Unit view vector

	for (const auto & light : lights_) {

		// Compute the influence of this light on the appearance of the hit object.
		if (light->getDistanceToLight(hitPoint.point) < 0) {
			// An ambient light, ignore shadows and add appropriate colour
			hitColour += light->getIlluminationAt(hitPoint.point) * hitPoint.material.ambientColour;
		} else {
			// Not an ambient light
			Direction L(light->getLightDirection(hitPoint.point));
			L = L/L.norm(); // get unit vector
			
			// Shadows
			Ray shadowRay;
			shadowRay.point = hitPoint.point;
			shadowRay.direction = L;
			RayIntersection shadowHitPoint = intersect(shadowRay);

			if (shadowHitPoint.distance >= infinity) {
				Direction R(2* (N.dot(L)) * N - L);

				Colour Id(light->getIlluminationAt(hitPoint.point));
				Colour Kd(hitPoint.material.diffuseColour);
				Colour Ks(hitPoint.material.specularColour);
				double alpha = hitPoint.material.specularExponent;

				// Diffuse
				hitColour += Id * Kd * (N.dot(L));

				// Specular
				hitColour += Id * Ks * pow(R.dot(V), alpha);
			}
		}
	}

	//// Code for reflections goes here
	//if (rayDepth > 0 && hitPoint.material.mirrorColour.red > 0 ||
	//					hitPoint.material.mirrorColour.green > 0 ||
	//					hitPoint.material.mirrorColour.blue > 0) {
	//	Ray reflectedRay;
	//	reflectedRay.point = hitPoint.point;
	//	reflectedRay.direction = 2 * (N.dot(V)) * N - V;
	//	
	//	Colour mirrorCol = hitPoint.material.mirrorColour;
	//	hitColour = (Colour(1,1,1) - mirrorCol) * hitColour + mirrorCol * computeColour(reflectedRay, rayDepth - 1);
	//}


	hitColour.clip();
	return hitColour;
}

bool Scene::hasCamera() const {
	return bool(camera_);
}
