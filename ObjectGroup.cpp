#include "ObjectGroup.h"

std::vector<RayIntersection> ObjectGroup::intersect(const Ray& ray) const {
	std::vector<RayIntersection> result;

	Ray inverseRay = transform.applyInverse(ray);

	for (const auto& objPtr : objects) {
		std::vector<RayIntersection> hits = objPtr->intersect(inverseRay);
		result.insert(result.end(), hits.begin(), hits.end());
	}

	for (auto& hit : result) {
		hit.point = transform.apply(hit.point);
		hit.normal = transform.apply(hit.normal);
		if (hit.normal.dot(ray.direction) > 0) {
			hit.normal = -hit.normal;
		}
		hit.distance = (hit.point - ray.point).norm();
	}

	return result;
}

void ObjectGroup::addObject(std::shared_ptr<Object> objectPtr) {
	objects.push_back(objectPtr);
}