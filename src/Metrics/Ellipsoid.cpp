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
	return (squared(a * cos(phi)) + squared(b * sin(phi))) * squared(cos(theta)) + squared(c * sin(theta));
}

double Ellipsoid::g_theta_phi(double theta, double phi) {
	return (squared(b) - squared(a)) * cos(theta) * sin(theta) * cos(phi) * sin(phi);
}

double Ellipsoid::g_phi_phi(double theta, double phi) {
	return (squared(b * cos(phi)) + squared(a * sin(phi))) * squared(sin(theta));
}

} // namespace Metrics
