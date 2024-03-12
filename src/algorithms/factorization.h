#ifndef FACTORIZATION_H
#define FACTORIZATION_H

#include "../classes/dyad.h"
#include "../classes/grid.h"
#include "../metrics/Metric.hpp"

void run_factorization(
	shared_ptr<Metric> metric,
	shared_ptr<Grid3DFunction> e_theta_1,
	shared_ptr<Grid3DFunction> e_phi,
	shared_ptr<Grid3DFunction> e_theta_2 = nullptr
);

#endif
