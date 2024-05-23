#include "CSG.h"

CSG::CSG(Operation operation) :
	operation(operation) { }


std::vector<RayIntersection> CSG::intersect(const Ray& ray) const {
	std::vector<RayIntersection> result;

	Ray inverseRay = transform.applyInverse(ray);

	double delta = pow(epsilon, 0.25);

	struct CSGHit
	{
		RayIntersection hit;
		uint8_t object;
	};
	
	
	std::vector<CSGHit> csgHits;
	std::vector<RayIntersection> leftHits;
	std::vector<RayIntersection> rightHits;

	// Cast ray through both objects
	for (const auto& hit: left->intersect(inverseRay)) {
		if (hit.distance > delta && hit.distance) {
			csgHits.push_back(CSGHit{hit, 0});
			leftHits.push_back(hit);
		}
	}
	for (const auto& hit: right->intersect(inverseRay)) {
		if (hit.distance > delta && hit.distance) {
			csgHits.push_back(CSGHit{hit, 1});
			rightHits.push_back(hit);
		}
	}

	// Sort hits by distance
	std::sort(csgHits.begin(), csgHits.end(), [](const CSGHit& a, const CSGHit& b) {
		return a.hit.distance < b.hit.distance;
	});

	std::sort(leftHits.begin(), leftHits.end(), [](const RayIntersection& a, const RayIntersection& b) {
		return a.distance < b.distance;
	});

	std::sort(rightHits.begin(), rightHits.end(), [](const RayIntersection& a, const RayIntersection& b) {
		return a.distance < b.distance;
	});

	if (csgHits.size() == 0)
	{
		return result;
	}	

	if(operation == UNION) {
		// Union return all hits
		for (const auto& csgHit: csgHits) {
			result.push_back(csgHit.hit);
		}		
	}
	else if(operation == INTERSECTION) {
		// Intersection
		bool inLeft = false;
		bool inRight = false;
		for (auto& csgHit: csgHits) {
			if(csgHit.object == 0) {
				inLeft = !inLeft;
			}
			else {
				inRight = !inRight;
			}

			if(inLeft && inRight) {
				result.push_back(csgHit.hit);
			}
		}
	}
	else if(operation == DIFFERENCE) {
		// Difference
		bool inLeft = false;
		bool inRight = false;
		for (auto& csgHit: csgHits) {
			if(csgHit.object == 0) {
				inLeft = !inLeft;
			}
			else {
				inRight = !inRight;
			}

			if(inLeft && !inRight) {
				result.push_back(csgHit.hit);
			}
		}
	}

	for (auto& hit: result) {
		// hit.material = material;
		hit.point = transform.apply(hit.point);
		hit.normal = transform.apply(hit.normal);
		hit.normal = hit.normal / hit.normal.norm();
		if (hit.normal.dot(ray.direction) > 0) {
			hit.normal = -hit.normal;
		}
		hit.distance = (hit.point - ray.point).norm();
	}

	return result;
}