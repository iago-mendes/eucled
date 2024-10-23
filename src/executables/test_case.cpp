#include <iostream>

#include "../algorithms/embedding.h"
#include "../classes/dyad.h"
#include "../classes/grid.h"
#include "../classes/metric.h"
#include "../functions/commutator.h"

int main() {
  // Construct mesh
  int N_theta, N_phi;
  std::cin >> N_theta >> N_phi;
  Grid grid(N_theta, N_phi);
  std::cout << "Mesh size: " << N_theta << "x" << N_phi << std::endl;

  // Read test case
  std::string test_case;
  std::cin >> test_case;
  std::cout << "Test case: " << test_case << std::endl;

  // Construct metric
  shared_ptr<Metric> metric = nullptr;
  if (test_case == "XPeanut") {
    double s0;
    std::cin >> s0;
    std::cout << "s0 = " << s0 << std::endl;

    metric = make_shared<XPeanutMetric>(s0);
  }

  // Start counting wall time
  auto start_time = std::chrono::high_resolution_clock::now();

  // Run embedding solver
  shared_ptr<Grid3DFunction> embedding = make_shared<Grid3DFunction>(grid);
  run_embedding(metric, embedding, nullptr, 250.);

  // Output wall time
  auto end_time = std::chrono::high_resolution_clock::now();
  auto wall_time = std::chrono::duration_cast<std::chrono::seconds>(
    end_time - start_time
  ).count();
  std::cout << "Wall time: " << wall_time / 60. << " min" << std::endl;
}
