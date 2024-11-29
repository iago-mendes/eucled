add_executable(eucled_numeric_embedding numeric_embedding.cpp)
target_link_libraries(eucled_numeric_embedding PRIVATE
	${HDF5_CXX_LIBRARIES}
	Metrics
	embedding
	relaxation
	dyad
	grid
	commutator
	cross_product
	dot_product
	math
	rms
)

add_executable(eucled_test_case test_case.cpp)
target_link_libraries(eucled_test_case
	Metrics
	embedding
	relaxation
	dyad
	grid
	commutator
	cross_product
	dot_product
	math
	rms
)
