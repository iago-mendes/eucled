#include "dyad.h"
using namespace std;

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
