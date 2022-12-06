#ifndef RELAXATION_H
#define RELAXATION_H

#include "../../classes/grid.h"
#include "../../classes/time_stepper.h"
#include "../../functions/commutator.h"
#include "../../functions/cross_product.h"

#define RESIDUAL_TOLERANCE 1e-13
#define MAX_ITERATIONS 1e4

double run_relaxation(
	shared_ptr<Grid3DFunction> e_theta_,
	shared_ptr<Grid3DFunction> e_phi_,
	double (*get_residual)(shared_ptr<Grid3DFunction> e_theta, shared_ptr<Grid3DFunction> e_phi)
);

#endif
