# Distributed under the MIT License.
# See LICENSE.txt for details.

add_library(Metrics STATIC
  Metric.hpp
  Ellipsoid.cpp Ellipsoid.hpp
  KerrHorizon.cpp KerrHorizon.hpp
  Numeric.cpp Numeric.hpp
  Sphere.cpp Sphere.hpp
  XPeanut.cpp XPeanut.hpp
  ZPeanut.cpp ZPeanut.hpp
)
target_include_directories(Metrics PRIVATE
	${HDF5_INCLUDE_DIRS}
)
target_link_libraries(Metrics PRIVATE
	${HDF5_CXX_LIBRARIES}
)
