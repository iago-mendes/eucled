#include <chrono>
#include <iostream>

#include "../algorithms/embedding.h"

#include "../Mesh/Mesh.hpp"
#include "../Mesh/DataMesh3D.hpp"
#include "../Metrics/Ellipsoid.hpp"
#include "../Metrics/KerrHorizon.hpp"
#include "../Metrics/Metric.hpp"
#include "../Metrics/Sphere.hpp"
#include "../Metrics/XPeanut.hpp"
#include "../Metrics/ZPeanut.hpp"

int main() {
  // Construct mesh
  int N_theta, N_phi;
  std::cin >> N_theta >> N_phi;
  Mesh mesh(N_theta, N_phi);
  std::cout << "Mesh size: " << N_theta << "x" << N_phi << std::endl;

  // Read test case
  std::string test_case;
  std::cin >> test_case;
  std::cout << "Test case: " << test_case << std::endl;

  // Construct metric
  std::shared_ptr<Metric> metric = nullptr;
  if (test_case == "Ellipsoid") {
    double a, b, c;
    std::cin >> a >> b >> c;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = " << c << std::endl;
    metric = std::make_shared<Metrics::Ellipsoid>(a, b, c);
  } else if (test_case == "KerrHorizon") {
    double spin;
    std::cin >> spin;
    std::cout << "spin = " << spin << std::endl;
    metric = std::make_shared<Metrics::KerrHorizon>(spin);
  } else if (test_case == "Sphere") {
    double R;
    std::cin >> R;
    std::cout << "R = " << R << std::endl;
    metric = std::make_shared<Metrics::Sphere>(R);
  } else if (test_case == "XPeanut") {
    double s0;
    std::cin >> s0;
    std::cout << "s0 = " << s0 << std::endl;
    metric = std::make_shared<Metrics::XPeanut>(s0);
  } else if (test_case == "ZPeanut") {
    double r0, b, e;
    std::cin >> r0 >> b >> e;
    std::cout << "r0 = " << r0 << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "e = " << e << std::endl;
    metric = std::make_shared<Metrics::ZPeanut>(r0, b, e);
  }

  // Start counting wall time
  auto start_time = std::chrono::high_resolution_clock::now();

  // Run embedding solver
  std::shared_ptr<DataMesh3D> embedding = std::make_shared<DataMesh3D>(mesh);
  run_embedding(metric, embedding, nullptr, 250.);

  // Output wall time
  auto end_time = std::chrono::high_resolution_clock::now();
  auto wall_time = std::chrono::duration_cast<std::chrono::seconds>(
    end_time - start_time
  ).count();
  std::cout << "Wall time: " << wall_time / 60. << " min" << std::endl;
}
