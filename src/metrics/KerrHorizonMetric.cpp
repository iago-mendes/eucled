#include "KerrHorizonMetric.hpp"

KerrHorizonMetric::KerrHorizonMetric() {
	parameters.push_back("Spin");
}

KerrHorizonMetric::KerrHorizonMetric(double chi) {
	this->chi = chi;
}

void KerrHorizonMetric::set_parameter(std::string parameter, std::string value) {
	if (parameter == "Spin") {
		this->chi = stod(value);
	}
}

double KerrHorizonMetric::rho() {
	return 1 + sqrt(1 - squared(this->chi));
}

double KerrHorizonMetric::sigma(double theta) {
	return 2 * this->rho() - squared(this->chi) * squared(sin(theta));
}

double KerrHorizonMetric::g_theta_theta(double theta, [[maybe_unused]] double phi) {
	return this->sigma(theta);
}

double KerrHorizonMetric::g_theta_phi([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return 0;
}

double KerrHorizonMetric::g_phi_phi(double theta, [[maybe_unused]] double phi) {
	return (4 * squared(this->rho()) / this->sigma(theta)) * squared(sin(theta));
}
