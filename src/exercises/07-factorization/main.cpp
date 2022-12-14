#include "main.h"
using namespace std;

// Semi-axes of oblate spheroid
double a = 1;
double b = 2;

Grid grid;
RoundSphereDyad round_sphere_dyad(a);
shared_ptr<GridFunction> round_sphere_e_theta_norm;
shared_ptr<GridFunction> round_sphere_e_phi_norm;
OblateSpheroidDyad oblate_spheroid_dyad(a, b);

// Input
shared_ptr<OblateSpheroidMetric> metric;

// Output
shared_ptr<Grid3DFunction> e_phi;
shared_ptr<Grid3DFunction> e_theta;

double get_e_phi(int i, int j, char coordinate) {
	double theta = grid.theta(i);
	double phi = grid.phi(j);

	double multiplier = sqrt(metric->g_phi_phi(theta, phi)) / round_sphere_e_phi_norm->points[i][j];

	switch (coordinate) {
		case 'x':
			return multiplier * round_sphere_dyad.e_phi_x(theta, phi);
		
		case 'y':
			return multiplier * round_sphere_dyad.e_phi_y(theta, phi);

		case 'z':
			return multiplier * round_sphere_dyad.e_phi_z(theta, phi);
		
		default:
			return -1;
	}
}

double get_e_theta(int i, int j, char coordinate) {
	double theta = grid.theta(i);
	double phi = grid.phi(j);

	double multiplier_theta = sqrt(
		metric->g_theta_theta(theta, phi) - squared(metric->g_theta_phi(theta, phi)) / metric->g_phi_phi(theta, phi)
	) / round_sphere_e_theta_norm->points[i][j];

	double multiplier_phi = (
		metric->g_theta_phi(theta, phi) / sqrt(metric->g_phi_phi(theta, phi))
	) / round_sphere_e_phi_norm->points[i][j];

	switch (coordinate) {
		case 'x':
			return multiplier_theta * round_sphere_dyad.e_theta_x(theta, phi) + multiplier_phi * round_sphere_dyad.e_phi_x(theta, phi);
		
		case 'y':
			return multiplier_theta * round_sphere_dyad.e_theta_y(theta, phi) + multiplier_phi * round_sphere_dyad.e_phi_y(theta, phi);

		case 'z':
			return multiplier_theta * round_sphere_dyad.e_theta_z(theta, phi) + multiplier_phi * round_sphere_dyad.e_phi_z(theta, phi);
		
		default:
			return -1;
	}
}

double negate_multiplier([[maybe_unused]] double theta, [[maybe_unused]] double phi, [[maybe_unused]] char coordinate) {
	return - 1;
}

void find_solution(int N_theta, int N_phi) {
	Grid grid_(N_theta, N_phi);
	grid = grid_;

	// Compute round sphere dyad norm.
	shared_ptr<Grid3DFunction> round_sphere_e_theta = round_sphere_dyad.get_theta_function(grid);
	shared_ptr<Grid3DFunction> round_sphere_e_phi = round_sphere_dyad.get_phi_function(grid);
	round_sphere_e_theta_norm = round_sphere_e_theta->norm();
	round_sphere_e_phi_norm = round_sphere_e_phi->norm();

	e_phi = make_shared<Grid3DFunction>(grid, get_e_phi);
	e_theta = make_shared<Grid3DFunction>(grid, get_e_theta);
}

int main() {
	metric = make_shared<OblateSpheroidMetric>(a, b);

	find_solution(10, 10);

	auto residual_theta = e_theta->added_with(oblate_spheroid_dyad.get_theta_function(grid), negate_multiplier);
	auto residual_phi = e_phi->added_with(oblate_spheroid_dyad.get_phi_function(grid), negate_multiplier);

	printf("R(e_theta) = %.5e\n", residual_theta->rms());
	printf("R(e_phi) = %.5e\n", residual_phi->rms());
}
