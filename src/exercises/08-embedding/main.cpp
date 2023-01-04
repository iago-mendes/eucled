#include "main.h"
using namespace std;

#include "main.h"
using namespace std;

// Rotation of black hole horizon.
double chi = 0.8;

// Dimensions of ellipsoid
double a = 2;
double b = 1;
double c = 1;

// Distance between sphere centers of peanut.
double d = 0.9;

Grid grid;

void output_embedding(shared_ptr<Grid3DFunction> embedding, char *identifier = nullptr) {
	char embedding_filename[50];
	if (identifier != nullptr)
		sprintf(embedding_filename, "./assets/embedding_%s.csv", identifier);
	else 
		sprintf(embedding_filename, "./assets/embedding.csv");
	ofstream embedding_output(embedding_filename);
	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			embedding_output << embedding->x_values[i][j] << "," << embedding->y_values[i][j] << "," << embedding->z_values[i][j] << endl;
		}
	}
	embedding_output.close();
}

void find_solution(int N_theta, int N_phi) {
	Grid grid_(N_theta, N_phi);
	grid = grid_;

	// shared_ptr<RoundSphereMetric> metric = make_shared<RoundSphereMetric>(chi);
	// shared_ptr<HorizonMetric> metric = make_shared<HorizonMetric>(chi);
	shared_ptr<EllipsoidMetric> metric = make_shared<EllipsoidMetric>(a, b, c);
	// shared_ptr<PeanutMetric> metric = make_shared<PeanutMetric>(d);
	// shared_ptr<DentedSphereMetric> metric = make_shared<DentedSphereMetric>();

	shared_ptr<Grid3DFunction> e_theta_1 = make_shared<Grid3DFunction>(grid);
	shared_ptr<Grid3DFunction> e_theta_2 = make_shared<Grid3DFunction>(grid);
	shared_ptr<Grid3DFunction> e_phi = make_shared<Grid3DFunction>(grid);
	shared_ptr<Grid3DFunction> embedding_1 = make_shared<Grid3DFunction>(grid);
	shared_ptr<Grid3DFunction> embedding_2 = make_shared<Grid3DFunction>(grid);

	char identifier[50];
	// char *identifier = nullptr; // no identifier
	// double identifier = (double) N_theta;
	// double identifier = a*100 + b*10 + c;
	// double identifier = d;
	// double identifier = chi;

	run_factorization(metric, e_theta_1, e_phi, e_theta_2);
	shared_ptr<Grid3DFunction> initial_e_phi = e_phi->get_copy();

	// First value of e_theta.
	sprintf(identifier, "1_(%.0f,%.0f,%.0f)", a, b, c);
	run_relaxation(e_theta_1, e_phi, get_commutator_rms, identifier);
	run_integration(e_theta_1, e_phi, embedding_1);
	output_embedding(embedding_1, identifier);

	// Second value of e_theta.
	sprintf(identifier, "2_(%.0f,%.0f,%.0f)", a, b, c);
	e_phi = initial_e_phi;
	run_relaxation(e_theta_2, e_phi, get_commutator_rms, identifier);
	run_integration(e_theta_2, e_phi, embedding_2);
	output_embedding(embedding_2, identifier);

	printf("\n\nDone!\n");
}

int main() {
	// Temporarily run for only one resolution.
	find_solution(30, 30);

	// Vary grid space.
	// for (int N = 30; N <= 50; N += 10) {
	// 	printf("\n%dx%d\n", N, 4*N);

	// 	find_solution(N, 4*N);
	// }

	// Vary chi.
	// for (chi = 0; chi <= 1; chi += 0.1) {
	// 	printf("\nchi = %f\n", chi);

	// 	find_solution(25, 100);
	// }

	// Vary distance between spheres.
	// for (d = 1.2; d <= 1.4; d += 0.1) {
	// 	printf("\nd = %.1f\n", d);

	// 	find_solution(50, 50);
	// }

	// Vary ellipsoid dimensions.
	// for (b = 1; b <= 9; b += 1) {
	// 	printf("\nb = %.1f\n", b);

	// 	find_solution(15, 60);
	// }
}
