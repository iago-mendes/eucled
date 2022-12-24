#include "main.h"
using namespace std;

Grid grid;

// Rotation of black hole horizon.
double chi = 1;
// HorizonDyad dyad(chi);
RoundSphereDyad dyad(chi);

shared_ptr<Grid3DFunction> e_theta(nullptr);
shared_ptr<Grid3DFunction> e_phi(nullptr);

// Returns residual.
double find_solution(int N_theta, int N_phi) {
	Grid grid_(N_theta, N_phi);
	grid = grid_;

	// Reset solutions
	e_theta = dyad.get_theta_function(grid);
	e_phi = dyad.get_phi_function(grid);
	
	return run_relaxation(e_theta, e_phi, get_commutator_rms);
}

int main() {
	// Temporarily run for only one resolution.
	int N = 20;
	find_solution(N, 4*N);
	return 0;

	ofstream convergence_output("./assets/convergence.csv");
	for (int N = 10; N <= 40; N += 10) {
		printf("\n(Nx, Ny) = (%d, %d)\n", N, 4*N);

		double residual = find_solution(N, 4*N);

		convergence_output << N << "," << residual << endl;
	}
}
