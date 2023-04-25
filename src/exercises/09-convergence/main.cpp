#include "main.h"
using namespace std;
using namespace std::chrono;

shared_ptr<Grid> grid = nullptr;
char identifier[50];

ofstream solution_residuals_output("./assets/solution_residuals.csv", ios_base::app);
ofstream durations_output("./assets/durations.csv", ios_base::app);

// Ellipsoid
double a = 1.5;
double b = 1;
double c = 1;
auto ellipsoid_metric = make_shared<EllipsoidMetric>(a, b, c);
auto analytical_ellipsoid = [] (int i, int j, char coordinate) {
	double theta = grid->theta(i);
	double phi = grid->phi(j);
	switch (coordinate) {
		case 'x':
			return a * cos(phi) * sin(theta);
		case 'y':
			return b * sin(phi) * sin(theta);
		case 'z':
			return c * cos(theta);
		default:
			return -1.;
	}
};

// // Non-axisymmetric z-Peanut
// double r0 = 1.;
// double b = .7;
// double e = 0;
// auto nonaxi_peanut_metric = make_shared<NonaxisymmetricZPeanutMetric>(r0, b, e);
// auto analytical_nonaxi_peanut = [] (double theta, double phi, char coordinate) {
// 	double r = r0 - b * squared(sin(theta)) + e * cos(theta) * sin(theta) * cos(phi);
// 	switch (coordinate) {
// 		case 'x':
// 			return r * sin(theta) * cos(phi);
// 		case 'y':
// 			return r * sin(theta) * sin(phi);
// 		case 'z':
// 			return r * cos(theta);
// 		default:
// 			return -1.;
// 	}
// };

void find_solution() {
	auto start_time = high_resolution_clock::now();

	shared_ptr<Grid3DFunction> embedding = make_shared<Grid3DFunction>(*grid);

	// Determine which surface to use
	auto metric = ellipsoid_metric;
	auto expected_embedding_expression = analytical_ellipsoid;

	// double final_time = 100; // For 15 x 30
	// final_time *= (grid->N_theta / 15.); // t_f \propto \sqrt N_\theta
	// // printf("Final time: %f\n", final_time);

	run_embedding(metric, embedding, identifier, 100.);

	auto expected_embedding = make_shared<Grid3DFunction>(*grid, expected_embedding_expression);
	double solution_residual = embedding
		->added_with(expected_embedding, -1)
		->multiplied_by([] (double theta, [[maybe_unused]] double phi) {return sin(theta);})
		->norm()
		->rms();
	
	printf("Solution residual: %e\n", solution_residual);
	solution_residuals_output << grid->N_theta << "," << solution_residual << endl;

	auto stop_time = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop_time - start_time).count();

	printf("Time duration: %f min\n", duration / 60.);
	durations_output << grid->N_theta << "," << duration << endl;

	printf("\n\nDone!\n\n");
}

int main() {
	for (int N_theta = 10; N_theta <= 50; N_theta += 5) {
		// int N_theta = 10;
		int N_phi = 2 * N_theta;
		// int N_phi = 4 * N_theta;
		grid = make_shared<Grid>(N_theta, N_phi);

		sprintf(identifier, "ellipsoid_%dx%d", N_theta, N_phi);
		// sprintf(identifier, "nonaxi_peanut_%dx%d", N_theta, N_phi);

		// double a = 1;
		// shared_ptr<Metric> metric = make_shared<PeanutMetric>(a);
		// sprintf(identifier, "zpeanut_%.1f_%dx%d", a, N_theta, N_phi);

		printf(">>> %s <<<\n", identifier);

		find_solution();
	}
}
