#ifndef RELAXATION_H
#define RELAXATION_H

#include "../classes/grid.h"
#include "../functions/commutator.h"
#include "../functions/cross_product.h"
#include "./integration.h"
#include "../Metrics/Metric.hpp"

#define RESIDUAL_TOLERANCE 1e-14
#define MAX_ITERATIONS 100000
#define INITIAL_ITERATIONS 1e2
#define OUTPUT_FREQUENCY 100
#define INITIAL_TIME_STEP 0.01

double run_relaxation(
	shared_ptr<Grid3DFunction> e_theta,
	shared_ptr<Grid3DFunction> e_phi,
	shared_ptr<Grid3DFunction> embedding,
	shared_ptr<Metric> metric,
	double (*get_residual)(shared_ptr<Grid3DFunction> e_theta, shared_ptr<Grid3DFunction> e_phi),
	char *identifier,
	double final_time
);

struct Iteration {
	shared_ptr<Grid3DFunction> solution1;
	shared_ptr<Grid3DFunction> solution2;
	double residual;
};

#endif
