#include "main.h"
using namespace std;
using namespace std::chrono;

Grid grid;

// Dimensions of ellipsoid
double a = 1.5;
double b = 1;
double c = 1;

double get_ellipsoid(int i, int j, char coordinate) {
	double theta = grid.theta(i);
	double phi = grid.phi(j);

	switch (coordinate) {
	case 'x':
		return a * cos(phi) * sin(theta);
		break;

	case 'y':
		return b * sin(phi) * sin(theta);
		break;

	case 'z':
		return c * cos(theta);
		break;

	default:
		return -1;
		break;
	}
}

double sin_sqrt_multiplier__main(
		double theta,
		[[maybe_unused]] double phi,
	[[maybe_unused]] char coordinate
) {
	return sqrt(sin(theta));
}

void find_solution(Grid grid, shared_ptr<Metric> metric, char *identifier = nullptr) {
	auto start_time = high_resolution_clock::now();

	shared_ptr<Grid3DFunction> embedding = make_shared<Grid3DFunction>(grid);

	run_embedding(metric, embedding, identifier);

	// shared_ptr<Grid3DFunction> ellipsoid = make_shared<Grid3DFunction>(grid, get_ellipsoid);
	// shared_ptr<Grid3DFunction> solution_residual_unweighted = embedding->added_with(ellipsoid, -1);
	// shared_ptr<Grid3DFunction> solution_residual = solution_residual_unweighted->multiplied_by(sin_sqrt_multiplier__main);
	// printf(
	// 	"Solution residuals:\n\tUnweighted: %e\n\tWeighted: %e\n",
	// 	solution_residual_unweighted->norm()->rms(),
	// 	solution_residual->norm()->rms()
	// );

	auto stop_time = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop_time - start_time).count();

	printf("Time duration: %ld ms\n", duration);

	printf("\n\nDone!\n");
}

int main() {
	int N_theta, N_phi;

	N_theta = 15;
	N_phi = 4 * N_theta;
	shared_ptr<Metric> metric = make_shared<PeanutMetric>();

	// Numeric metric input
	// shared_ptr<NumericalMetric> metric = make_shared<NumericalMetric>();
	// N_theta = metric->grid.N_theta;
	// N_phi = metric->grid.N_phi;

	Grid grid(N_theta, N_phi);

	char identifier[50];
	// sprintf(identifier, "numerical_%dx%d", N_theta, N_phi);
	sprintf(identifier, "peanut_%dx%d", N_theta, N_phi);

	// Single run.
	find_solution(grid, metric, identifier);

	// Vary grid space.
	// for (int N = 10; N <= 40; N *= 2) {
	// 	printf("\n%dx%d\n", N, 4*N);

	// 	find_solution(N, 4*N);
	// }
}
