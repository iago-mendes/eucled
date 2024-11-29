add_executable(eucled_numeric_embedding numeric_embedding.cpp)
target_link_libraries(eucled_numeric_embedding PRIVATE
	${HDF5_CXX_LIBRARIES}
	Mesh
	Metrics
	embedding
	relaxation
	commutator
	cross_product
	math
	rms
)

add_executable(eucled_test_case test_case.cpp)
target_link_libraries(eucled_test_case
	Mesh
	Metrics
	embedding
	relaxation
	commutator
	cross_product
	math
	rms
)
