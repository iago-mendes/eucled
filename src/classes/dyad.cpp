#include "dyad.h"
using namespace std;

RoundSphereDyad::RoundSphereDyad(double radius) {
	R = radius;
}

double RoundSphereDyad::e_theta_x(double theta, double phi) {
	return R * cos(theta) * cos(phi);
}

double RoundSphereDyad::e_theta_y(double theta, double phi) {
	return R * cos(theta) * sin(phi);
}

double RoundSphereDyad::e_theta_z(double theta, double phi) {
	return - R * sin(theta);
}

double RoundSphereDyad::e_phi_x(double theta, double phi) {
	return - R * sin(theta) * sin(phi);
}

double RoundSphereDyad::e_phi_y(double theta, double phi) {
	return R * sin(theta) * cos(phi);
}

double RoundSphereDyad::e_phi_z(double theta, double phi) {
	return 0;
}
