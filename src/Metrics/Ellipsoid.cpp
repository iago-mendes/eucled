// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "./Ellipsoid.hpp"

#include <math.h>

#include "../Utilities/math.hpp"

namespace Metrics {

Ellipsoid::Ellipsoid(double a, double b, double c) {
	this->a = a;
	this->b = b;
	this->c = c;
}

double Ellipsoid::g_theta_theta(double theta, double phi) {
	return (sqr(a * cos(phi)) + sqr(b * sin(phi))) * sqr(cos(theta)) + sqr(c * sin(theta));
}

double Ellipsoid::g_theta_phi(double theta, double phi) {
	return (sqr(b) - sqr(a)) * cos(theta) * sin(theta) * cos(phi) * sin(phi);
}

double Ellipsoid::g_phi_phi(double theta, double phi) {
	return (sqr(b * cos(phi)) + sqr(a * sin(phi))) * sqr(sin(theta));
}

} // namespace Metrics
