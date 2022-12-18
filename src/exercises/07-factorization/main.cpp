#include "main.h"
using namespace std;

// Rotation of black hole horizon.
double chi = 0;

Grid grid;
HorizonDyad expected_dyad(chi);

// Input
shared_ptr<HorizonMetric> metric = make_shared<HorizonMetric>(chi);

// Output
shared_ptr<Grid3DFunction> e_theta;
shared_ptr<Grid3DFunction> e_phi;

double negate_multiplier([[maybe_unused]] double theta, [[maybe_unused]] double phi, [[maybe_unused]] char coordinate) {
	return - 1;
}

void find_solution(int N_theta, int N_phi) {
	Grid grid_(N_theta, N_phi);
	grid = grid_;

	e_theta = make_shared<Grid3DFunction>(grid);
	e_phi = make_shared<Grid3DFunction>(grid);

	run_factorization(metric, e_theta, e_phi);
}

int main() {
	find_solution(30, 120);

	auto residual_theta = e_theta->added_with(expected_dyad.get_theta_function(grid), negate_multiplier);
	auto residual_phi = e_phi->added_with(expected_dyad.get_phi_function(grid), negate_multiplier);

	printf("R(e_theta) = %.5e\n", residual_theta->rms());
	printf("R(e_phi) = %.5e\n", residual_phi->rms());
}
