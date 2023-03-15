#include "integration.h"
using namespace std;

Grid grid__integration;
int N__integration;

ofstream rightside_x_distribution_output("./assets/rightside_x_distribution.csv");
ofstream rightside_y_distribution_output("./assets/rightside_y_distribution.csv");
ofstream rightside_z_distribution_output("./assets/rightside_z_distribution.csv");

int ij_to_I(int i, int j) {
	int I = i + grid__integration.N_theta * j;

	return I;
}

int I_to_i(int I) {
	int i = I % grid__integration.N_theta;

	return i;
}

int I_to_j(int I) {
	int j = I / grid__integration.N_theta;

	return j;
}

/**
 * Converts a 2D matrix to a 1D vector.
*/
void R2_to_R1(const vector<vector<double>> *src_R2, vector<double> *dest_R1) {
	for (int i = 0; i < grid__integration.N_theta; i++) {
		for (int j = 0; j < grid__integration.N_phi; j++) {
			int I = ij_to_I(i, j);

			(*dest_R1)[I] = (*src_R2)[i][j];
		}
	}
}

/**
 * Converts a 1D vector to a 2D matrix.
*/
void R1_to_R2(const vector<double> *src_R1, vector<vector<double>> *dest_R2) {
	for (int I = 0; I < N__integration; I++) {
		int i = I_to_i(I);
		int j = I_to_j(I);

		(*dest_R2)[i][j] = (*src_R1)[I];
	}
}

double L_operator(const vector<double> *function_old, int I) {
	static shared_ptr<GridFunction> function = nullptr;
	static const vector<double> *function_ref = nullptr;

	int i = I_to_i(I);
	int j = I_to_j(I);
	double theta = grid__integration.theta(i);

	if (function_ref != function_old) {
		vector<double> base_vector(grid__integration.N_phi, 0);
		vector<vector<double>> function_R2(grid__integration.N_theta, base_vector);
		R1_to_R2(function_old, &function_R2);
		function = make_shared<GridFunction>(grid__integration, &function_R2);
		function_ref = function_old;
	}

	return (
		function->partial_theta()->partial_theta()->points[i][j] +
		(1/tan(theta)) * function->partial_theta()->points[i][j] +
		(1/squared(sin(theta))) * function->partial_phi()->partial_phi()->points[i][j]
	);
}

void subtract_right_side_averages(vector<double> *b) {
	double average = 0;
	for (int I = 0; I < N__integration; I++) {
		double theta = grid__integration.theta(I_to_i(I));
		average += (*b)[I] * sin(theta) / N__integration;
	}

	if (abs(average) < 1e-14) {
		printf("%e\n", average);
		return;
	}

	for (int I = 0; I < N__integration; I++) {
		(*b)[I] -= (M_PI / 2) * average;
	}

	double corrected_average = 0;
	for (int I = 0; I < N__integration; I++) {
		double theta = grid__integration.theta(I_to_i(I));
		corrected_average += (*b)[I] * sin(theta) / N__integration;
	}

	printf("%e --> %e\n", average, corrected_average);
}

void output_average(vector<double> *average, ofstream *output) {
	for (int I = 0; I < N__integration; I++) {
		int i = I_to_i(I);
		int j = I_to_j(I);

		(*output) << (*average)[I];

		if (i == grid__integration.N_theta - 1 && j == grid__integration.N_phi - 1) { // last
			(*output) << endl;
		} else {
			(*output) << ",";
		}
	}
}

void run_integration(
	shared_ptr<Grid3DFunction> e_theta,
	shared_ptr<Grid3DFunction> e_phi,
	shared_ptr<Grid3DFunction> embedding
) {
	grid__integration = e_theta->grid;
	N__integration = grid__integration.N_theta * grid__integration.N_phi;

	vector<vector<double>> x = embedding->x_values;
	vector<double> x_R1(N__integration, 0);
	R2_to_R1(&x, &x_R1);

	vector<vector<double>> y = embedding->y_values;
	vector<double> y_R1(N__integration, 0);
	R2_to_R1(&y, &y_R1);

	vector<vector<double>> z = embedding->z_values;
	vector<double> z_R1(N__integration, 0);
	R2_to_R1(&z, &z_R1);

	// Compute right side of the equations.
	vector<double> b_x_R1(N__integration, 0);
	vector<double> b_y_R1(N__integration, 0);
	vector<double> b_z_R1(N__integration, 0);
	for (int i = 0; i < grid__integration.N_theta; i++) {
		for (int j = 0; j < grid__integration.N_phi; j++) {
			int I = ij_to_I(i, j);
			double theta = grid__integration.theta(i);

			b_x_R1[I] =
				e_theta->partial_theta()->x_values[i][j] +
				(1/tan(theta)) * e_theta->x_values[i][j] +
				(1/squared(sin(theta))) * e_phi->partial_phi()->x_values[i][j];
			b_y_R1[I] =
				e_theta->partial_theta()->y_values[i][j] +
				(1/tan(theta)) * e_theta->y_values[i][j] +
				(1/squared(sin(theta))) * e_phi->partial_phi()->y_values[i][j];
			b_z_R1[I] =
				e_theta->partial_theta()->z_values[i][j] +
				(1/tan(theta)) * e_theta->z_values[i][j] +
				(1/squared(sin(theta))) * e_phi->partial_phi()->z_values[i][j];
		}
	}

	output_average(&b_x_R1, &rightside_x_distribution_output);
	output_average(&b_y_R1, &rightside_y_distribution_output);
	output_average(&b_z_R1, &rightside_z_distribution_output);

	printf("Fixing right side of Poisson equations:\n");
	printf("\tx: ");
	subtract_right_side_averages(&b_x_R1);
	printf("\ty: ");
	subtract_right_side_averages(&b_y_R1);
	printf("\tz: ");
	subtract_right_side_averages(&b_z_R1);

	output_average(&b_x_R1, &rightside_x_distribution_output);
	output_average(&b_y_R1, &rightside_y_distribution_output);
	output_average(&b_z_R1, &rightside_z_distribution_output);

	printf("Solving Poisson equations:\n");
	printf("\tx: ");
	run_bicgstab(L_operator, &x_R1, &b_x_R1);
	printf("\ty: ");
	run_bicgstab(L_operator, &y_R1, &b_y_R1);
	printf("\tz: ");
	run_bicgstab(L_operator, &z_R1, &b_z_R1);

	R1_to_R2(&x_R1, &embedding->x_values);
	R1_to_R2(&y_R1, &embedding->y_values);
	R1_to_R2(&z_R1, &embedding->z_values);
}
