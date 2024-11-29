#include "./Mesh.hpp"

#include "math.h"

Mesh::Mesh(int N_theta, int N_phi) {
	this->N_theta = N_theta;
	this->N_phi = N_phi;

	delta_theta = M_PI / N_theta;
	delta_phi = 2 * M_PI / N_phi;
}

double Mesh::theta(int i) {
	return this->delta_theta * (i + 1. / 2.);
}

double Mesh::phi(int j) {
	return this->delta_phi * j;
}

int Mesh::i(double theta) {
	return (int) (theta / this->delta_theta - 1. / 2.);
}

int Mesh::j(double phi) {
	return (int) (phi / this->delta_phi);
}
