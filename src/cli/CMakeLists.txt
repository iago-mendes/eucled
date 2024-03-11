add_executable(fidec fidec.cpp)
target_link_libraries(fidec
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
