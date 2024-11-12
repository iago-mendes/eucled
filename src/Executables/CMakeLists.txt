add_executable(eucled_test_case test_case.cpp)
target_link_libraries(eucled_test_case
	Metrics
	embedding
	relaxation
	dyad
	grid
	metric
	commutator
	cross_product
	dot_product
	math
	rms
)
