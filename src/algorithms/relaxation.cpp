#include "relaxation.h"
using namespace std;

shared_ptr<Grid3DFunction> e_theta__relaxation(nullptr);
shared_ptr<Grid3DFunction> e_phi__relaxation(nullptr);

TimeStepper time_stepper(INITIAL_TIME_STEP, 10);
Grid *grid__relaxation;

// Operator from equation 28 for theta
shared_ptr<Grid3DFunction> D_theta(shared_ptr<Grid3DFunction> v) {
	return v
		->multiplied_by([] (double theta, [[maybe_unused]] double phi) {return sin(theta);}) // multiply by sin(theta)
		->partial_theta() // take theta partial derivative
		->multiplied_by([] (double theta, [[maybe_unused]] double phi) {return 1/sin(theta);}) // divide by sin(theta)
	;
}

// Operator from equation 28 for phi
shared_ptr<Grid3DFunction> D_phi(shared_ptr<Grid3DFunction> v) {
	return v
		->multiplied_by([] (double theta, [[maybe_unused]] double phi) {return sin(theta);}) // multiply by sin(theta)
		->partial_phi() // take theta partial derivative
		->multiplied_by([] (double theta, [[maybe_unused]] double phi) {return 1/sin(theta);}) // divide by sin(theta)
	;
}

// Angular velocity vector from equation 30
shared_ptr<Grid3DFunction> omega() {
	shared_ptr<Grid3DFunction> commutator = get_commutator(e_theta__relaxation, e_phi__relaxation);

	return
		get_cross_product(D_theta(commutator), e_phi__relaxation)
		->added_with(get_cross_product(D_phi(commutator), e_theta__relaxation), -1)
	;
}

// Euler stepping for theta
void update_e_theta(double time_step) {
	shared_ptr<Grid3DFunction> e_theta_derivative = get_cross_product(omega(), e_theta__relaxation);

	e_theta__relaxation = e_theta__relaxation
		// ->added_with(e_theta_derivative, -time_step);
		->added_with(e_theta_derivative->multiplied_by(-time_step), [] (double theta, [[maybe_unused]] double phi) {return squared(sin(theta));});
}

// Euler stepping for phi
void update_e_phi(double time_step) {
	shared_ptr<Grid3DFunction> e_phi_derivative = get_cross_product(omega(), e_phi__relaxation);

	e_phi__relaxation = e_phi__relaxation
		// ->added_with(e_phi_derivative, -time_step);
		->added_with(e_phi_derivative->multiplied_by(-time_step), [] (double theta, [[maybe_unused]] double phi) {return squared(sin(theta));});
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

	printf("Relaxation finished with R = %.2e after %d iterations.\n", best_solution.residual, iteration_number);
	
	printf(
		"Dot product residuals:\n\ttheta theta: %.2e\n\ttheta phi: %.2e\n\tphi phi: %.2e\n",
		dot_product_residual_theta_theta,
		dot_product_residual_theta_phi,
		dot_product_residual_phi_phi
	);

	(*e_theta) = (*best_solution.solution1);
	(*e_phi) = (*best_solution.solution2);

	return minimum_residual;
}
