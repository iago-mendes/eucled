#include "main.h"
using namespace std;
using namespace std::chrono;

void find_solution(Grid grid, shared_ptr<Metric> metric, char *identifier = nullptr) {
	auto start_time = high_resolution_clock::now();

	auto round_sphere = [](double theta, double phi, char coord) {
		switch (coord) {
			case 'x':
				return sin(theta) * cos(phi);
			case 'y':
				return sin(theta) * sin(phi);
			case 'z':
				return cos(theta);
			default:
				return 0.;
		}
	};

	shared_ptr<Grid3DFunction> embedding = make_shared<Grid3DFunction>(grid, round_sphere);

	run_embedding(metric, embedding, identifier);

	auto stop_time = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop_time - start_time).count();

	printf("Time duration: %f min\n", duration / 60.0);

	printf("\n\nDone!\n");
}

int main() {
	int N_theta, N_phi;
	char identifier[50];

	N_theta = 15;
	N_phi = 2 * N_theta;

	// Numeric metric input
	// shared_ptr<NumericalMetric> metric = make_shared<NumericalMetric>();
	// N_theta = metric->grid.N_theta;
	// N_phi = metric->grid.N_phi;
	// sprintf(identifier, "numerical_%dx%d", N_theta, N_phi);

	Grid grid(N_theta, N_phi);

	// shared_ptr<Metric> metric = make_shared<NonaxisymmetricZPeanutMetric>(1., .7, .1, grid);
	// sprintf(identifier, "nonaxi_peanut_%dx%d", N_theta, N_phi);
	shared_ptr<Metric> metric = make_shared<EllipsoidMetric>(1.5, 1, 1);
	sprintf(identifier, "ellipsoid_%dx%d", N_theta, N_phi);
	// shared_ptr<Metric> metric = make_shared<RoundSphereMetric>(1);
	// sprintf(identifier, "round_%dx%d", N_theta, N_phi);

	printf(">>> %s <<<\n", identifier);

	find_solution(grid, metric, identifier);
}
