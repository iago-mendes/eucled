#include "main.h"
using namespace std;

Grid grid;

OblateSpheroidDyad dyad;
// Semi axes
double a = 1;
double b = 2;

Grid3DFunction e_theta;
Grid3DFunction e_phi;

double get_e_theta(int i, int j, char coordinate) {
	double theta = grid.theta(i);
	double phi = grid.phi(j);

	switch (coordinate) {
		case 'x':
			return dyad.e_theta_x(theta, phi);
			break;
		
		case 'y':
			return dyad.e_theta_y(theta, phi);
			break;

		case 'z':
			return dyad.e_theta_z(theta, phi);
			break;
		
		default:
			return -1;
			break;
	}
}

double get_e_phi(int i, int j, char coordinate) {
	double theta = grid.theta(i);
	double phi = grid.phi(j);

	switch (coordinate) {
		case 'x':
			return dyad.e_phi_x(theta, phi);
			break;
		
		case 'y':
			return dyad.e_phi_y(theta, phi);
			break;

		case 'z':
			return dyad.e_phi_z(theta, phi);
			break;
		
		default:
			return -1;
			break;
	}
}

// Returns residual.
double find_solution(int N) {
	Grid grid_(N, N);
	grid = grid_;

	// Reset solutions
	Grid3DFunction e_theta_(grid, get_e_theta);
	e_theta = e_theta_;
	Grid3DFunction e_phi_(grid, get_e_phi);
	e_phi = e_phi_;

	return run_relaxation(&e_theta, &e_phi, get_commutator_rms);
}

int main() {
	OblateSpheroidDyad dyad_(a, b);
	dyad = dyad_;

	find_solution(20);
}
