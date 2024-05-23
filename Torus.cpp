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
	
	Ray inverseRay = transform.applyInverse(ray);
	const Point& p = inverseRay.point;
	const Direction& d = inverseRay.direction;

	double R = radius_;
	double r = 0.5;

	float po = 1.0;
	float Ra2 = R * R;
	float ra2 = r * r;
	float m = p.dot(p);
	float n = d.dot(p); //opposite?
	float k = (m + Ra2 - ra2) / 2.0;
	float k3 = n;
	float k2 = n*n + Ra2*d(2)*d(2) + k;
	float k1 = n*k - Ra2*p(2)*d(2);
	float k0 = k*k - Ra2*p(2)*p(2) - Ra2*ra2;

	if (abs(k3 * (k3 * k3 - k2) + k1) < 0.01) {
		po = -1.0;
		float tmp = k1; k1=k3; k3 = tmp;
		k0 = 1.0 / k0;
		k1 = k1 * k0;
		k2 = k2 * k0;
		k3 = k3 * k0;
	}

	std::vector<double> roots = solveQuartic(k, k0, k1, k2, k3);

	for (double t:roots)
	{
		if (t > epsilon) {
			RayIntersection hit;
			Point hitPoint = p + t * d;
			hit.point = transform.apply(hitPoint);
			hit.normal = transform.apply(Point((p*(p.dot(p) - r*r - R*r*(Point(1.0, 1.0, -1.0))))));
			hit.normal = hit.normal / hit.normal.norm();
			if (hit.normal.dot(ray.direction) > 0) {
				hit.normal = -hit.normal;
			}
			hit.distance = (hit.point - ray.point).norm();
			hit.material = material;
			result.push_back(hit);
		}
	}


	//float c2 = k2*2.0 - 3.0*k3*k3;
	//float c1 = k3*(k3*k3-k2) + k1;
	//float c0 = k3*(k3*(c2+2.0*k2)-8.0*k1)+4.0*k0;
	//c2 /= 3.0;
	//c1 *= 2.0;
	//c0 /= 3.0;
	//float Q = c2*c2 + c0;
	//float RR = c2*c2*c2 - 3.0*c2*c0 + c1*c1;
	//float h = R*R - Q*Q*Q;
	//float t = 1e20;
	//if (h >= 0.0)
	//{
	//	h = sqrt(h);
	//	float v = sign(RR+h)*pow(abs(RR+h), 1.0/3.0);
	//	float u = sign(RR-h)*pow(abs(RR-h), 1.0/3.0);
	//	float sx = (v + u) + 4.0 * c2;
	//	float sy = (v - u) * sqrt(3.0);
	//	float y = sqrt(0.5*(sqrt(sx*sx+sy*sy)+sx));
	//	float x = 0.5 * sy / y;
	//	float r = 2.0 * c1 / (x * x + y * y);
	//	float t1 = x - r - k3; t1 = (po < 0.0) ? 2.0 / t1 : t1;
	//	float t2 = -x - r - k3; t2 = (po < 0.0) ? 2.0 / t2 : t2;
	//	if (t1 > 0.0) t = t1;
	//	if (t2 > 0.0) t = fmin(t, t2);
	//}
	//else {
	//	float sQ = sqrt(Q);
	//	float w = sQ * cos(acos(-R / (sQ * Q)) / 3.0); 
	//	float d2 = -(w + c2); if (d2 < 0.0) return result;
	//	float d1 = sqrt(d2);
	//	float h1 = sqrt(w - 2.0 * c2 + c1 / d1);
	//	float h2 = sqrt(w - 2.0 * c2 - c1 / d1);
	//	float t1 = -d1 - h1 - k3; t1 = (po < 0.0) ? 2.0 / t1 : t1;
	//	float t2 = -d1 + h1 - k3; t2 = (po < 0.0) ? 2.0 / t2 : t2;
	//	float t3 = d1 - h2 - k3; t3 = (po < 0.0) ? 2.0 / t3 : t3;
	//	float t4 = d1 + h2 - k3; t4 = (po < 0.0) ? 2.0 / t4 : t4;
	//	float t = 1e20;
	//	if (t1 > 0.0) t = t1;
	//	if (t2 > 0.0) t = fmin(t, t2);
	//	if (t3 > 0.0) t = fmin(t, t3);
	//	if (t4 > 0.0) t = fmin(t, t4);
	//}

	//RayIntersection hit;
	//Point hitPoint = p + t * d;
	//hit.point = transform.apply(hitPoint);
	//hit.normal = transform.apply(Point((p*(p.dot(p) - r*r - R*r*(Point(1.0, 1.0, -1.0))))));
	//hit.normal = hit.normal / hit.normal.norm();
	//if (hit.normal.dot(ray.direction) > 0) {
	//	hit.normal = -hit.normal;
	//}
	//hit.distance = (hit.point - ray.point).norm();
	//hit.material = material;	
	//return result;
}
