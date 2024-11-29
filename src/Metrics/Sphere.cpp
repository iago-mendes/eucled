// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "./Sphere.hpp"

#include <math.h>

#include "../Utilities/math.hpp"

namespace Metrics {

Sphere::Sphere(double R) {
	this->R = R;
}

double Sphere::g_theta_theta(double theta, double phi) {
	return squared(R);
}

double Sphere::g_theta_phi(double theta, double phi) {
	return 0;
}

double Sphere::g_phi_phi(double theta, double phi) {
	return squared(R) * squared(sin(theta));
}

} // namespace Metrics
