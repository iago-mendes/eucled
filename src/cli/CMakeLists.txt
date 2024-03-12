add_executable(fidec fidec.cpp fidec.hpp)
target_link_libraries(fidec
	Metric
	OblateSpheroidMetric
	RoundSphereMetric
	embedding
)
