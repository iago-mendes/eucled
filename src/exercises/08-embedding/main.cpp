#include "main.h"
using namespace std;

#include "main.h"
using namespace std;

// Rotation of black hole horizon.
double chi = 0.5;

Grid grid;

void find_solution(int N_theta, int N_phi) {
	Grid grid_(N_theta, N_phi);
	grid = grid_;

	shared_ptr<HorizonMetric> metric = make_shared<HorizonMetric>(chi);
	// shared_ptr<RoundSphereMetric> metric = make_shared<RoundSphereMetric>(chi);

	shared_ptr<Grid3DFunction> e_theta = make_shared<Grid3DFunction>(grid);
	shared_ptr<Grid3DFunction> e_phi = make_shared<Grid3DFunction>(grid);
	shared_ptr<Grid3DFunction> embedding = make_shared<Grid3DFunction>(grid);

	run_factorization(metric, e_theta, e_phi);
	run_relaxation(e_theta, e_phi, get_commutator_rms);
	run_integration(e_theta, e_phi, embedding);

	ofstream embedding_output("./assets/embedding.csv");
	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			embedding_output << embedding->x_values[i][j] << "," << embedding->y_values[i][j] << "," << embedding->z_values[i][j] << endl;
		}
	}

	printf("\n\nDone!\n");
}

int main() {
	for (int N = 10; N <= 100; N += 20) {
		printf("\n(Nx, Ny) = (%d, %d)\n", N, N);

		find_solution(N, N);
	}
}
