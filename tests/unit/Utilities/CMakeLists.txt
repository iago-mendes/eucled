# Distributed under the MIT License.
# See LICENSE.txt for details.

add_executable(Test_integrate Test_integrate.cpp)
target_link_libraries(Test_integrate
  gtest gtest_main
  Mesh
  Utilities
)
add_test(NAME Test_integrate COMMAND Test_integrate)
