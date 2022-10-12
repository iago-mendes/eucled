#include "grid.h"
using namespace std;

// Grid

Grid::Grid(int N_theta_, int N_phi_) {
	N_theta = N_theta_;
	N_phi = N_phi_;

	delta_theta = M_PI / N_theta;
	delta_phi = 2 * M_PI / N_phi;
}

double Grid::theta(int i) {
	return M_PI / N_theta * (i + 1 / 2);
}

double Grid::phi(int j) {
	return 2 * M_PI / N_theta * j;
}

// GridFunction

GridFunction::GridFunction(Grid grid_, double (*function)(int i, int j)) {
	grid = grid_;

	vector<double> base_vector(grid.N_phi, 0);
	points.resize(grid.N_theta, base_vector);

	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			points[i][j] = function(i, j);
		}
	}
}

double GridFunction::partial_theta(int i, int j) {
	if (i == 0) {
		return (points[1][j] - points[0][j]) / grid.delta_theta;
	}
	else if (i == grid.N_theta - 1) {
		return (points[grid.N_theta - 1][j] - points[grid.N_theta - 2][j]) / grid.delta_theta;
	}
	else {
		return (points[i + 1][j] - points[i - 1][j]) / (2 * grid.delta_theta);
	}
}

double GridFunction::partial_phi(int i, int j) {
	double df;

	if (j == 0) {
		df = points[i][1] - points[i][grid.N_phi - 1];
	}
	else if (j == grid.N_phi - 1) {
		df = points[i][0] - points[i][grid.N_phi - 2];
	}
	else {
		df = points[i][j + 1] - points[i][j - 1];
	}

	return df / (2 * grid.delta_phi);
}

// Grid3DFunction

Grid3DFunction::Grid3DFunction(Grid grid_, double (*function)(int i, int j, char coordinate)) {
	grid = grid_;

	vector<double> base_vector(grid.N_phi, 0);
	x_values.resize(grid.N_theta, base_vector);
	y_values.resize(grid.N_theta, base_vector);
	z_values.resize(grid.N_theta, base_vector);

	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			x_values[i][j] = function(i, j, 'x');
			y_values[i][j] = function(i, j, 'y');
			z_values[i][j] = function(i, j, 'z');
		}
	}
}

double Grid3DFunction::rms() {
	double x_rms = get_rms(&x_values);
	double y_rms = get_rms(&y_values);
	double z_rms = get_rms(&z_values);

	double rms = (x_rms + y_rms + z_rms) / 3;
	return rms;
}
