// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "./embedding.hpp"

#include <cstdio>
#include <math.h>
#include <fstream>

#include "./relaxation.hpp"

using namespace std;

std::shared_ptr<DataMesh3D> find_embedding(
	const std::shared_ptr<Metric> metric,
	const Mesh mesh,
	const double final_time
) {
	// Initialize dyad vectors and embedding to sphere of radius 2
	auto e_theta = make_shared<DataMesh3D>(mesh, [](double theta, double phi, char coord) {
		switch (coord) {
			case 'x':
				return 2. * cos(theta) * cos(phi);
			case 'y':
				return 2. * cos(theta) * sin(phi);
			case 'z':
				return - 2. * sin(theta);
			default:
				return 0.;
		}
	});
	auto e_phi = make_shared<DataMesh3D>(mesh, [](double theta, double phi, char coord) {
		switch (coord) {
			case 'x':
				return - 2. * sin(theta) * sin(phi);
			case 'y':
				return 2. * sin(theta) * cos(phi);
			case 'z':
				return 0.;
			default:
				return 0.;
		}
	});
	auto embedding = make_shared<DataMesh3D>(mesh, [](double theta, double phi, char coord) {
		switch (coord) {
			case 'x':
				return 2. * sin(theta) * cos(phi);
			case 'y':
				return 2. * sin(theta) * sin(phi);
			case 'z':
				return 2. * cos(theta);
			default:
				return 0.;
		}
	});

	run_relaxation(e_theta, e_phi, embedding, metric, final_time);

	ofstream embedding_output("Embedding.vol");
	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			embedding_output << embedding->x_points[i][j] << "," << embedding->y_points[i][j] << "," << embedding->z_points[i][j] << endl;
		}
	}
	embedding_output.close();

	return embedding;
}
