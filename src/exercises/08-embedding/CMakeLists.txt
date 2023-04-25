add_executable(08_embedding main.cpp)
target_link_libraries(08_embedding
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
