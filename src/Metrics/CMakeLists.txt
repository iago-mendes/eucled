add_library(Metrics STATIC
  Metric.hpp
  KerrHorizon.cpp KerrHorizon.hpp
  Numeric.cpp Numeric.hpp
  XPeanut.cpp XPeanut.hpp
)
target_include_directories(Metrics PRIVATE
	${HDF5_INCLUDE_DIRS}
)
target_link_libraries(Metrics PRIVATE
	${HDF5_CXX_LIBRARIES}
)
