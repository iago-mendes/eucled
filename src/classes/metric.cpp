#include "metric.h"
using namespace std;

// Round Sphere

RoundSphereMetric::RoundSphereMetric(double radius) {
	this->R = radius;
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

// Oblate Spheroid

OblateSpheroidMetric::OblateSpheroidMetric(double a, double b) {
	this->a = a;
	this->b = b;
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

// Horizon

HorizonMetric::HorizonMetric(double chi) {
	this->chi = chi;
}

double HorizonMetric::rho() {
	return 1 + sqrt(1 - squared(this->chi));
}

double HorizonMetric::sigma(double theta) {
	return 2 * this->rho() - squared(this->chi) * squared(sin(theta));
}

double HorizonMetric::g_theta_theta(double theta, [[maybe_unused]] double phi) {
	return this->sigma(theta);
}

double HorizonMetric::g_theta_phi([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return 0;
}

double HorizonMetric::g_phi_phi(double theta, [[maybe_unused]] double phi) {
	return (4 * squared(this->rho()) / this->sigma(theta)) * squared(sin(theta));
}
