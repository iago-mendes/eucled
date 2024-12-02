// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "./Mesh.hpp"

#include <math.h>
#include <stdexcept>

Mesh::Mesh(int N_theta, int N_phi) {
	if (N_theta < 10 or N_phi < 10) {
		throw std::logic_error("The minimum mesh size is 10x10!");
	}

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
