#include "ZPeanutMetric.hpp"

ZPeanutMetric::ZPeanutMetric() {
	parameters.push_back("Separation");
}

ZPeanutMetric::ZPeanutMetric(double a) {
	this->a = a;
}

void ZPeanutMetric::set_parameter(std::string parameter, std::string value) {
	if (parameter == "Separation") {
		this->a = stod(value);
	}
}

double ZPeanutMetric::g_theta_theta([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return 1;
}

double ZPeanutMetric::g_theta_phi([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return 0;
}

double ZPeanutMetric::g_phi_phi(double theta, [[maybe_unused]] double phi) {
	return squared((1 - a * squared(sin(theta))) * sin(theta));
}
