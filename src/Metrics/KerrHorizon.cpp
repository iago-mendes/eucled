// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "./KerrHorizon.hpp"

#include <math.h>

#include "../Utilities/math.hpp"

namespace Metrics {

KerrHorizon::KerrHorizon(double chi) {
	this->chi = chi;
}

double KerrHorizon::rho() {
	return 1 + sqrt(1 - sqr(this->chi));
}

double KerrHorizon::sigma(double theta) {
	return 2 * this->rho() - sqr(this->chi) * sqr(sin(theta));
}

double KerrHorizon::g_theta_theta(double theta, [[maybe_unused]] double phi) {
	return this->sigma(theta);
}

double KerrHorizon::g_theta_phi([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return 0;
}

double KerrHorizon::g_phi_phi(double theta, [[maybe_unused]] double phi) {
	return (4 * sqr(this->rho()) / this->sigma(theta)) * sqr(sin(theta));
}

} // namespace Metrics
