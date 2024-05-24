#include "CSG.h"

CSG::CSG(Operation operation) :
	operation(operation) { }


std::vector<RayIntersection> CSG::intersect(const Ray& ray) const {
	std::vector<RayIntersection> result;

	Ray inverseRay = transform.applyInverse(ray);

	struct CSGHit
	{
		RayIntersection hit;
		uint8_t object;
	};	
	
	std::vector<CSGHit> csgHits;

	// Cast ray through both objects
	for (const auto& hit: left->intersect(inverseRay)) {
		csgHits.push_back(CSGHit{hit, 0});
	}
	for (const auto& hit: right->intersect(inverseRay)) {
		csgHits.push_back(CSGHit{hit, 1});
	}

	// Sort hits by distance
	std::sort(csgHits.begin(), csgHits.end(), [](const CSGHit& a, const CSGHit& b) {
		return a.hit.distance < b.hit.distance;
	});

	if (csgHits.size() == 0)
	{
		return result;
	}	

	if(operation == UNION) {
		// Union return first hits
		result.push_back(csgHits[0].hit);
	}
	// Acts similar to a state machine. Keep track of which objects you are inside 
	// of and keep hits when the condition is satisfied
	else if(operation == INTERSECTION) {
		// Intersection. In Left and in Right
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
		// Difference. In Left and not in Right
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