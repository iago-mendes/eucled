#pragma once

#include "../Mesh/DataMesh3D.hpp"
#include "../Metrics/Metric.hpp"

#define RESIDUAL_TOLERANCE 1e-14
#define MAX_ITERATIONS 100000
#define INITIAL_ITERATIONS 1e2
#define OUTPUT_FREQUENCY 100
#define INITIAL_TIME_STEP 0.01

double run_relaxation(
	std::shared_ptr<DataMesh3D> e_theta,
	std::shared_ptr<DataMesh3D> e_phi,
	std::shared_ptr<DataMesh3D> embedding,
	std::shared_ptr<Metric> metric,
	double (*get_residual)(std::shared_ptr<DataMesh3D> e_theta, std::shared_ptr<DataMesh3D> e_phi),
	char *identifier,
	double final_time
);

struct Iteration {
	std::shared_ptr<DataMesh3D> solution1;
	std::shared_ptr<DataMesh3D> solution2;
	double residual;
};
