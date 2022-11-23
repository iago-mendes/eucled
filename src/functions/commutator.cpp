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
	// static int count = 0;
	// printf("<%d>", ++count);
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

	commutator = (*commutator).multiplied_by(sin_sqrt_multiplier);

	return (*commutator).rms();
}
