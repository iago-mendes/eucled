#include "UniformGrid.hpp"

UniformGrid::UniformGrid(int N_theta, int N_phi) {
	this->N_theta = N_theta;
	this->N_phi = N_phi;

	this->delta_theta = M_PI / N_theta;
	this->delta_phi = 2. * M_PI / N_phi;
}

double UniformGrid::theta(int i) {
	return this->delta_theta * (i + 1./2.);
}

double UniformGrid::phi(int j) {
	return this->delta_phi * j;
}

int UniformGrid::i(double theta) {
	return (int) (theta / this->delta_theta - 1./2.);
}

int UniformGrid::j(double phi) {
	return (int) (phi / this->delta_phi);
}
