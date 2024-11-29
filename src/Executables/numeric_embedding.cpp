#include <iostream>

#include "../algorithms/embedding.h"
#include "../classes/grid.h"
#include "../functions/commutator.h"

#include "../Metrics/Metric.hpp"
#include "../Metrics/Numeric.hpp"

int main() {
  // Get information needed to get data from H5 file
  std::string file_path, horizon_key, observation_id;
  cin >> file_path >> horizon_key >> observation_id;

  // Create numeric metric
  shared_ptr<Metrics::Numeric> metric = make_shared<Metrics::Numeric>(file_path, horizon_key, observation_id);

  // Start counting wall time
  auto start_time = std::chrono::high_resolution_clock::now();

  // Run embedding solver
  shared_ptr<Grid3DFunction> embedding = make_shared<Grid3DFunction>(metric->grid);
  run_embedding(metric, embedding);

  // Output wall time
  auto end_time = std::chrono::high_resolution_clock::now();
  auto wall_time = std::chrono::duration_cast<std::chrono::seconds>(
    end_time - start_time
  ).count();
  std::cout << "Wall time: " << wall_time / 60. << " min" << std::endl;
}
