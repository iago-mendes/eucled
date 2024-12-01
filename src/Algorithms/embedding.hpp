// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include "../Mesh/DataMesh3D.hpp"
#include "../Metrics/Metric.hpp"

/**
 * Find an isometric embedding for surface described by the given metric tensor.
 */
std::shared_ptr<DataMesh3D> find_embedding(
	const std::shared_ptr<Metric> metric,
	const Mesh mesh,
	const double final_time = 0
);
