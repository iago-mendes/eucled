// Distributed under the MIT License.
// See LICENSE.txt for details.

#include <chrono>
#include <iostream>

#include "../Algorithms/embedding.hpp"
#include "../Mesh/DataMesh3D.hpp"
#include "../Metrics/Metric.hpp"
#include "../Metrics/Numeric.hpp"

int main() {
  // Set line-buffering mode for std::cout
  std::setvbuf(stdout, nullptr, _IOLBF, 0);

  // Get information needed to get data from H5 file
  std::string file_path, horizon_key, observation_id;
  std::cin >> file_path >> horizon_key >> observation_id;

  // Create numeric metric
  std::shared_ptr<Metrics::Numeric> metric = std::make_shared<Metrics::Numeric>(file_path, horizon_key, observation_id);

  // Start counting wall time
  auto start_time = std::chrono::high_resolution_clock::now();

  // Run embedding solver
  const auto embedding = find_embedding(metric, metric->mesh);

  // Output wall time
  auto end_time = std::chrono::high_resolution_clock::now();
  auto wall_time = std::chrono::duration_cast<std::chrono::seconds>(
    end_time - start_time
  ).count();
  std::cout << std::defaultfloat << "Wall time: " << wall_time / 60. << " min" << std::endl;
}
