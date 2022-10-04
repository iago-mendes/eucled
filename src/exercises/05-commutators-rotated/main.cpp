#include "main.h"
using namespace std;

Grid grid;
RotatedSphereDyad dyad;

double alpha = 10;

double constant_rotator([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	return 10;
}

double nonconstant_rotator(double theta, double phi) {
	return alpha * sin(theta) * cos(phi);
}

double get_e_phi_x(int i, int j) {
	double theta = grid.theta(i);
	double phi = grid.phi(j);

	return dyad.e_phi_x(theta, phi);
}

double get_e_phi_y(int i, int j) {
	double theta = grid.theta(i);
	double phi = grid.phi(j);

	return dyad.e_phi_y(theta, phi);
}

double get_e_phi_z(int i, int j) {
	double theta = grid.theta(i);
	double phi = grid.phi(j);

	return dyad.e_phi_z(theta, phi);
}

double get_e_theta_x(int i, int j) {
	double theta = grid.theta(i);
	double phi = grid.phi(j);

	return dyad.e_theta_x(theta, phi);
}

double get_e_theta_y(int i, int j) {
	double theta = grid.theta(i);
	double phi = grid.phi(j);

	return dyad.e_theta_y(theta, phi);
}

double get_e_theta_z(int i, int j) {
	double theta = grid.theta(i);
	double phi = grid.phi(j);

	return dyad.e_theta_z(theta, phi);
}

// Returns the overall RMS residual.
double check_finite_difference_derivatives(int N, double (*rotator)(double theta, double phi)) {
	Grid grid_(N, N);
	grid = grid_;

	RotatedSphereDyad dyad_(rotator);
	dyad = dyad_;

	printf("\n-------------------------------------------------------------\n");
	printf("N_theta = N_phi = %d\n\n", N);

	
	GridFunction e_phi_x(grid, get_e_phi_x);
	GridFunction e_phi_y(grid, get_e_phi_y);
	GridFunction e_phi_z(grid, get_e_phi_z);
	GridFunction e_theta_x(grid, get_e_theta_x);
	GridFunction e_theta_y(grid, get_e_theta_y);
	GridFunction e_theta_z(grid, get_e_theta_z);

	vector<double> x_residuals;
	vector<double> y_residuals;
	vector<double> z_residuals;

	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			double x_residual = e_phi_x.partial_theta(i, j) - e_theta_x.partial_phi(i, j);
			double y_residual = e_phi_y.partial_theta(i, j) - e_theta_y.partial_phi(i, j);
			double z_residual = e_phi_z.partial_theta(i, j) - e_theta_z.partial_phi(i, j);

			x_residuals.push_back(x_residual);
			y_residuals.push_back(y_residual);
			z_residuals.push_back(z_residual);
		}
	}

	double x_rms_residual = get_rms(&x_residuals);
	double y_rms_residual = get_rms(&y_residuals);
	double z_rms_residual = get_rms(&z_residuals);

	printf("x's RMS residual = %.5e\n", x_rms_residual);
	printf("y's RMS residual = %.5e\n", y_rms_residual);
	printf("z's RMS residual = %.5e\n", z_rms_residual);

	return (x_rms_residual + y_rms_residual + z_rms_residual) / 3;
}

int main() {
	printf("Exercise 4: Check commutators for oblate spheroid.\n");

	printf("\n=============================================================\n");
	printf("CONSTANT ROTATOR\n");
	printf("=============================================================\n");

	ofstream constant_residuals_output("./assets/constant_residuals.csv");
	for (int N = 10; N <= 2000; N *= 2) {
		double residual = check_finite_difference_derivatives(N, constant_rotator);
		constant_residuals_output << N << "," << residual << endl;
	}

	printf("\n=============================================================\n");
	printf("NON-CONSTANT ROTATOR\n");
	printf("=============================================================\n");

	ofstream nonconstant_residuals_output("./assets/nonconstant_residuals.csv");
	for (int N = 10; N <= 2000; N *= 2) {
		double residual = check_finite_difference_derivatives(N, nonconstant_rotator);
		nonconstant_residuals_output << N << "," << residual << endl;
	}
}
