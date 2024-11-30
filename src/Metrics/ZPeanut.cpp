// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "./ZPeanut.hpp"

#include <math.h>

#include "../Utilities/math.hpp"

namespace Metrics {

ZPeanut::ZPeanut(double r0, double b, double e) {
	this->r0 = r0;
	this->b = b;
	this->e = e;
}

double ZPeanut::r(double theta, double phi) {
	return r0 - b * sqr(sin(theta)) + e * cos(theta) * sin(theta) * cos(phi);
}

double ZPeanut::partial_theta_r(double theta, double phi) {
	return - 2 * b * sin(theta) * cos(theta) - e * sqr(sin(theta)) * cos(phi) + e * sqr(cos(theta)) * cos(phi);
}

double ZPeanut::partial_phi_r(double theta, double phi) {
	return  - e * cos(theta) * sin(theta) * sin(phi);
}

double ZPeanut::g_theta_theta(double theta, double phi) {
	double partial_theta_r = this->partial_theta_r(theta, phi);
	double r = this->r(theta, phi);

	return sqr(partial_theta_r) + sqr(r);
}

double ZPeanut::g_theta_phi(double theta, double phi) {
	double partial_theta_r = this->partial_theta_r(theta, phi);
	double partial_phi_r = this->partial_phi_r(theta, phi);

	return partial_theta_r * partial_phi_r;
}

double ZPeanut::g_phi_phi(double theta, double phi) {
	double partial_phi_r = this->partial_phi_r(theta, phi);
	double r = this->r(theta, phi);

	return sqr(partial_phi_r) + sqr(r) * sqr(sin(theta));
}

} // namespace Metrics
