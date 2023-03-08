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
	return this->delta_theta * (i + (double) 1 / 2);
}

double Grid::phi(int j) {
	return this->delta_phi * j;
}

int Grid::i(double theta) {
	return (int) (theta / this->delta_theta - 1 / 2);
}

int Grid::j(double phi) {
	return (int) (phi / this->delta_phi);
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

	cached_partial_theta = nullptr;
	cached_partial_phi = nullptr;
}

GridFunction::GridFunction(Grid grid, vector<vector<double>> *points) {
	this->grid = grid;

	vector<double> base_vector(grid.N_phi, 0);
	this->points.resize(grid.N_theta, base_vector);

	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			this->points[i][j] = (*points)[i][j];
		}
	}

	this->cached_partial_theta = nullptr;
	this->cached_partial_phi = nullptr;
}

GridFunction::GridFunction(Grid grid) {
	this->grid = grid;

	vector<double> base_vector(grid.N_phi, 0);
	this->points.resize(grid.N_theta, base_vector);

	this->cached_partial_theta = nullptr;
	this->cached_partial_phi = nullptr;
}

shared_ptr<GridFunction> GridFunction::get_copy() {
	auto copy = make_shared<GridFunction>(grid);

	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			copy->points[i][j] = this->points[i][j];
		}
	}

	return copy;
}

shared_ptr<GridFunction> GridFunction::partial_theta() {
	if (cached_partial_theta != nullptr) {
		return cached_partial_theta;
	}

	auto new_function = get_copy();

	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			if (i == 0) {
				new_function->points[i][j] = (points[1][j] - points[0][j]) / grid.delta_theta;
			}
			else if (i == grid.N_theta - 1) {
				new_function->points[i][j] = (points[grid.N_theta - 1][j] - points[grid.N_theta - 2][j]) / grid.delta_theta;
			}
			else {
				new_function->points[i][j] = (points[i + 1][j] - points[i - 1][j]) / (2 * grid.delta_theta);
			}
		}
	}

	cached_partial_theta = new_function;

	return new_function;
}

shared_ptr<GridFunction> GridFunction::partial_phi() {
	if (cached_partial_phi != nullptr) {
		return cached_partial_phi;
	}

	auto new_function = get_copy();

	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
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
			
			new_function->points[i][j] = df / (2 * grid.delta_phi);
		}
	}

	cached_partial_phi = new_function;

	return new_function;
}

double GridFunction::rms() {
	return get_rms(&this->points);
}

shared_ptr<GridFunction> GridFunction::added_with(shared_ptr<GridFunction> function, double multiplier) {
	shared_ptr<GridFunction> new_function = this->get_copy();

	for (int i = 0; i < this->grid.N_theta; i++) {
		for (int j = 0; j < this->grid.N_phi; j++) {
			new_function->points[i][j] += multiplier * function->points[i][j];
		}
	}

	return new_function;
}

shared_ptr<GridFunction> GridFunction::added_with(double (*function)(double theta, double phi)) {
	shared_ptr<GridFunction> new_function = this->get_copy();

	for (int i = 0; i < this->grid.N_theta; i++) {
		double theta = this->grid.theta(i);

		for (int j = 0; j < this->grid.N_phi; j++) {
			double phi = this->grid.phi(j);

			new_function->points[i][j] += function(theta, phi);
		}
	}

	return new_function;
}

shared_ptr<GridFunction> GridFunction::multiplied_by(double (*multiplier)(double theta, double phi)) {
	shared_ptr<GridFunction> new_function = get_copy();

	for (int i = 0; i < grid.N_theta; i++) {
		double theta = grid.theta(i);

		for (int j = 0; j < grid.N_phi; j++) {
			double phi = grid.phi(j);
			
			new_function->points[i][j] *= multiplier(theta, phi);
		}
	}

	return new_function;
}

void GridFunction::print() {
	for (int i = 0; i < this->grid.N_theta; i++) {
		printf("\t");
		for (int j = 0; j < this->grid.N_phi; j++) {
			printf("%9.2e ", this->points[i][j]);
		}
		printf("\n");
	}
}

void GridFunction::print(string identifier) {
	cout << identifier << endl;
	this->print();
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

Grid3DFunction::Grid3DFunction(Grid grid) {
	this->grid = grid;

	vector<double> base_vector(grid.N_phi, 0);
	x_values.resize(grid.N_theta, base_vector);
	y_values.resize(grid.N_theta, base_vector);
	z_values.resize(grid.N_theta, base_vector);
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
	if (cached_norm != nullptr) {
		return cached_norm;
	}

	auto norm = make_shared<GridFunction>(grid, zero_function_alt);

	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			norm->points[i][j] = sqrt( squared(x_values[i][j]) + squared(y_values[i][j]) + squared(z_values[i][j]) );
		}
	}

	cached_norm = norm;

	return norm;
}

shared_ptr<Grid3DFunction> Grid3DFunction::partial_theta() {
	if (cached_partial_theta != nullptr) {
		return cached_partial_theta;
	}

	auto new_function = get_copy();

	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			if (i == 0) { // forward
				new_function->x_values[i][j] = (- 3 * x_values[0][j] + 4 * x_values[1][j] - x_values[2][j]) / (2 * grid.delta_theta);
				new_function->y_values[i][j] = (- 3 * y_values[0][j] + 4 * y_values[1][j] - y_values[2][j]) / (2 * grid.delta_theta);
				new_function->z_values[i][j] = (- 3 * z_values[0][j] + 4 * z_values[1][j] - z_values[2][j]) / (2 * grid.delta_theta);
			}
			else if (i == grid.N_theta - 1) { // backward
				new_function->x_values[i][j] = (3 * x_values[grid.N_theta - 1][j] - 4 * x_values[grid.N_theta - 2][j] + x_values[grid.N_theta - 3][j]) / (2 * grid.delta_theta);
				new_function->y_values[i][j] = (3 * y_values[grid.N_theta - 1][j] - 4 * y_values[grid.N_theta - 2][j] + y_values[grid.N_theta - 3][j]) / (2 * grid.delta_theta);
				new_function->z_values[i][j] = (3 * z_values[grid.N_theta - 1][j] - 4 * z_values[grid.N_theta - 2][j] + z_values[grid.N_theta - 3][j]) / (2 * grid.delta_theta);
			}
			else { // centered
				new_function->x_values[i][j] = (x_values[i + 1][j] - x_values[i - 1][j]) / (2 * grid.delta_theta);
				new_function->y_values[i][j] = (y_values[i + 1][j] - y_values[i - 1][j]) / (2 * grid.delta_theta);
				new_function->z_values[i][j] = (z_values[i + 1][j] - z_values[i - 1][j]) / (2 * grid.delta_theta);
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

shared_ptr<Grid3DFunction> Grid3DFunction::multiplied_by(double (*multiplier)(double theta, double phi)) {
	auto new_function = get_copy();

	for (int i = 0; i < grid.N_theta; i++) {
		double theta = grid.theta(i);

		for (int j = 0; j < grid.N_phi; j++) {
			double phi = grid.phi(j);
			
			(*new_function).x_values[i][j] *= multiplier(theta, phi);
			(*new_function).y_values[i][j] *= multiplier(theta, phi);
			(*new_function).z_values[i][j] *= multiplier(theta, phi);
		}
	}

	return new_function;
}

shared_ptr<Grid3DFunction> Grid3DFunction::multiplied_by(shared_ptr<GridFunction> multiplier) {
	auto new_function = get_copy();

	for (int i = 0; i < grid.N_theta; i++) {
		for (int j = 0; j < grid.N_phi; j++) {
			(*new_function).x_values[i][j] *= multiplier->points[i][j];
			(*new_function).y_values[i][j] *= multiplier->points[i][j];
			(*new_function).z_values[i][j] *= multiplier->points[i][j];
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
	double (*multiplier)(double theta, double phi)
) {
	auto new_function = get_copy();

	for (int i = 0; i < grid.N_theta; i++) {
		double theta = grid.theta(i);

		for (int j = 0; j < grid.N_phi; j++) {
			double phi = grid.phi(j);

			(*new_function).x_values[i][j] += multiplier(theta, phi) * (*function).x_values[i][j];
			(*new_function).y_values[i][j] += multiplier(theta, phi) * (*function).y_values[i][j];
			(*new_function).z_values[i][j] += multiplier(theta, phi) * (*function).z_values[i][j];
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

shared_ptr<GridFunction> Grid3DFunction::dot_product_with(shared_ptr<Grid3DFunction> function) {
	shared_ptr<GridFunction> dot_product = make_shared<GridFunction>(this->grid);	

	for (int i = 0; i < this->grid.N_theta; i++) {
		for (int j = 0; j < this->grid.N_phi; j++) {
			dot_product->points[i][j] =
				this->x_values[i][j] * function->x_values[i][j] +
				this->y_values[i][j] * function->y_values[i][j] +
				this->z_values[i][j] * function->z_values[i][j];
		}
	}
	
	return dot_product;
}

void Grid3DFunction::print() {
	printf("\tx:\n");
	for (int i = 0; i < this->grid.N_theta; i++) {
		printf("\t\t");
		for (int j = 0; j < this->grid.N_phi; j++) {
			printf("%9.2e ", this->x_values[i][j]);
		}
		printf("\n");
	}

	printf("\ty:\n");
	for (int i = 0; i < this->grid.N_theta; i++) {
		printf("\t\t");
		for (int j = 0; j < this->grid.N_phi; j++) {
			printf("%9.2e ", this->y_values[i][j]);
		}
		printf("\n");
	}

	printf("\tz:\n");
	for (int i = 0; i < this->grid.N_theta; i++) {
		printf("\t\t");
		for (int j = 0; j < this->grid.N_phi; j++) {
			printf("%9.2e ", this->z_values[i][j]);
		}
		printf("\n");
	}
}

void Grid3DFunction::print(string identifier) {
	cout << identifier << endl;
	this->print();
}
