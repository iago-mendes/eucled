#include "OblateSpheroidMetric.hpp"

OblateSpheroidMetric::OblateSpheroidMetric() {
	parameters.push_back("1st Radius");
	parameters.push_back("2nd Radius");
}

OblateSpheroidMetric::OblateSpheroidMetric(double a, double b) {
	this->a = a;
	this->b = b;
}

void OblateSpheroidMetric::setParameter(std::string parameter, double value) {
	if (parameter == "1st Radius") {
		this->a = a;
	} else if (parameter == "2nd Radius") {
		this->b = b;
	}
}

double OblateSpheroidMetric::g_theta_theta([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return squared(a);
}

double OblateSpheroidMetric::g_theta_phi([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return 0;
}

double OblateSpheroidMetric::g_phi_phi(double theta, [[maybe_unused]] double phi) {
	return squared(b) * squared(sin(theta));
}
