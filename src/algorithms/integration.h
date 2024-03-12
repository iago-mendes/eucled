#ifndef INTEGRATION_H
#define INTEGRATION_H

#include "../classes/dyad.h"
#include "../classes/grid.h"
#include "../metrics/Metric.hpp"
#include "../utils/math.h"
#include "./bicgstab.h"

/**
 * Runs integration algorithm to find X, Y, and Z using the dyad.
 * Writes result to 'embedding'.
*/
void run_integration(
	shared_ptr<Grid3DFunction> e_theta,
	shared_ptr<Grid3DFunction> e_phi,
	shared_ptr<Grid3DFunction> embedding
);

#endif
