#include "RoundSphereMetric.hpp"

RoundSphereMetric::RoundSphereMetric() {
	parameters.push_back("Radius");
}

RoundSphereMetric::RoundSphereMetric(double radius) {
	this->R = radius;
}

void RoundSphereMetric::set_parameter(std::string parameter, std::string value) {
	if (parameter == "Radius") {
		this->R = stod(value);
	}
}

double RoundSphereMetric::g_theta_theta([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return squared(R);
}

double RoundSphereMetric::g_theta_phi([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return 0;
}

double RoundSphereMetric::g_phi_phi(double theta, [[maybe_unused]] double phi) {
	return squared(R) * squared(sin(theta));
}
