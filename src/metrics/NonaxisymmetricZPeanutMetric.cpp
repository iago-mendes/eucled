#include "NonaxisymmetricZPeanutMetric.hpp"

NonaxisymmetricZPeanutMetric::NonaxisymmetricZPeanutMetric() {
	parameters.push_back("Radius");
	parameters.push_back("Separation");
	parameters.push_back("Non-axisymmetry");
}

NonaxisymmetricZPeanutMetric::NonaxisymmetricZPeanutMetric(double r0, double b, double e) {
	this->r0 = r0;
	this->b = b;
	this->e = e;
}

void NonaxisymmetricZPeanutMetric::set_parameter(std::string parameter, std::string value) {
	if (parameter == "Radius") {
		this->r0 = stod(value);
	} else if (parameter == "Separation") {
		this->b = stod(value);
	} else if (parameter == "Non-axisymmetry") {
		this->e = stod(value);
	}
}

double NonaxisymmetricZPeanutMetric::r(double theta, double phi) {
	return r0 - b * squared(sin(theta)) + e * cos(theta) * sin(theta) * cos(phi);
}

double NonaxisymmetricZPeanutMetric::partial_theta_r(double theta, double phi) {
	return - 2 * b * sin(theta) * cos(theta) - e * squared(sin(theta)) * cos(phi) + e * squared(cos(theta)) * cos(phi);
}

double NonaxisymmetricZPeanutMetric::partial_phi_r(double theta, double phi) {
	return  - e * cos(theta) * sin(theta) * sin(phi);
}

double NonaxisymmetricZPeanutMetric::g_theta_theta([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	double partial_theta_r = this->partial_theta_r(theta, phi);
	double r = this->r(theta, phi);

	return squared(partial_theta_r) + squared(r);
}

double NonaxisymmetricZPeanutMetric::g_theta_phi([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	double partial_theta_r = this->partial_theta_r(theta, phi);
	double partial_phi_r = this->partial_phi_r(theta, phi);

	return partial_theta_r * partial_phi_r;
}

double NonaxisymmetricZPeanutMetric::g_phi_phi(double theta, [[maybe_unused]] double phi) {
	double partial_phi_r = this->partial_phi_r(theta, phi);
	double r = this->r(theta, phi);

	return squared(partial_phi_r) + squared(r) * squared(sin(theta));
}
