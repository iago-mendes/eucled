// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "./integrate.hpp"

#include <math.h>

#include "./math.hpp"

double integrate(std::shared_ptr<DataMesh> data) {
	double result = 0.;

	for (int i = 0; i < data->mesh.N_theta; i++) {
		const double theta = data->mesh.theta(i);
		const double dA = sin(theta) * data->mesh.delta_theta * data->mesh.delta_phi;

		for (int j = 0; j < data->mesh.N_phi; j++) {
			result += data->points[i][j] * dA;
		}
	}

	return result;
}

double integrate(std::shared_ptr<DataMesh> data_theta_theta,
                 std::shared_ptr<DataMesh> data_theta_phi,
                 std::shared_ptr<DataMesh> data_phi_phi) {
	const auto sqr_norm = data_theta_theta * data_theta_theta
								+ data_theta_phi * data_theta_phi * 2 * inv_sqr_sin_theta
								+ data_phi_phi * data_phi_phi * inv_sin_theta_4;
	
	return sqrt(integrate(sqr_norm));
}
