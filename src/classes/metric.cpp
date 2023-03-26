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

// Ellipsoid

EllipsoidMetric::EllipsoidMetric(double a, double b, double c) {
	this->a = a;
	this->b = b;
	this->c = c;
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

// Peanut

PeanutMetric::PeanutMetric(double a) {
	this->a = a;
}

double PeanutMetric::g_theta_theta([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return 1;
}

double PeanutMetric::g_theta_phi([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return 0;
}

double PeanutMetric::g_phi_phi(double theta, [[maybe_unused]] double phi) {
	return squared((1 - a * squared(sin(theta))) * sin(theta));
}

// Dented Sphere

double DentedSphereMetric::g_theta_theta([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return squared(cos(theta)) + squared(sin(theta) * (3 * cos(2 * theta) - 1)) / 4;
}

double DentedSphereMetric::g_theta_phi([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return 0;
}

double DentedSphereMetric::g_phi_phi(double theta, [[maybe_unused]] double phi) {
	return squared(sin(theta));
}

// Numerical

NumericalMetric::NumericalMetric() {
	int N_theta, N_phi;
	cin >> N_theta >> N_phi;

	Grid grid_(N_theta, N_phi);
	this->grid = grid_;

	for (int i = 0; i < N_theta; i++) {
		vector<double> phi_values[3];

		for (int j = 0; j < N_phi; j++) {
			double point[3];
			cin >> point[0] >> point[1] >> point[2];

			phi_values[0].push_back(point[0]);
			phi_values[1].push_back(point[1]);
			phi_values[2].push_back(point[2]);
		}

		data[0].push_back(phi_values[0]);
		data[1].push_back(phi_values[1]);
		data[2].push_back(phi_values[2]);
	}
}

double NumericalMetric::g_theta_theta([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	int i = this->grid.i(theta);
	int j = this->grid.j(phi);

	return this->data[0][i][j];
}

double NumericalMetric::g_theta_phi([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	int i = this->grid.i(theta);
	int j = this->grid.j(phi);
	
	return this->data[2][i][j];
}

double NumericalMetric::g_phi_phi(double theta, [[maybe_unused]] double phi) {
	int i = this->grid.i(theta);
	int j = this->grid.j(phi);
	
	return this->data[1][i][j];
}

// Non-axisymmetric z-Peanut

NonaxisymmetricZPeanutMetric::NonaxisymmetricZPeanutMetric(double r0, double b, double e) {
	this->r0 = r0;
	this->b = b;
	this->e = e;
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
