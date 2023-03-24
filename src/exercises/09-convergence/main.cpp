#include "main.h"
using namespace std;
using namespace std::chrono;

double a = 1.5;
double b = 1;
double c = 1;
double delta_a = 0.01;

shared_ptr<Metric> metric = make_shared<EllipsoidMetric>(a, b, c);
shared_ptr<Dyad> initial_guess = make_shared<EllipsoidDyad>(a - delta_a, b, c);

shared_ptr<Grid> grid = nullptr;
char identifier[50];

ofstream solution_residuals_output("./assets/solution_residuals.csv", ios_base::app);
ofstream durations_output("./assets/durations.csv", ios_base::app);

void find_solution() {
	auto start_time = high_resolution_clock::now();

	shared_ptr<Grid3DFunction> embedding = make_shared<Grid3DFunction>(*grid);

	run_embedding(metric, embedding, identifier, initial_guess);

	shared_ptr<Grid3DFunction> ellipsoid = make_shared<Grid3DFunction>(*grid, [] (int i, int j, char coordinate) {
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
	});

	double solution_residual = embedding
		->added_with(ellipsoid, -1)
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
	for (int N_theta = 10; N_theta <= 40; N_theta += 5) {
		// int N_theta = 10;
		int N_phi = 2 * N_theta;
		// int N_phi = 4 * N_theta;
		grid = make_shared<Grid>(N_theta, N_phi);

		sprintf(identifier, "ellipsoid_%dx%d", N_theta, N_phi);

		printf(">>> %s <<<\n", identifier);

		find_solution();
	}
}
