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
	char *identifier
) {
	grid__embedding = embedding->grid;

	shared_ptr<Grid3DFunction> e_theta = make_shared<Grid3DFunction>(grid__embedding);
	shared_ptr<Grid3DFunction> e_phi = make_shared<Grid3DFunction>(grid__embedding);

	run_factorization(metric, e_theta, e_phi);
	run_relaxation(e_theta, e_phi, get_commutator_rms, metric, identifier);
	run_integration(e_theta, e_phi, embedding);

	output_embedding(embedding, identifier);
}
