// Distributed under the MIT License.
// See LICENSE.txt for details.

#include <gtest/gtest.h>

#include <math.h>

#include "../../../src/Mesh/DataMesh.hpp"
#include "../../../src/Utilities/integrate.hpp"
#include "../../../src/Utilities/math.hpp"

namespace {

void test_data_mesh_on_unit_sphere(const int N) {
  const Mesh mesh(N, 2*N);
  const auto data = std::make_shared<DataMesh>(mesh, sin_theta);
  const double result = integrate(data);
  EXPECT_NEAR(result, squared(M_PI), 1e-12);
}

} // namespace

TEST(IntegrateTest, DataMeshOnUnitSphere) {
  for (int N = 10; N <= 100; N += 10) {
    test_data_mesh_on_unit_sphere(N);
  }
}
