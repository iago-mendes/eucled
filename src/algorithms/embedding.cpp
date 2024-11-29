#include "embedding.h"
using namespace std;

Grid grid__embedding;

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
	char *identifier,
	double final_time
) {
	grid__embedding = embedding->grid;

	// Initialize dyad vectors to a sphere
	shared_ptr<Grid3DFunction> e_theta = make_shared<Grid3DFunction>(grid__embedding, [] (int i, int j, char coord) {
		double R = 2.;
		double theta = grid__embedding.theta(i);
		double phi = grid__embedding.phi(j);

		switch (coord) {
			case 'x':
				return R * cos(theta) * cos(phi);
			case 'y':
				return R * cos(theta) * sin(phi);
			case 'z':
				return - R * sin(theta);
			default:
				return -1.;
		}
	});
	shared_ptr<Grid3DFunction> e_phi = make_shared<Grid3DFunction>(grid__embedding, [] (int i, int j, char coord) {
		double R = 2.;
		double theta = grid__embedding.theta(i);
		double phi = grid__embedding.phi(j);

		switch (coord) {
			case 'x':
				return - R * sin(theta) * sin(phi);
			case 'y':
				return R * sin(theta) * cos(phi);
			case 'z':
				return 0.;
			default:
				return -1.;
		}
	});

	run_relaxation(e_theta, e_phi, embedding, metric, get_commutator_rms, identifier, final_time);

	output_embedding(embedding, identifier);
}
