#include "relaxation.h"
using namespace std;

shared_ptr<Grid3DFunction> e_theta__relaxation(nullptr);
shared_ptr<Grid3DFunction> e_phi__relaxation(nullptr);

TimeStepper time_stepper(INITIAL_TIME_STEP, 10);

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

double negate_multiplier(
	[[maybe_unused]] double theta,
	[[maybe_unused]] double phi,
	[[maybe_unused]] char coordinate
) {
	return - 1;
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

	e_theta_derivative = (*e_theta_derivative).multiplied_by(negate_multiplier);

	e_theta__relaxation = (*e_theta__relaxation).added_with(e_theta_derivative, time_step);
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

	e_phi_derivative = (*e_phi_derivative).multiplied_by(negate_multiplier);

	e_phi__relaxation = (*e_phi__relaxation).added_with(e_phi_derivative, time_step);
}

double run_relaxation(
	shared_ptr<Grid3DFunction> e_theta,
	shared_ptr<Grid3DFunction> e_phi,
	double (*get_residual)(shared_ptr<Grid3DFunction> e_theta, shared_ptr<Grid3DFunction> e_phi)
) {
	e_theta__relaxation = e_theta;
	e_phi__relaxation = e_phi;
	Grid *grid = &e_theta->grid;

	char residuals_filename[50];
	sprintf(residuals_filename, "./assets/residuals_%d.csv", grid->N_theta);
	ofstream residuals_output(residuals_filename);
	ofstream residual_distribution_output("./assets/residual_distribution.csv");

	time_stepper.reset();

	// output x values
	for (int i = 0; i < grid->N_theta; i++) {
		for (int j = 0; j < grid->N_phi; j++) {
			residual_distribution_output << grid->theta(i);

			if (i == grid->N_theta - 1 && j == grid->N_phi - 1) { // last
				residual_distribution_output << endl;
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

	// Solve.
	int iteration_number = 0;
	while (
		residual > RESIDUAL_TOLERANCE &&
		iteration_number < MAX_ITERATIONS
	) {
		if (iteration_number % OUTPUT_FREQUENCY == 0)
			printf("(%d) R = %8.2e, step = %8.2e\n", iteration_number, residual, time_step);
		
		update_e_theta(time_step);
		update_e_phi(time_step);

		residual = abs(get_residual(e_theta__relaxation, e_phi__relaxation));
		residuals_output << iteration_number << "," << residual << endl;
		minimum_residual = min(minimum_residual, residual);

		if (iteration_number % OUTPUT_FREQUENCY == 0) {
			// output residual norm values
			auto residual_norm = get_commutator_norm(e_theta__relaxation, e_phi__relaxation);
			for (int i = 0; i < grid->N_theta; i++) {
				for (int j = 0; j < grid->N_phi; j++) {
					residual_distribution_output << residual_norm->points[i][j];

					if (i == grid->N_theta - 1 && j == grid->N_phi - 1) { // last
						residual_distribution_output << endl;
					} else {
						residual_distribution_output << ",";
					}
				}
			}
		}

		iteration_number++;
	}

	(*e_theta) = (*e_theta__relaxation);
	(*e_phi) = (*e_phi__relaxation);

	printf("Relaxation finished with R = %8.2e after %5d iterations.\n", minimum_residual, iteration_number);
	return minimum_residual;
}
