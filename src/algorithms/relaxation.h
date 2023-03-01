#ifndef RELAXATION_H
#define RELAXATION_H

#include "../classes/grid.h"
#include "../classes/time_stepper.h"
#include "../functions/commutator.h"
#include "../functions/cross_product.h"
#include "./integration.h"

#define RESIDUAL_TOLERANCE 1e-15
#define MAX_ITERATIONS 100000
#define INITIAL_ITERATIONS 1e2
#define OUTPUT_FREQUENCY 100
#define INITIAL_TIME_STEP 0.01

double run_relaxation(
	shared_ptr<Grid3DFunction> e_theta,
	shared_ptr<Grid3DFunction> e_phi,
	double (*get_residual)(shared_ptr<Grid3DFunction> e_theta, shared_ptr<Grid3DFunction> e_phi),
	char *identifier = nullptr
);

#endif
