add_executable(eucled_numeric_embedding numeric_embedding.cpp)
target_link_libraries(eucled_numeric_embedding PRIVATE
	${HDF5_CXX_LIBRARIES}
	embedding
	relaxation
	Mesh
	Metrics
	Utilities
)

add_executable(eucled_test_case test_case.cpp)
target_link_libraries(eucled_test_case
	embedding
	relaxation
	Mesh
	Metrics
	Utilities
)
