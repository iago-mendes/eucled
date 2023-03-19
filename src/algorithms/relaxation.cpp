#include "relaxation.h"
using namespace std;

shared_ptr<Grid3DFunction> e_theta__relaxation(nullptr);
shared_ptr<Grid3DFunction> e_phi__relaxation(nullptr);
shared_ptr<Grid3DFunction> embedding__relaxation(nullptr);
shared_ptr<Metric> metric__relaxation(nullptr);

Grid *grid__relaxation;

// Free parameter
double gamma__relaxation = 1;

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
		get_cross_product(e_phi__relaxation, D_theta(commutator))
		->added_with(get_cross_product(e_theta__relaxation, D_phi(commutator)), -1)
	;
}

// Constraint from equation 31 for theta theta
shared_ptr<GridFunction> C_theta_theta() {
	return e_theta__relaxation
		->dot_product_with(e_theta__relaxation)
		->added_with([] (double theta, double phi) {return - metric__relaxation->g_theta_theta(theta, phi);})
	;
}

// Constraint from equation 31 for theta phi
shared_ptr<GridFunction> C_theta_phi() {
	return e_theta__relaxation
		->dot_product_with(e_phi__relaxation)
		->added_with([] (double theta, double phi) {return - metric__relaxation->g_theta_phi(theta, phi);})
	;
}

// Constraint from equation 31 for phi phi
shared_ptr<GridFunction> C_phi_phi() {
	return e_phi__relaxation
		->dot_product_with(e_phi__relaxation)
		->added_with([] (double theta, double phi) {return - metric__relaxation->g_phi_phi(theta, phi);})
	;
}

// Euler stepping for theta
void update_e_theta(double time_step) {
	// From equation 42
	shared_ptr<Grid3DFunction> e_theta_derivative =
		get_cross_product(omega(), e_theta__relaxation)
		->added_with(
			e_theta__relaxation
				->multiplied_by(gamma__relaxation)
				->multiplied_by(C_theta_theta())
		, -1)
		->added_with(
			e_phi__relaxation
				->multiplied_by(gamma__relaxation)
				->multiplied_by(C_theta_phi())
				->multiplied_by([] (double theta, [[maybe_unused]] double phi) {return 1/squared(sin(theta));})
		, -1)
	;

	e_theta__relaxation = e_theta__relaxation
		->added_with(e_theta_derivative, time_step);
		// ->added_with(e_theta_derivative->multiplied_by(time_step), [] (double theta, [[maybe_unused]] double phi) {return squared(sin(theta));});
		// ->added_with(e_theta_derivative->multiplied_by(time_step), [] (double theta, [[maybe_unused]] double phi) {return 1/sin(theta);});
}

// Euler stepping for phi
void update_e_phi(double time_step) {
	// From equation 43
	shared_ptr<Grid3DFunction> e_phi_derivative =
		get_cross_product(omega(), e_phi__relaxation)
		->added_with(
			e_theta__relaxation
				->multiplied_by(gamma__relaxation)
				->multiplied_by(C_theta_phi())
		, -1)
		->added_with(
			e_phi__relaxation
				->multiplied_by(gamma__relaxation)
				->multiplied_by(C_phi_phi())
				->multiplied_by([] (double theta, [[maybe_unused]] double phi) {return 1/squared(sin(theta));})
		, -1)
	;

	e_phi__relaxation = e_phi__relaxation
		->added_with(e_phi_derivative, time_step);
		// ->added_with(e_phi_derivative->multiplied_by(time_step), [] (double theta, [[maybe_unused]] double phi) {return squared(sin(theta));});
		// ->added_with(e_phi_derivative->multiplied_by(time_step), [] (double theta, [[maybe_unused]] double phi) {return 1/sin(theta);});
}

void update_embedding(double time_step, int i) {
	auto cot_theta = [](double theta, [[maybe_unused]] double phi) {return 1/tan(theta);};
	auto inverse_squared_sin_theta = [](double theta, [[maybe_unused]] double phi) {return 1/squared(sin(theta));};

	shared_ptr<Grid3DFunction> laplacian =
		embedding__relaxation->partial_theta()->partial_theta()
		->added_with(embedding__relaxation->partial_theta(), cot_theta)
		->added_with(embedding__relaxation->partial_phi()->partial_phi(), inverse_squared_sin_theta)
	;
	// printf("\tlaplacian = %e\n", laplacian->norm()->rms());

	shared_ptr<Grid3DFunction> source =
		e_theta__relaxation->partial_theta()
		->added_with(e_theta__relaxation, cot_theta)
		->added_with(e_phi__relaxation->partial_phi(), inverse_squared_sin_theta)
	;
	// printf("\tsource = %e\n", source->norm()->rms());

	shared_ptr<Grid3DFunction> embedding_derivative = laplacian->added_with(source, -1);
	if (i%50 == 0)
		printf("\tembedding_derivative = %e\n", embedding_derivative->norm()->rms());
	embedding__relaxation = embedding__relaxation->added_with(embedding_derivative, .02 * time_step);
}

double run_relaxation(
	shared_ptr<Grid3DFunction> e_theta,
	shared_ptr<Grid3DFunction> e_phi,
	shared_ptr<Grid3DFunction> embedding,
	shared_ptr<Metric> metric,
	double (*get_residual)(shared_ptr<Grid3DFunction> e_theta, shared_ptr<Grid3DFunction> e_phi),
	char *identifier
) {
	e_theta__relaxation = e_theta;
	e_phi__relaxation = e_phi;
	embedding__relaxation = embedding;
	grid__relaxation = &e_theta->grid;
	metric__relaxation = metric;
	// embedding->print();

	char residuals_filename[50];
	char constraints_filename[50];
	if (identifier != nullptr) {
		sprintf(residuals_filename, "./assets/residuals_%s.csv", identifier);
		sprintf(constraints_filename, "./assets/constraints_%s.csv", identifier);
	} else {
		sprintf(residuals_filename, "./assets/residuals_%d.csv", grid__relaxation->N_theta);
		sprintf(constraints_filename, "./assets/constraints.csv");
	}
	ofstream residuals_output(residuals_filename);
	ofstream residual_distribution_output("./assets/residual_distribution.csv");
	ofstream constraints_output(constraints_filename);

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

	double time_step = 0.01; // Good for 15 x 60
	time_step *= squared(15. / (double) grid__relaxation->N_theta);
	// double time_step = 2.0e-03; // Good for 15 x 60
	printf("Time step = %.2e\n", time_step);

	double residual= abs(get_residual(e_theta__relaxation, e_phi__relaxation));

	Iteration best_solution;
	best_solution.solution1 = e_theta;
	best_solution.solution2 = e_phi;
	best_solution.residual = INFINITY; // Should be replaced by another solution found after relaxation is done.

	// Solve.
	int iteration_number = 0;
	int max_iterations = MAX_ITERATIONS;
	bool started_decreasing = false;
	double prev_residual = residual;
	while (iteration_number < max_iterations) {
		if (iteration_number % OUTPUT_FREQUENCY == 0) {
		// if (iteration_number % 10 == 0) {
			printf("(%d) R = %8.2e, Embedding RMS = %e\n", iteration_number, residual, embedding__relaxation->norm()->rms());
		}
		
		update_e_theta(time_step);
		update_e_phi(time_step);
		update_embedding(time_step, iteration_number);

		residual = abs(get_residual(e_theta__relaxation, e_phi__relaxation));
		residuals_output << iteration_number << "," << residual << endl;

		constraints_output
			<< C_theta_theta()->multiplied_by([] (double theta, [[maybe_unused]] double phi) {return sin(theta);})->rms() << ","
			<< C_theta_phi()->multiplied_by([] (double theta, [[maybe_unused]] double phi) {return sin(theta);})->rms() << ","
			<< C_phi_phi()->multiplied_by([] (double theta, [[maybe_unused]] double phi) {return sin(theta);})->rms() << endl;

		if (!started_decreasing && iteration_number > 500 && iteration_number % 100 == 0) {
			if (residual < prev_residual) {
				started_decreasing = true;
			}
			prev_residual = residual;
		}

		if (started_decreasing && residual < best_solution.residual) {
			best_solution.solution1 = e_theta__relaxation;
			best_solution.solution2 = e_phi__relaxation;
			best_solution.residual = residual;
		} else if (started_decreasing && max_iterations == MAX_ITERATIONS) {
			// Run 100 more iterations after minimum residual was found.
			// max_iterations = iteration_number + 100;
			max_iterations = iteration_number;
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

	printf("Relaxation finished with R = %.2e after %d iterations.\n", best_solution.residual, iteration_number);

	for (int i = 0; i < 10000; i++) {
		if (i % 100 == 0) {
			printf("(%d) R = %8.2e, Embedding RMS = %e\n", i, residual, embedding__relaxation->norm()->rms());
		}
		update_embedding(time_step, i);
	}

  // double dot_product_residual_theta_theta =
	// 	e_theta->dot_product_with(e_theta)->added_with(
	// 		best_solution.solution1->dot_product_with(best_solution.solution1),
	// 		-1
	// 	)->rms();
	// double dot_product_residual_theta_phi =
	// 	e_theta->dot_product_with(e_phi)->added_with(
	// 		best_solution.solution1->dot_product_with(best_solution.solution2),
	// 		-1
	// 	)->rms();
	// double dot_product_residual_phi_phi =
	// 	e_phi->dot_product_with(e_phi)->added_with(
	// 		best_solution.solution2->dot_product_with(best_solution.solution2),
	// 		-1
	// 	)->rms();
	
	// printf(
	// 	"Dot product residuals:\n\ttheta theta: %.2e\n\ttheta phi: %.2e\n\tphi phi: %.2e\n",
	// 	dot_product_residual_theta_theta,
	// 	dot_product_residual_theta_phi,
	// 	dot_product_residual_phi_phi
	// );

	(*e_theta) = (*best_solution.solution1);
	(*e_phi) = (*best_solution.solution2);
	(*embedding) = (*embedding__relaxation);

	return best_solution.residual;
}
