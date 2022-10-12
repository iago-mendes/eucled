#include "commutator.h"

Grid3DFunction *e_theta__commutator;
Grid3DFunction *e_phi__commutator;

double get_commutator_helper(int i, int j, char coordinate) {
	switch (coordinate) {
		case 'x':
			return (*e_phi__commutator).x_values[i][j] - (*e_theta__commutator).x_values[i][j];
			break;
		
		case 'y':
			return (*e_phi__commutator).y_values[i][j] - (*e_theta__commutator).y_values[i][j];
			break;
		
		case 'z':
			return (*e_phi__commutator).z_values[i][j] - (*e_theta__commutator).z_values[i][j];
			break;
	
		default:
			return -1;
			break;
	}
}

double get_commutator_rms(Grid3DFunction *e_theta, Grid3DFunction *e_phi) {
	e_theta__commutator = e_theta;
	e_phi__commutator = e_phi;

	Grid3DFunction commutator((*e_theta).grid, get_commutator_helper);

	return commutator.rms();
}
