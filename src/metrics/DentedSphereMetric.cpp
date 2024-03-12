#include "DentedSphereMetric.hpp"

double DentedSphereMetric::g_theta_theta([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return squared(cos(theta)) + squared(sin(theta) * (3 * cos(2 * theta) - 1)) / 4;
}

double DentedSphereMetric::g_theta_phi([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return 0;
}

double DentedSphereMetric::g_phi_phi(double theta, [[maybe_unused]] double phi) {
	return squared(sin(theta));
}
