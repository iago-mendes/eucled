// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "./embedding.hpp"

#include <cstdio>
#include <math.h>
#include <fstream>

#include "./relaxation.hpp"
#include "../Utilities/commutator.hpp"

using namespace std;

Mesh grid__embedding;

void output_embedding(shared_ptr<DataMesh3D> embedding, char *identifier = nullptr) {
	char embedding_filename[50];
	if (identifier != nullptr)
		sprintf(embedding_filename, "./assets/embedding_%s.csv", identifier);
	else 
		sprintf(embedding_filename, "./assets/embedding.csv");
	ofstream embedding_output(embedding_filename);
	for (int i = 0; i < grid__embedding.N_theta; i++) {
		for (int j = 0; j < grid__embedding.N_phi; j++) {
			embedding_output << embedding->x_points[i][j] << "," << embedding->y_points[i][j] << "," << embedding->z_points[i][j] << endl;
		}
	}
	embedding_output.close();
}

void run_embedding(
	shared_ptr<Metric> metric,
	shared_ptr<DataMesh3D> embedding,
	char *identifier,
	double final_time
) {
	grid__embedding = embedding->mesh;

	// Initialize dyad vectors to a sphere
	shared_ptr<DataMesh3D> e_theta = make_shared<DataMesh3D>(grid__embedding, [] (int i, int j, char coord) {
		double R = 1.;
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
	shared_ptr<DataMesh3D> e_phi = make_shared<DataMesh3D>(grid__embedding, [] (int i, int j, char coord) {
		double R = 1.;
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

	run_relaxation(e_theta, e_phi, embedding, metric, identifier, final_time);

	output_embedding(embedding, identifier);
}
