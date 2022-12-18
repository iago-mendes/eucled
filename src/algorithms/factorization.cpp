#include "factorization.h"
using namespace std;

shared_ptr<Metric> metric__factorization;
Grid grid__factorization;
RoundSphereDyad round_sphere_dyad__factorization(1);
shared_ptr<GridFunction> round_sphere_e_theta_norm__factorization;
shared_ptr<GridFunction> round_sphere_e_phi_norm__factorization;

double get_e_theta__factorization(int i, int j, char coordinate) {
	double theta = grid__factorization.theta(i);
	double phi = grid__factorization.phi(j);

	double multiplier_theta = sqrt(
		metric__factorization->g_theta_theta(theta, phi) - squared(metric__factorization->g_theta_phi(theta, phi)) / metric__factorization->g_phi_phi(theta, phi)
	) / round_sphere_e_theta_norm__factorization->points[i][j];

	double multiplier_phi = (
		metric__factorization->g_theta_phi(theta, phi) / sqrt(metric__factorization->g_phi_phi(theta, phi))
	) / round_sphere_e_phi_norm__factorization->points[i][j];

	switch (coordinate) {
		case 'x':
			return multiplier_theta * round_sphere_dyad__factorization.e_theta_x(theta, phi) + multiplier_phi * round_sphere_dyad__factorization.e_phi_x(theta, phi);
		
		case 'y':
			return multiplier_theta * round_sphere_dyad__factorization.e_theta_y(theta, phi) + multiplier_phi * round_sphere_dyad__factorization.e_phi_y(theta, phi);

		case 'z':
			return multiplier_theta * round_sphere_dyad__factorization.e_theta_z(theta, phi) + multiplier_phi * round_sphere_dyad__factorization.e_phi_z(theta, phi);
		
		default:
			return -1;
	}
}

double get_e_phi__factorization(int i, int j, char coordinate) {
	double theta = grid__factorization.theta(i);
	double phi = grid__factorization.phi(j);

	double multiplier = sqrt(metric__factorization->g_phi_phi(theta, phi)) / round_sphere_e_phi_norm__factorization->points[i][j];

	switch (coordinate) {
		case 'x':
			return multiplier * round_sphere_dyad__factorization.e_phi_x(theta, phi);
		
		case 'y':
			return multiplier * round_sphere_dyad__factorization.e_phi_y(theta, phi);

		case 'z':
			return multiplier * round_sphere_dyad__factorization.e_phi_z(theta, phi);
		
		default:
			return -1;
	}
}

void run_factorization(
	shared_ptr<Metric> metric,
	shared_ptr<Grid3DFunction> e_theta,
	shared_ptr<Grid3DFunction> e_phi
) {
	metric__factorization = metric;
	grid__factorization = e_theta->grid;

	// Compute round sphere dyad norm.
	shared_ptr<Grid3DFunction> round_sphere_e_theta = round_sphere_dyad__factorization.get_theta_function(grid__factorization);
	shared_ptr<Grid3DFunction> round_sphere_e_phi = round_sphere_dyad__factorization.get_phi_function(grid__factorization);
	round_sphere_e_theta_norm__factorization = round_sphere_e_theta->norm();
	round_sphere_e_phi_norm__factorization = round_sphere_e_phi->norm();

	// Compute factorization.
	shared_ptr<Grid3DFunction> e_theta_ = make_shared<Grid3DFunction>(grid__factorization, get_e_theta__factorization);
	(*e_theta) = *e_theta_;
	shared_ptr<Grid3DFunction> e_phi_ = make_shared<Grid3DFunction>(grid__factorization, get_e_phi__factorization);
	(*e_phi) = *e_phi_;
}
