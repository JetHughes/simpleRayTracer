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
	const Point& D = inverseRay.point;
	const Direction& E = inverseRay.direction;

	// Adapted from http://cosinekitty.com/raytrace/chapter13_torus.html

	double A = radius_;
	double B = radius_ / 2;

	double Dx = D(0);
	double Dy = D(1);
	double Dz = D(2);

	double Ex = E(0);
	double Ey = E(1);
	double Ez = E(2);

	// formula: (x^2 + y^2 + z^2 + A^2 - B^2)^2 - 4A^2(x^2 + y^2) = 0

	double G = 4.0*(A*A)*(Ex*Ex + Ey*Ey);
	double H = 8.0*(A*A)*(Dx*Ex + Dy*Ey);
	double I = 4.0*(A*A)*(Dx*Dx + Dy*Dy);
	double J = Ex*Ex + Ey*Ey + Ez*Ez;
	double K = 2.0*(D.dot(E));
	double L = Dx*Dx + Dy*Dy + Dz*Dz + A*A - B*B;

	// new formula: (Ju^2 + Ku + L)^2 = Gu^2 + Hu + I
	// coefficients: J^2u^4 + 2JKu^3 + (2JL + K^2 - G)u^2 + (2KL-H)u + (L^2 - I) = 0

	double a = J*J;
	double b = 2.0*J*K;
	double c = 2.0*J*L + K*K - G;
	double d = 2.0*K*L - H;
	double e = L*L - I;

	std::vector<double> roots = solveQuartic(a, b, c, d, e);

	for (double t:roots)
	{
		// higher tolerance than usual
		double tolerance = 1.0e-4;
		if (t > tolerance) {
			Point P = D + t * E;

			double Px = P(0);
			double Py = P(1);
			double Pz = P(2);

			double alpha = 1.0 - A / sqrt(Px*Px + Py*Py);

			Normal N(alpha*Px, alpha*Py, Pz);

			RayIntersection hit;
			hit.point = transform.apply(P);
			hit.normal = transform.apply(N);
			hit.normal = hit.normal / hit.normal.norm();
			if (hit.normal.dot(ray.direction) > 0) {
				hit.normal = -hit.normal;
			}
			hit.distance = (hit.point - ray.point).norm();
			hit.material = material;
			result.push_back(hit);
		}
	}

	return result;
}
