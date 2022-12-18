#ifndef FACTORIZATION_H
#define FACTORIZATION_H

#include "../classes/dyad.h"
#include "../classes/grid.h"
#include "../classes/metric.h"

void run_factorization(
	shared_ptr<Metric> metric,
	shared_ptr<Grid3DFunction> e_theta,
	shared_ptr<Grid3DFunction> e_phi
);

#endif
