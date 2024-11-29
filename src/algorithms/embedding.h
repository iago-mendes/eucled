#ifndef EMBEDDING_H
#define EMBEDDING_H

#include "../classes/grid.h"
#include "../Metrics/Metric.hpp"
#include "./relaxation.h"

/**
 * Finds an isometric embedding for surface described by the given metric tensor.
 * Writes result to 'embedding'.
*/
void run_embedding(
	shared_ptr<Metric> metric,
	shared_ptr<Grid3DFunction> embedding,
	char *identifier = nullptr,
	double final_time = 0
);

#endif
