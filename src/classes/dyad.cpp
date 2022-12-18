#include "dyad.h"
using namespace std;

shared_ptr<Grid3DFunction> Dyad::get_theta_function(Grid grid) {
	shared_ptr<Grid3DFunction> function = make_shared<Grid3DFunction>(grid);

	for (int i = 0; i < grid.N_theta; i++) {
		double theta = grid.theta(i);

		for (int j = 0; j < grid.N_phi; j++) {
			double phi = grid.phi(j);

			function->x_values[i][j] = this->e_theta_x(theta, phi);
			function->y_values[i][j] = this->e_theta_y(theta, phi);
			function->z_values[i][j] = this->e_theta_z(theta, phi);
		}
	}

	return function;
}

shared_ptr<Grid3DFunction> Dyad::get_phi_function(Grid grid) {
	shared_ptr<Grid3DFunction> function = make_shared<Grid3DFunction>(grid);

	for (int i = 0; i < grid.N_theta; i++) {
		double theta = grid.theta(i);

		for (int j = 0; j < grid.N_phi; j++) {
			double phi = grid.phi(j);

			function->x_values[i][j] = this->e_phi_x(theta, phi);
			function->y_values[i][j] = this->e_phi_y(theta, phi);
			function->z_values[i][j] = this->e_phi_z(theta, phi);
		}
	}

	return function;
}

// Round Sphere

RoundSphereDyad::RoundSphereDyad(double radius) {
	R = radius;
}

double RoundSphereDyad::e_theta_x(double theta, double phi) {
	return R * cos(theta) * cos(phi);
}

double RoundSphereDyad::e_theta_y(double theta, double phi) {
	return R * cos(theta) * sin(phi);
}

double RoundSphereDyad::e_theta_z(double theta, [[maybe_unused]] double phi) {
	return - R * sin(theta);
}

double RoundSphereDyad::e_phi_x(double theta, double phi) {
	return - R * sin(theta) * sin(phi);
}

double RoundSphereDyad::e_phi_y(double theta, double phi) {
	return R * sin(theta) * cos(phi);
}

double RoundSphereDyad::e_phi_z([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return 0;
}

// Oblate Spheroid

OblateSpheroidDyad::OblateSpheroidDyad(double a, double b) {
	this->a = a;
	this->b = b;
}

double OblateSpheroidDyad::e_theta_x(double theta, double phi) {
	return a * cos(theta) * cos(phi);
}

double OblateSpheroidDyad::e_theta_y(double theta, double phi) {
	return a * cos(theta) * sin(phi);
}

double OblateSpheroidDyad::e_theta_z(double theta, [[maybe_unused]] double phi) {
	return - a * sin(theta);
}

double OblateSpheroidDyad::e_phi_x(double theta, double phi) {
	return - b * sin(theta) * sin(phi);
}

double OblateSpheroidDyad::e_phi_y(double theta, double phi) {
	return b * sin(theta) * cos(phi);
}

double OblateSpheroidDyad::e_phi_z([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return 0;
}

// Rotated Sphere

RotatedSphereDyad::RotatedSphereDyad(double (*rotator)(double theta, double phi)) {
	this->rotator = rotator;

	RoundSphereDyad round_dyad_(1);
	round_dyad = round_dyad_;
}

double RotatedSphereDyad::e_theta_x(double theta, double phi) {
	double upsilon = rotator(theta, phi);
	return cos(upsilon) * round_dyad.e_theta_x(theta, phi) + sin(upsilon) * round_dyad.e_theta_z(theta, phi);
}

double RotatedSphereDyad::e_theta_y(double theta, double phi) {
	return round_dyad.e_theta_y(theta, phi);
}

double RotatedSphereDyad::e_theta_z(double theta, double phi) {
	double upsilon = rotator(theta, phi);
	return cos(upsilon) * round_dyad.e_theta_z(theta, phi) - sin(upsilon) * round_dyad.e_theta_z(theta, phi);
}

double RotatedSphereDyad::e_phi_x(double theta, double phi) {
	double upsilon = rotator(theta, phi);
	return cos(upsilon) * round_dyad.e_phi_x(theta, phi) + sin(upsilon) * round_dyad.e_phi_z(theta, phi);
}

double RotatedSphereDyad::e_phi_y(double theta, double phi) {
	return round_dyad.e_phi_y(theta, phi);
}

double RotatedSphereDyad::e_phi_z(double theta, double phi) {
	double upsilon = rotator(theta, phi);
	return cos(upsilon) * round_dyad.e_phi_z(theta, phi) - sin(upsilon) * round_dyad.e_phi_z(theta, phi);
}

// Horizon

HorizonDyad::HorizonDyad(double chi) {
	this->chi = chi;
}

double HorizonDyad::rho() {
	return 1 + sqrt(1 - squared(this->chi));
}

double HorizonDyad::alpha(double theta) {
	return sqrt(2 * this->rho() - squared(this->chi) * squared(sin(theta)));
}

double HorizonDyad::beta(double theta) {
	return 2 * this->rho() / this->alpha(theta);
}

double HorizonDyad::e_theta_x(double theta, double phi) {
	return this->alpha(theta) * cos(theta) * cos(phi);
}

double HorizonDyad::e_theta_y(double theta, double phi) {
	return this->alpha(theta) * cos(theta) * sin(phi);
}

double HorizonDyad::e_theta_z(double theta, [[maybe_unused]] double phi) {
	return - this->alpha(theta) * sin(theta);
}

double HorizonDyad::e_phi_x(double theta, double phi) {
	return - this->beta(theta) * sin(theta) * sin(phi);
}

double HorizonDyad::e_phi_y(double theta, double phi) {
	return this->beta(theta) * sin(theta) * cos(phi);
}

double HorizonDyad::e_phi_z([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return 0;
}
