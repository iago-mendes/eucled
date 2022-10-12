#include "relaxation.h"
using namespace std;

Grid3DFunction *e_theta__relaxation;
Grid3DFunction *e_phi__relaxation;

void update_e_theta() {}

void update_e_phi() {}

double run_relaxation(
	Grid3DFunction *e_theta,
	Grid3DFunction *e_phi,
	double (*get_residual)(Grid3DFunction *e_theta, Grid3DFunction *e_phi)
) {
	e_theta__relaxation = e_theta;
	e_phi__relaxation = e_phi;

	double residual;
	int iteration_number = 0;
	while (
		(residual = get_residual(e_theta, e_phi)) > RESIDUAL_TOLERANCE &&
		++iteration_number < MAX_ITERATIONS
	) {
		update_e_theta();
		update_e_phi();
	}

	printf("Relaxation finished with R = %8.2e after %5d iterations.\n", residual, iteration_number);
	return residual;
}
