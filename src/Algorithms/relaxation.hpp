// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include "../Mesh/DataMesh3D.hpp"
#include "../Metrics/Metric.hpp"

void run_relaxation(
	std::shared_ptr<DataMesh3D> e_theta,
	std::shared_ptr<DataMesh3D> e_phi,
	std::shared_ptr<DataMesh3D> embedding,
	std::shared_ptr<Metric> metric,
	double final_time
);
