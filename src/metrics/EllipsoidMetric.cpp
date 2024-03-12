#include "EllipsoidMetric.hpp"

EllipsoidMetric::EllipsoidMetric() {
	parameters.push_back("X Semi-axis");
	parameters.push_back("Y Semi-axis");
	parameters.push_back("Z Semi-axis");
}

EllipsoidMetric::EllipsoidMetric(double a, double b, double c) {
	this->a = a;
	this->b = b;
	this->c = c;
}

void EllipsoidMetric::set_parameter(std::string parameter, std::string value) {
	if (parameter == "X Semi-axis") {
		this->a = stod(value);
	} else if (parameter == "Y Semi-axis") {
		this->b = stod(value);
	} else if (parameter == "Z Semi-axis") {
		this->c = stod(value);
	}
}

double EllipsoidMetric::g_theta_theta(double theta, double phi) {
	return (squared(a * cos(phi)) + squared(b * sin(phi))) * squared(cos(theta)) + squared(c * sin(theta));
}

double EllipsoidMetric::g_theta_phi(double theta, double phi) {
	return (squared(b) - squared(a)) * cos(theta) * sin(theta) * cos(phi) * sin(phi);
}

double EllipsoidMetric::g_phi_phi(double theta, double phi) {
	return (squared(b * cos(phi)) + squared(a * sin(phi))) * squared(sin(theta));
}
