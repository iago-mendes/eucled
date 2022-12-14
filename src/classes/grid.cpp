#include "grid.h"
using namespace std;

// Miscellaneous

double zero_function([[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] char coordinate) {
	return 0;
}

double zero_function_alt([[maybe_unused]] int i, [[maybe_unused]] int j) {
	return 0;
}

// Grid

Grid::Grid(int N_theta_, int N_phi_) {
	N_theta = N_theta_;
	N_phi = N_phi_;

	delta_theta = M_PI / N_theta;
	delta_phi = 2 * M_PI / N_phi;
}

double Grid::theta(int i) {
	return M_PI / N_theta * (i + (double) 1 / 2);
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

	cached_partial_theta = nullptr;
	cached_partial_phi = nullptr;
}

Grid3DFunction::Grid3DFunction(Grid grid) {
	this->grid = grid;

	vector<double> base_vector(grid.N_phi, 0);
	x_values.resize(grid.N_theta, base_vector);
	y_values.resize(grid.N_theta, base_vector);
	z_values.resize(grid.N_theta, base_vector);

	cached_partial_theta = nullptr;
	cached_partial_phi = nullptr;
}

double Grid3DFunction::rms() {
	double x_rms = get_rms(&x_values);
	double y_rms = get_rms(&y_values);
	double z_rms = get_rms(&z_values);

	double rms = (x_rms + y_rms + z_rms) / 3;
	return rms;
}

shared_ptr<Grid3DFunction> Grid3DFunction::get_copy() {
	auto copy = make_shared<Grid3DFunction>(grid, zero_function);

	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			(*copy).x_values[i][j] = x_values[i][j];
			(*copy).y_values[i][j] = y_values[i][j];
			(*copy).z_values[i][j] = z_values[i][j];
		}
	}

	return copy;
}

shared_ptr<GridFunction> Grid3DFunction::norm() {
	auto norm = make_shared<GridFunction>(grid, zero_function_alt);

	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			norm->points[i][j] = sqrt( squared(x_values[i][j]) + squared(y_values[i][j]) + squared(z_values[i][j]) );
		}
	}

	return norm;
}

shared_ptr<Grid3DFunction> Grid3DFunction::partial_theta() {
	if (cached_partial_theta != nullptr) {
		return cached_partial_theta;
	}

	auto new_function = get_copy();

	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			if (i == 0) {
				(*new_function).x_values[i][j] = (x_values[1][j] - x_values[0][j]) / grid.delta_theta;
				(*new_function).y_values[i][j] = (y_values[1][j] - y_values[0][j]) / grid.delta_theta;
				(*new_function).z_values[i][j] = (z_values[1][j] - z_values[0][j]) / grid.delta_theta;
			}
			else if (i == grid.N_theta - 1) {
				(*new_function).x_values[i][j] = (x_values[grid.N_theta - 1][j] - x_values[grid.N_theta - 2][j]) / grid.delta_theta;
				(*new_function).y_values[i][j] = (y_values[grid.N_theta - 1][j] - y_values[grid.N_theta - 2][j]) / grid.delta_theta;
				(*new_function).z_values[i][j] = (z_values[grid.N_theta - 1][j] - z_values[grid.N_theta - 2][j]) / grid.delta_theta;
			}
			else {
				(*new_function).x_values[i][j] = (x_values[i + 1][j] - x_values[i - 1][j]) / (2 * grid.delta_theta);
				(*new_function).y_values[i][j] = (y_values[i + 1][j] - y_values[i - 1][j]) / (2 * grid.delta_theta);
				(*new_function).z_values[i][j] = (z_values[i + 1][j] - z_values[i - 1][j]) / (2 * grid.delta_theta);
			}
		}
	}

	cached_partial_theta = new_function;

	return new_function;
}

shared_ptr<Grid3DFunction> Grid3DFunction::partial_phi() {
	if (cached_partial_phi != nullptr) {
		return cached_partial_phi;
	}

	auto new_function = get_copy();

	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			if (j == 0) {
				(*new_function).x_values[i][j] = (x_values[i][1] - x_values[i][grid.N_phi - 1]) / (2 * grid.delta_phi);
				(*new_function).y_values[i][j] = (y_values[i][1] - y_values[i][grid.N_phi - 1]) / (2 * grid.delta_phi);
				(*new_function).z_values[i][j] = (z_values[i][1] - z_values[i][grid.N_phi - 1]) / (2 * grid.delta_phi);
			}
			else if (j == grid.N_phi - 1) {
				(*new_function).x_values[i][j] = (x_values[i][0] - x_values[i][grid.N_phi - 2]) / (2 * grid.delta_phi);
				(*new_function).y_values[i][j] = (y_values[i][0] - y_values[i][grid.N_phi - 2]) / (2 * grid.delta_phi);
				(*new_function).z_values[i][j] = (z_values[i][0] - z_values[i][grid.N_phi - 2]) / (2 * grid.delta_phi);
			}
			else {
				(*new_function).x_values[i][j] = (x_values[i][j + 1] - x_values[i][j - 1]) / (2 * grid.delta_phi);
				(*new_function).y_values[i][j] = (y_values[i][j + 1] - y_values[i][j - 1]) / (2 * grid.delta_phi);
				(*new_function).z_values[i][j] = (z_values[i][j + 1] - z_values[i][j - 1]) / (2 * grid.delta_phi);
			}
		}
	}

	cached_partial_phi = new_function;

	return new_function;
}

shared_ptr<Grid3DFunction> Grid3DFunction::multiplied_by(double (*multiplier)(double theta, double phi, char coordinate)) {
	auto new_function = get_copy();

	for (int i = 0; i < grid.N_theta; i++) {
		double theta = grid.theta(i);

		for (int j = 0; j < grid.N_phi; j++) {
			double phi = grid.phi(j);
			
			(*new_function).x_values[i][j] *= multiplier(theta, phi, 'x');
			(*new_function).y_values[i][j] *= multiplier(theta, phi, 'y');
			(*new_function).z_values[i][j] *= multiplier(theta, phi, 'z');
		}
	}

	return new_function;
}

shared_ptr<Grid3DFunction> Grid3DFunction::multiplied_by(double multiplier) {
	auto new_function = get_copy();

	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			(*new_function).x_values[i][j] *= multiplier;
			(*new_function).y_values[i][j] *= multiplier;
			(*new_function).z_values[i][j] *= multiplier;
		}
	}

	return new_function;
}

shared_ptr<Grid3DFunction> Grid3DFunction::added_with(
	shared_ptr<Grid3DFunction> function,
	double (*multiplier)(double theta, double phi, char coordinate)
) {
	auto new_function = get_copy();

	for (int i = 0; i < grid.N_theta; i++) {
		double theta = grid.theta(i);

		for (int j = 0; j < grid.N_phi; j++) {
			double phi = grid.phi(j);

			(*new_function).x_values[i][j] += multiplier(theta, phi, 'x') * (*function).x_values[i][j];
			(*new_function).y_values[i][j] += multiplier(theta, phi, 'y') * (*function).y_values[i][j];
			(*new_function).z_values[i][j] += multiplier(theta, phi, 'z') * (*function).z_values[i][j];
		}
	}

	return new_function;
}

shared_ptr<Grid3DFunction> Grid3DFunction::added_with(
	shared_ptr<Grid3DFunction> function,
	double multiplier
) {
	auto new_function = get_copy();

	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			(*new_function).x_values[i][j] += multiplier * (*function).x_values[i][j];
			(*new_function).y_values[i][j] += multiplier * (*function).y_values[i][j];
			(*new_function).z_values[i][j] += multiplier * (*function).z_values[i][j];
		}
	}

	return new_function;
}
