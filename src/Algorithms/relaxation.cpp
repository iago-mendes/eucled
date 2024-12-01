// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "./relaxation.hpp"

#include <iostream>
#include <math.h>

#include "../IO/DatFileWriter.hpp"
#include "../IO/VolFileWriter.hpp"
#include "../Utilities/cross_product.hpp"
#include "../Utilities/integrate.hpp"
#include "../Utilities/math.hpp"

namespace {

#define MAX_ITERATIONS 100000
#define LOG_FREQUENCY 1000
#define OUTPUT_FREQUENCY 100

std::shared_ptr<DataMesh3D> e_theta(nullptr);
std::shared_ptr<DataMesh3D> e_phi(nullptr);
std::shared_ptr<DataMesh3D> embedding(nullptr);
std::shared_ptr<Metric> metric(nullptr);

DatFileWriter residuals_output("Residuals.dat");
VolFileWriter residual_distribution_output("EmbeddingResidual.vol");

// Theta derivative operator
std::shared_ptr<DataMesh3D> D_theta(std::shared_ptr<DataMesh3D> v) {
	return (v * sin_theta)->partial_theta() * inv_sin_theta;
}

// Phi derivative operator
std::shared_ptr<DataMesh3D> D_phi(std::shared_ptr<DataMesh3D> v) {
	return (v * sin_theta)->partial_phi() * inv_sin_theta;
}

/**
 * Update the embedding functions using a relaxation scheme on the Poisson
 * equations that relate the embedding functions and the dyad vectors
 * 
 * @return Poisson relaxation residual
 */
double update_embedding(double time_step) {
	auto laplacian = embedding->second_partial_theta()
									 + embedding->partial_theta() * inv_tan_theta
									 + embedding->second_partial_phi() * inv_sqr_sin_theta;

	auto source = e_theta->partial_theta()
		            + e_theta * inv_tan_theta
		            + e_phi->partial_phi() * inv_sqr_sin_theta;

	auto dt_embedding = laplacian - source;
	embedding = embedding + dt_embedding * sqr_sin_theta * time_step;

	return sqrt(integrate(dt_embedding->sqr_norm()));
}

/**
 * Get a residual to measure how well the embedding functions satisfy the
 * embedding equations
 * 
 * @return embedding residual
 */
double get_embedding_residual() {
	auto R_theta_theta = embedding->partial_theta()->dot(embedding->partial_theta())
											 + [](double theta, double phi) {return - metric->g_theta_theta(theta, phi);};
	auto R_theta_phi = embedding->partial_theta()->dot(embedding->partial_phi())
										 + [](double theta, double phi) {return - metric->g_theta_phi(theta, phi);};
	auto R_phi_phi = embedding->partial_phi()->dot(embedding->partial_phi())
									 + [](double theta, double phi) {return - metric->g_phi_phi(theta, phi);};
	
	const auto R_sqr_norm = R_theta_theta * R_theta_theta
								        + R_theta_phi * R_theta_phi * 2 * inv_sqr_sin_theta
								        + R_phi_phi * R_phi_phi * inv_sin_theta_4;
	residual_distribution_output.write_data(R_sqr_norm);

	return sqrt(integrate(R_sqr_norm));
}

} // namespace

void run_relaxation(
	std::shared_ptr<DataMesh3D> e_theta_,
	std::shared_ptr<DataMesh3D> e_phi_,
	std::shared_ptr<DataMesh3D> embedding_,
	std::shared_ptr<Metric> metric_,
	double final_time
) {
	// Set useful variables
	e_theta = e_theta_;
	e_phi = e_phi_;
	embedding = embedding_;
	metric = metric_;
	Mesh& mesh = e_theta_->mesh;
	bool use_fixed_final_time = final_time != 0;

	// Set up output
	residuals_output.write_header({
		"Iteration",
		"RelaxationTime",
		"CommutatorResidual",
		"ConstraintResidual",
		"PoissonResidual",
		"EmbeddingResidual"
	});
	residual_distribution_output.write_mesh(mesh);
	
	// Set relaxation parameters
	double damping_param = 10.;
	double time_step = 0.01; // Good for 15 x 30
	time_step *= sqr(15. / (double) mesh.N_theta);
	std::cout << "Time step = " << time_step << std::endl;

	// Initialize residuals
	double commutator_residual = INFINITY;
	double constraint_residual = INFINITY;
	double poisson_residual = INFINITY;
	double embedding_residual = get_embedding_residual();
	std::cout << std::scientific
						<< "Initial residual: "
						<< "embedding = " << embedding_residual << std::endl;

	// Dyad relaxation + Poisson relaxation
	int iteration = 0;
	for (; iteration < MAX_ITERATIONS; iteration++) {

		// Commutator
		const auto commutator = e_phi->partial_theta() - e_theta->partial_phi();
		const double prev_commutator_residual = commutator_residual;
		commutator_residual = sqrt(integrate(commutator->sqr_norm()));

		// Constraints
		const auto C_theta_theta = e_theta->dot(e_theta)
												 + [](double theta, double phi) {return - metric->g_theta_theta(theta, phi);};
		const auto C_theta_phi = e_theta->dot(e_phi)
											 + [](double theta, double phi) {return - metric->g_theta_phi(theta, phi);};
		const auto C_phi_phi = e_phi->dot(e_phi)
										 + [](double theta, double phi) {return - metric->g_phi_phi(theta, phi);};
		constraint_residual = integrate_norm(C_theta_theta, C_theta_phi, C_phi_phi);

		// Angular velocity
		const auto omega = get_cross_product(e_phi, D_theta(commutator))
				 				       - get_cross_product(e_theta, D_phi(commutator));

		// Evolution equations
		const auto dt_e_theta = get_cross_product(omega, e_theta)
										        - e_theta * damping_param * C_theta_theta
										        - e_phi * damping_param * C_theta_phi * inv_sqr_sin_theta;
		const auto dt_e_phi = get_cross_product(omega, e_phi)
										      - e_theta * damping_param * C_theta_phi
									      	- e_phi * damping_param * C_phi_phi * inv_sqr_sin_theta;

		// First-order time stepping
		e_phi = e_phi + dt_e_phi * time_step;
		e_theta = e_theta + dt_e_theta * time_step;
		poisson_residual = update_embedding(time_step);

		// Output residuals
		if (iteration % OUTPUT_FREQUENCY == 0) {
			embedding_residual = get_embedding_residual();
			residuals_output.write_data({
				iteration,
				iteration*time_step,
				commutator_residual,
				constraint_residual,
				poisson_residual,
				embedding_residual
			});
		}
		if (iteration % LOG_FREQUENCY == 0) {
			std::cout << std::scientific
								<< iteration << ") Residuals: "
			          << "commutator = " << commutator_residual << ", "
			          << "constraint = " << constraint_residual << ", "
			          << "poisson = " << poisson_residual << ", "
			          << "embedding = " << embedding_residual << std::endl;
		}

		// Check for NaN values
		if (isnan(commutator_residual) or isnan(embedding_residual)) {
			std::cout << "ERROR: NaN values" << std::endl;
			exit(1);
		}

		// Check stop conditions
		double relative_commutator_change = fabs(commutator_residual - prev_commutator_residual) / prev_commutator_residual;
		if (use_fixed_final_time and iteration * time_step >= final_time) {
			std::cout << "Reached fixed relaxation final time" << std::endl;
			break;
		} else if (!use_fixed_final_time and iteration*time_step > 10. and relative_commutator_change < 1e-10) {
			std::cout << "Reached commutator residual asymptote" << std::endl;
			break;
		}
	}
	std::cout << "Dyad relaxation finished in " << iteration << " iterations" << std::endl;
	std::cout << "\tcommutator residual = " << commutator_residual << std::endl;
	std::cout << "\tconstraint residual = " << constraint_residual << std::endl;

	// Poisson relaxation
	const int min_iterations = iteration + (int) (10. / time_step);
	for (; iteration < MAX_ITERATIONS; iteration++) {

		// First-order time stepping
		poisson_residual = update_embedding(time_step);

		// Output residuals
		if (iteration % OUTPUT_FREQUENCY == 0) {
			embedding_residual = get_embedding_residual();
			residuals_output.write_data({
				iteration,
				iteration*time_step,
				commutator_residual,
				constraint_residual,
				poisson_residual,
				embedding_residual
			});
		}
		if (iteration % LOG_FREQUENCY == 0) {
			std::cout << std::scientific
								<< iteration << ") Residuals: "
			          << "poisson = " << poisson_residual << ", "
			          << "embedding = " << embedding_residual << std::endl;
		}

		// Check stop condition
		if (iteration > min_iterations and poisson_residual <= commutator_residual) {
			std::cout << "Reached poisson residual tolerance" << std::endl;
			break;
		}
	}
	std::cout << "Poisson relaxation finished in " << iteration << " iterations" << std::endl;
	std::cout << "\tpoisson residual = " << poisson_residual << std::endl;
	std::cout << "\tembedding residual = " << embedding_residual << std::endl;
	
	(*e_theta_) = (*e_theta);
	(*e_phi_) = (*e_phi);
	(*embedding_) = (*embedding);
}
