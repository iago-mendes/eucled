#include "main.h"
using namespace std;

Grid grid;

OblateSpheroidDyad dyad;
// Semi axes
double a = 1;
double b = 2;

shared_ptr<Grid3DFunction> e_theta(nullptr);
shared_ptr<Grid3DFunction> e_phi(nullptr);

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
double find_solution(int N_theta, int N_phi) {
	Grid grid_(N_theta, N_phi);
	grid = grid_;

	// Reset solutions
	e_theta = make_shared<Grid3DFunction>(grid, get_e_theta);
	e_phi = make_shared<Grid3DFunction>(grid, get_e_phi);

	return run_relaxation(e_theta, e_phi, get_commutator_rms);
}

int main() {
	OblateSpheroidDyad dyad_(a, b);
	dyad = dyad_;

	// Temporarily run for only one resolution.
	find_solution(20, 80);
	return 0;

	ofstream convergence_output("./assets/convergence.csv");
	for (int N = 10; N <= 100; N += 10) {
		printf("\nN = %d\n", N);

		double residual = find_solution(N, N);

		convergence_output << N << "," << residual << endl;
	}
}
