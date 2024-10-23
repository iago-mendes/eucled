add_executable(test_case test_case.cpp)
target_link_libraries(test_case
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
