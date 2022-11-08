#include "commutator.h"

Grid3DFunction *e_theta__commutator;
Grid3DFunction *e_phi__commutator;

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

Grid3DFunction *get_commutator(Grid3DFunction *e_theta, Grid3DFunction *e_phi) {
	e_theta__commutator = e_theta;
	e_phi__commutator = e_phi;

	Grid3DFunction *commutator;
	commutator = new Grid3DFunction;
	(*commutator) = Grid3DFunction((*e_theta).grid, get_commutator_helper);

	return commutator;
}

double get_commutator_rms(Grid3DFunction *e_theta, Grid3DFunction *e_phi) {
	Grid3DFunction *commutator = get_commutator(e_theta, e_phi);

	return (*commutator).rms();
}
