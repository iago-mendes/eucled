// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include "../Mesh/DataMesh3D.hpp"
#include "../Metrics/Metric.hpp"

/**
 * Finds an isometric embedding for surface described by the given metric tensor.
 * Writes result to 'embedding'.
*/
void run_embedding(
	std::shared_ptr<Metric> metric,
	std::shared_ptr<DataMesh3D> embedding,
	double final_time = 0
);
