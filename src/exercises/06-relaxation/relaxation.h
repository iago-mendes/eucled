#ifndef RELAXATION_H
#define RELAXATION_H

#include "../../classes/grid.h"

#define RESIDUAL_TOLERANCE 1e-13
#define MAX_ITERATIONS 1e2

double run_relaxation(
	Grid3DFunction *e_theta_,
	Grid3DFunction *e_phi_,
	double (*get_residual)(Grid3DFunction *e_theta, Grid3DFunction *e_phi)
);

#endif
