add_executable(09_convergence main.cpp)
target_link_libraries(09_convergence
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
