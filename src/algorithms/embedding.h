#ifndef EMBEDDING_H
#define EMBEDDING_H

#include "../classes/dyad.h"
#include "../classes/grid.h"
#include "../classes/metric.h"
#include "./factorization.h"
#include "./relaxation.h"
#include "./integration.h"

/**
 * Finds an isometric embedding for surface described by the given metric tensor.
 * Writes result to 'embedding'.
*/
void run_embedding(
	shared_ptr<Metric> metric,
	shared_ptr<Grid3DFunction> embedding,
	char *identifier = nullptr
);

#endif
