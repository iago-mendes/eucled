#include "commutator.h"

shared_ptr<Grid3DFunction> e_theta__commutator(nullptr);
shared_ptr<Grid3DFunction> e_phi__commutator(nullptr);

double sin_sqrt_multiplier(
		double theta,
		[[maybe_unused]] double phi,
	[[maybe_unused]] char coordinate
) {
	return sqrt(sin(theta));
}

double get_commutator_helper(int i, int j, char coordinate) {
	switch (coordinate) {
	case 'x':
		return (*(*e_phi__commutator).partial_theta()).x_values[i][j] - (*(*e_theta__commutator).partial_phi()).x_values[i][j];
		break;

	case 'y':
		return (*(*e_phi__commutator).partial_theta()).y_values[i][j] - (*(*e_theta__commutator).partial_phi()).y_values[i][j];
		break;

	case 'z':
		return (*(*e_phi__commutator).partial_theta()).z_values[i][j] - (*(*e_theta__commutator).partial_phi()).z_values[i][j];
		break;

	default:
		return -1;
		break;
	}
}

shared_ptr<Grid3DFunction> get_commutator(shared_ptr<Grid3DFunction> e_theta, shared_ptr<Grid3DFunction> e_phi) {
	e_theta__commutator = e_theta;
	e_phi__commutator = e_phi;

	shared_ptr<Grid3DFunction> commutator = make_shared<Grid3DFunction>((*e_theta).grid, get_commutator_helper);

	return commutator;
}

double get_commutator_rms(shared_ptr<Grid3DFunction> e_theta, shared_ptr<Grid3DFunction> e_phi) {
	shared_ptr<Grid3DFunction> commutator = get_commutator(e_theta, e_phi);

	commutator = commutator->multiplied_by(sin_sqrt_multiplier);
	shared_ptr<GridFunction> norm = commutator->norm();

	double squared_sum = 0;
	int total_count = 0;
	double polar_range = M_PI / 4; // 45 degrees

	for (int i = 0; i < e_theta->grid.N_theta; i++)
	{
		double theta = e_theta->grid.theta(i);
		if (theta < 0 + polar_range || theta > M_PI - polar_range) {
			continue;
		}

		for (int j = 0; j < e_theta->grid.N_phi; j++) {
			squared_sum += squared(norm->points[i][j]);
			total_count++;
		}
	}

	double result = sqrt(squared_sum / total_count);

	return result;

	return norm->rms();
}

shared_ptr<GridFunction> get_commutator_norm(shared_ptr<Grid3DFunction> e_theta, shared_ptr<Grid3DFunction> e_phi) {
	shared_ptr<Grid3DFunction> commutator = get_commutator(e_theta, e_phi);

	return commutator->norm();
}
