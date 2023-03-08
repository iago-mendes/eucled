#include "embedding.h"
using namespace std;

Grid grid__embedding;
RoundSphereDyad round_sphere_dyad(1);

void output_embedding(shared_ptr<Grid3DFunction> embedding, char *identifier = nullptr) {
	char embedding_filename[50];
	if (identifier != nullptr)
		sprintf(embedding_filename, "./assets/embedding_%s.csv", identifier);
	else 
		sprintf(embedding_filename, "./assets/embedding.csv");
	ofstream embedding_output(embedding_filename);
	for (int i = 0; i < grid__embedding.N_theta; i++) {
		for (int j = 0; j < grid__embedding.N_phi; j++) {
			embedding_output << embedding->x_values[i][j] << "," << embedding->y_values[i][j] << "," << embedding->z_values[i][j] << endl;
		}
	}
	embedding_output.close();
}

void run_embedding(
	shared_ptr<Metric> metric,
	shared_ptr<Grid3DFunction> embedding,
	char *identifier
) {
	grid__embedding = embedding->grid;

	// // Not working (needs to be debugged)
	// shared_ptr<Grid3DFunction> e_theta = round_sphere_dyad.get_theta_function(grid__embedding);
	// shared_ptr<Grid3DFunction> e_phi = round_sphere_dyad.get_phi_function(grid__embedding);

	// Same purpose as previous block
	shared_ptr<Grid3DFunction> e_theta = make_shared<Grid3DFunction>(grid__embedding, [] (int i, int j, char coord) {
		double theta = grid__embedding.theta(i);
		double phi = grid__embedding.phi(j);

		switch (coord) {
			case 'x':
				return round_sphere_dyad.e_theta_x(theta,  phi);
			case 'y':
				return round_sphere_dyad.e_theta_y(theta,  phi);
			case 'z':
				return round_sphere_dyad.e_theta_z(theta,  phi);
			default:
				return -1.;
		}
	});
	shared_ptr<Grid3DFunction> e_phi = make_shared<Grid3DFunction>(grid__embedding, [] (int i, int j, char coord) {
		double theta = grid__embedding.theta(i);
		double phi = grid__embedding.phi(j);

		switch (coord) {
			case 'x':
				return round_sphere_dyad.e_phi_x(theta,  phi);
			case 'y':
				return round_sphere_dyad.e_phi_y(theta,  phi);
			case 'z':
				return round_sphere_dyad.e_phi_z(theta,  phi);
			default:
				return -1.;
		}
	});

	run_relaxation(e_theta, e_phi, metric, get_commutator_rms, identifier);
	run_integration(e_theta, e_phi, embedding);

	output_embedding(embedding, identifier);
}
