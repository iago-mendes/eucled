#include "relaxation.h"
using namespace std;

shared_ptr<Grid3DFunction> e_theta__relaxation(nullptr);
shared_ptr<Grid3DFunction> e_phi__relaxation(nullptr);

TimeStepper time_stepper(INITIAL_TIME_STEP, 10);
Grid *grid__relaxation;

double sin_multiplier(
	double theta,
	[[maybe_unused]] double phi,
	[[maybe_unused]] char coordinate
) {
	return sin(theta);
}

double inverse_sin_multiplier(
	double theta,
	[[maybe_unused]] double phi,
	[[maybe_unused]] char coordinate
) {
	return 1 / sin(theta);
}

double sin_squared_multiplier(
	double theta,
	[[maybe_unused]] double phi,
	[[maybe_unused]] char coordinate
) {
	return squared(sin(theta));
}

void update_e_theta(double time_step) {
	shared_ptr<Grid3DFunction> e_theta = e_theta__relaxation;
	shared_ptr<Grid3DFunction> e_phi = e_phi__relaxation;

	shared_ptr<Grid3DFunction> left_side = get_commutator(e_theta, e_phi);
	shared_ptr<Grid3DFunction> right_side = get_commutator(e_theta, e_phi);

	left_side = (*left_side).multiplied_by(sin_multiplier);
	right_side = (*right_side).multiplied_by(sin_multiplier);

	if (isnan((*left_side).rms()) || isnan((*right_side).rms())) {
		printf("NaN 1\n");
	}

	left_side = (*left_side).partial_theta();
	right_side = (*right_side).partial_phi();

	if (isnan((*left_side).rms()) || isnan((*right_side).rms())) {
		printf("NaN 2\n");
	}

	left_side = get_cross_product(left_side, e_phi);
	right_side = get_cross_product(right_side, e_theta);

	if (isnan((*left_side).rms()) || isnan((*right_side).rms())) {
		printf("NaN 3\n");
	}

	left_side = (*left_side).multiplied_by(inverse_sin_multiplier);
	right_side = (*right_side).multiplied_by(inverse_sin_multiplier);

	if (isnan((*left_side).rms()) || isnan((*right_side).rms())) {
		printf("NaN 4\n");
	}

	shared_ptr<Grid3DFunction> e_theta_derivative = (*left_side).added_with(right_side, -1);

	e_theta_derivative = get_cross_product(e_theta_derivative, e_theta);

	e_theta_derivative = (*e_theta_derivative).multiplied_by(-time_step);

	e_theta__relaxation = (*e_theta__relaxation).added_with(e_theta_derivative, sin_squared_multiplier);
}

void update_e_phi(double time_step) {
	shared_ptr<Grid3DFunction> e_theta = e_theta__relaxation;
	shared_ptr<Grid3DFunction> e_phi = e_phi__relaxation;

	shared_ptr<Grid3DFunction> left_side = get_commutator(e_theta, e_phi);
	shared_ptr<Grid3DFunction> right_side = get_commutator(e_theta, e_phi);

	left_side = (*left_side).multiplied_by(sin_multiplier);
	right_side = (*right_side).multiplied_by(sin_multiplier);

	left_side = (*left_side).partial_theta();
	right_side = (*right_side).partial_phi();

	left_side = get_cross_product(left_side, e_phi);
	right_side = get_cross_product(right_side, e_theta);

	left_side = (*left_side).multiplied_by(inverse_sin_multiplier);
	right_side = (*right_side).multiplied_by(inverse_sin_multiplier);

	shared_ptr<Grid3DFunction> e_phi_derivative = (*left_side).added_with(right_side, -1);

	e_phi_derivative = get_cross_product(e_phi_derivative, e_phi);

	e_phi_derivative = (*e_phi_derivative).multiplied_by(-time_step);

	e_phi__relaxation = (*e_phi__relaxation).added_with(e_phi_derivative, sin_squared_multiplier);
}

double run_relaxation(
	shared_ptr<Grid3DFunction> e_theta,
	shared_ptr<Grid3DFunction> e_phi,
	double (*get_residual)(shared_ptr<Grid3DFunction> e_theta, shared_ptr<Grid3DFunction> e_phi),
	char *identifier
) {
	e_theta__relaxation = e_theta;
	e_phi__relaxation = e_phi;
	grid__relaxation = &e_theta->grid;

	char residuals_filename[50];
	if (identifier != nullptr) {
		sprintf(residuals_filename, "./assets/residuals_%s.csv", identifier);
	} else {
		sprintf(residuals_filename, "./assets/residuals_%d.csv", grid__relaxation->N_theta);
	}
	ofstream residuals_output(residuals_filename);
	ofstream residual_distribution_output("./assets/residual_distribution.csv");

	time_stepper.reset();

	// output x values
	for (int i = 0; i < grid__relaxation->N_theta; i++) {
		for (int j = 0; j < grid__relaxation->N_phi; j++) {
			residual_distribution_output << grid__relaxation->theta(i);

			if (i == grid__relaxation->N_theta - 1 && j == grid__relaxation->N_phi - 1) { // last
				residual_distribution_output << endl;
			} else {
				residual_distribution_output << ",";
			}
		}
	}

	// output y values
	for (int i = 0; i < grid__relaxation->N_theta; i++) {
		for (int j = 0; j < grid__relaxation->N_phi; j++) {
			residual_distribution_output << grid__relaxation->phi(j);

			if (i == grid__relaxation->N_theta - 1 && j == grid__relaxation->N_phi - 1) { // last
				residual_distribution_output << endl;
			} else {
				residual_distribution_output << ",";
			}
		}
	}

	double residual = abs(get_residual(e_theta__relaxation, e_phi__relaxation));

	// Find ideal time step.
	double time_step = INITIAL_TIME_STEP;
	for (int i = 0; i < INITIAL_ITERATIONS; i++) {
		if (i % OUTPUT_FREQUENCY == 0)
			printf("(%d) R = %8.2e, step = %8.2e\n", i, residual, time_stepper.get_step());

		update_e_theta(time_stepper.get_step());
		update_e_phi(time_stepper.get_step());

		double previous_residual = residual;
		residual = abs(get_residual(e_theta__relaxation, e_phi__relaxation));

		shared_ptr<Iteration> updated_iteration = time_stepper.update_step(e_theta__relaxation, e_phi__relaxation, residual);
		if (updated_iteration->solution1 != e_theta__relaxation || updated_iteration->solution2 != e_phi__relaxation) {
			e_theta__relaxation = updated_iteration->solution1;
			e_phi__relaxation = updated_iteration->solution2;
			residual = updated_iteration->residual;
		}

		if (residual < previous_residual) {
			time_step = min(time_step, time_stepper.get_step());
		}
	}
	printf("Restarting with step = %8.2e...\n", time_step);

	// Restart.
	e_theta__relaxation = e_theta;
	e_phi__relaxation = e_phi;
	residual = abs(get_residual(e_theta__relaxation, e_phi__relaxation));
	double minimum_residual = residual;

	Iteration best_solution;
	best_solution.solution1 = e_theta;
	best_solution.solution2 = e_phi;
	best_solution.residual = residual;

	// Solve.
	int iteration_number = 0;
	int max_iterations = MAX_ITERATIONS;
	while (iteration_number < max_iterations) {
		if (iteration_number % OUTPUT_FREQUENCY == 0) {
			printf("(%d) R = %8.2e, step = %8.2e\n", iteration_number, residual, time_step);
		}
		
		update_e_theta(time_step);
		update_e_phi(time_step);

		residual = abs(get_residual(e_theta__relaxation, e_phi__relaxation));
		residuals_output << iteration_number << "," << residual << endl;

		if (residual < best_solution.residual) {
			best_solution.solution1 = e_theta__relaxation;
			best_solution.solution2 = e_phi__relaxation;
			best_solution.residual = residual;
		} else if (max_iterations == MAX_ITERATIONS) {
			// Run 100 more iterations after minimum residual was found.
			max_iterations = iteration_number + 100;
			printf("Minimum residual was reached.\n");
		}

		if (iteration_number % OUTPUT_FREQUENCY == 0) {
			// output residual norm values
			auto residual_norm = get_commutator_norm(e_theta__relaxation, e_phi__relaxation);
			for (int i = 0; i < grid__relaxation->N_theta; i++) {
				for (int j = 0; j < grid__relaxation->N_phi; j++) {
					residual_distribution_output << residual_norm->points[i][j];

					if (i == grid__relaxation->N_theta - 1 && j == grid__relaxation->N_phi - 1) { // last
						residual_distribution_output << endl;
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

  double dot_product_residual_theta_theta =
		e_theta->dot_product_with(e_theta)->added_with(
			best_solution.solution1->dot_product_with(best_solution.solution1),
			-1
		)->rms();
	double dot_product_residual_theta_phi =
		e_theta->dot_product_with(e_phi)->added_with(
			best_solution.solution1->dot_product_with(best_solution.solution2),
			-1
		)->rms();
	double dot_product_residual_phi_phi =
		e_phi->dot_product_with(e_phi)->added_with(
			best_solution.solution2->dot_product_with(best_solution.solution2),
			-1
		)->rms();
	
	printf(
		"Dot product residuals:\n\ttheta theta = %.2e\n\ttheta phi = %.2e\n\tphi phi = %.2e\n",
		dot_product_residual_theta_theta,
		dot_product_residual_theta_phi,
		dot_product_residual_phi_phi
	);

	(*e_theta) = (*best_solution.solution1);
	(*e_phi) = (*best_solution.solution2);

	printf("Relaxation finished with R = %.2e after %d iterations.\n", best_solution.residual, iteration_number);
	return minimum_residual;
}
