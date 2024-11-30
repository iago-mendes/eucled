// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "./relaxation.hpp"

#include <cstdio>
#include <math.h>
#include <fstream>

#include "../Utilities/commutator.hpp"
#include "../Utilities/cross_product.hpp"
#include "../Utilities/math.hpp"

namespace {

std::shared_ptr<DataMesh3D> e_theta(nullptr);
std::shared_ptr<DataMesh3D> e_phi(nullptr);
std::shared_ptr<DataMesh3D> embedding(nullptr);
std::shared_ptr<Metric> metric(nullptr);

Mesh *grid;

// Damping parameter
double damping_param = 1.;

// Theta operator
std::shared_ptr<DataMesh3D> D_theta(std::shared_ptr<DataMesh3D> v) {
	return (v * sin_theta)->partial_theta() * inv_sin_theta;
}

// Phi operator
std::shared_ptr<DataMesh3D> D_phi(std::shared_ptr<DataMesh3D> v) {
	return (v * sin_theta)->partial_phi() * inv_sin_theta;
}

// Angular velocity vector
std::shared_ptr<DataMesh3D> omega() {
	std::shared_ptr<DataMesh3D> commutator = get_commutator(e_theta, e_phi);

	return get_cross_product(e_phi, D_theta(commutator))
				 - get_cross_product(e_theta, D_phi(commutator));
}

// Theta-theta constraint
std::shared_ptr<DataMesh> C_theta_theta() {
	return e_theta->dot(e_theta)
		+ [](double theta, double phi) {return - metric->g_theta_theta(theta, phi);}
	;
}

// Theta-phi constraint
std::shared_ptr<DataMesh> C_theta_phi() {
	return e_theta->dot(e_phi)
		+ [](double theta, double phi) {return - metric->g_theta_phi(theta, phi);}
	;
}

// Phi-phi constraint
std::shared_ptr<DataMesh> C_phi_phi() {
	return e_phi->dot(e_phi)
		+ [](double theta, double phi) {return - metric->g_phi_phi(theta, phi);}
	;
}

// Euler stepping for e_theta
void update_e_theta(double time_step) {
	std::shared_ptr<DataMesh3D> e_theta_dot =
		get_cross_product(omega(), e_theta)
		- e_theta * damping_param * C_theta_theta()
		- e_phi * damping_param * C_theta_phi() * inv_sqr_sin_theta
	;

	e_theta = e_theta + e_theta_dot * time_step;
}

// Euler stepping for e_phi
void update_e_phi(double time_step) {
	std::shared_ptr<DataMesh3D> e_phi_derivative =
		get_cross_product(omega(), e_phi)
		- e_theta * damping_param * C_theta_phi()
		- e_phi * damping_param * C_phi_phi() * inv_sqr_sin_theta
	;

	e_phi = e_phi + e_phi_derivative * time_step;
}

// Euler stepping for embedding
// (returns embedding residual)
double update_embedding(double time_step) {
	auto laplacian =
		embedding->second_partial_theta()
		+ embedding->partial_theta() * inv_tan_theta
		+ embedding->second_partial_phi() * inv_sqr_sin_theta
	;

	auto source =
		e_theta->partial_theta()
		+ e_theta * inv_tan_theta
		+ e_phi->partial_phi() * inv_sqr_sin_theta
	;

	std::shared_ptr<DataMesh3D> embedding_dot = laplacian - source;
	embedding = embedding + embedding_dot * sqr_sin_theta * 0.5 * time_step;

	double embedding_residual = (embedding_dot * sqrt_sin_theta)->norm()->rms();
	return embedding_residual;
}

} // namespace

double run_relaxation(
	std::shared_ptr<DataMesh3D> e_theta_,
	std::shared_ptr<DataMesh3D> e_phi_,
	std::shared_ptr<DataMesh3D> embedding_,
	std::shared_ptr<Metric> metric_,
	double final_time
) {
	e_theta = e_theta_;
	e_phi = e_phi_;
	embedding = embedding_;
	grid = &e_theta_->mesh;
	metric = metric_;
	bool use_fixed_final_time = final_time != 0;

	std::ofstream residuals_output("residuals.csv");
	std::ofstream embedding_residuals_output("embedding_residuals.csv");
	std::ofstream residual_distribution_output("residual_distribution.csv");
	std::ofstream constraints_output("constraints.csv");

	// output x values
	for (int i = 0; i < grid->N_theta; i++) {
		for (int j = 0; j < grid->N_phi; j++) {
			residual_distribution_output << grid->theta(i);

			if (i == grid->N_theta - 1 && j == grid->N_phi - 1) { // last
				residual_distribution_output << std::endl;
			} else {
				residual_distribution_output << ",";
			}
		}
	}

	// output y values
	for (int i = 0; i < grid->N_theta; i++) {
		for (int j = 0; j < grid->N_phi; j++) {
			residual_distribution_output << grid->phi(j);

			if (i == grid->N_theta - 1 && j == grid->N_phi - 1) { // last
				residual_distribution_output << std::endl;
			} else {
				residual_distribution_output << ",";
			}
		}
	}

	double time_step = 0.01; // Good for 15 x 30
	time_step *= sqr(15. / (double) grid->N_theta);
	printf("Time step = %.2e\n", time_step);

	double residual= abs(get_commutator_rms(e_theta, e_phi));
	double embedding_residual = INFINITY;

	auto best_e_theta = e_theta;
	auto best_e_phi = e_phi;
	auto best_residual = INFINITY;

	// Solve.
	int iteration_number = 0;
	int max_iterations = MAX_ITERATIONS;
	bool started_decreasing = false;
	double prev_residual = residual;
	while (
		use_fixed_final_time ? iteration_number * time_step < final_time : iteration_number < max_iterations
	) {
		if (iteration_number % OUTPUT_FREQUENCY == 0) {
			printf("(%d) Dyad residual = %e, Embedding residual = %e\n", iteration_number, residual, embedding_residual);
		}
		
		update_e_theta(time_step);
		update_e_phi(time_step);
		embedding_residual = update_embedding(time_step);

		residual = abs(get_commutator_rms(e_theta, e_phi));
		residuals_output << iteration_number << "," << residual << std::endl;

		embedding_residuals_output << iteration_number << "," << embedding_residual << std::endl;

		constraints_output
			<< (C_theta_theta() * sin_theta)->rms() << ","
			<< (C_theta_phi() * sin_theta)->rms() << ","
			<< (C_phi_phi() * sin_theta)->rms() << std::endl;
		
		if (isnan(residual)) {
			printf("Found NaN's!\n");
		}

		if (!started_decreasing) {
			if (residual < prev_residual) {
				started_decreasing = true;
				printf("Started decreasing!\n");
			}
			prev_residual = residual;
		}

		if (started_decreasing && residual < best_residual) {
			best_e_theta = e_theta;
			best_e_phi = e_phi;
			best_residual = residual;
		} else if (started_decreasing && max_iterations == MAX_ITERATIONS) {
			max_iterations = iteration_number; // Stop
			printf("Reached minimum dyad residual.\n");
		}

		if (iteration_number % OUTPUT_FREQUENCY == 0) {
			// output residual norm values
			auto residual_norm = get_commutator(e_theta, e_phi)->norm();
			for (int i = 0; i < grid->N_theta; i++) {
				for (int j = 0; j < grid->N_phi; j++) {
					residual_distribution_output << residual_norm->points[i][j];

					if (i == grid->N_theta - 1 && j == grid->N_phi - 1) { // last
						residual_distribution_output << std::endl;
					} else {
						residual_distribution_output << ",";
					}
				}
			}
		}

		if (residual <= RESIDUAL_TOLERANCE) {
			// Stop after the residual tolerance is reached.
			printf("Residual tolerance was reached.\n");
			break;
		}

		iteration_number++;
	}

	int dyad_last_iteration = iteration_number;
	printf("Dyad relaxation finished with R = %.2e after %d iterations.\n", best_residual, dyad_last_iteration);

	// double embedding_final_time = dyad_final_time + 50;

	max_iterations = iteration_number + 100000;
	bool printed_minimum_msg = false;
	bool printed_tolerance_msg = false;
	while (
		use_fixed_final_time ? iteration_number * time_step < 2*final_time : iteration_number < max_iterations
	) {
		if (iteration_number % OUTPUT_FREQUENCY == 0) {
			printf("(%d) Embedding residual = %e\n", iteration_number, embedding_residual);
		}
		double prev_embedding_residual = embedding_residual;

		embedding_residual = update_embedding(time_step);

		embedding_residuals_output << iteration_number << "," << embedding_residual << std::endl;

		if (abs(embedding_residual) > abs(prev_embedding_residual)) {
			max_iterations = iteration_number; // Stop
			if (!printed_minimum_msg) {
				printf("Reached MINIMUM embedding residual.\n");
				printed_minimum_msg = true;
			}
		}

		if ((iteration_number - dyad_last_iteration) * time_step > 10 && abs(embedding_residual) <= best_residual) {
			max_iterations = iteration_number; // Stop
			if (!printed_tolerance_msg) {
				printf("Reached embedding residual TOLERANCE.\n");
				printed_tolerance_msg = true;
			}
		}

		iteration_number++;
	}
	
	printf("Embedding relaxation finished with R = %.2e after %d iterations.\n", embedding_residual, iteration_number);

	(*e_theta_) = (*best_e_theta);
	(*e_phi_) = (*best_e_phi);
	(*embedding_) = (*embedding);

	return best_residual;
}
