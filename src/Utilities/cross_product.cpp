#include "./cross_product.hpp"

std::shared_ptr<DataMesh3D> vector_a__cross_product(nullptr);
std::shared_ptr<DataMesh3D> vector_b__cross_product(nullptr);

double get_cross_product_helper(int i, int j, char coordinate) {
	switch (coordinate) {
		case 'x':
			return
				(*vector_a__cross_product).y_points[i][j] * (*vector_b__cross_product).z_points[i][j] -
				(*vector_a__cross_product).z_points[i][j] * (*vector_b__cross_product).y_points[i][j];
			break;
		
		case 'y':
			return
				(*vector_a__cross_product).z_points[i][j] * (*vector_b__cross_product).x_points[i][j] -
				(*vector_a__cross_product).x_points[i][j] * (*vector_b__cross_product).z_points[i][j];
			break;
		
		case 'z':
			return
				(*vector_a__cross_product).x_points[i][j] * (*vector_b__cross_product).y_points[i][j] -
				(*vector_a__cross_product).y_points[i][j] * (*vector_b__cross_product).x_points[i][j];
			break;
	
		default:
			return -1;
			break;
	}
}

std::shared_ptr<DataMesh3D> get_cross_product(std::shared_ptr<DataMesh3D> vector_a, std::shared_ptr<DataMesh3D> vector_b) {
	vector_a__cross_product = vector_a;
	vector_b__cross_product = vector_b;

	std::shared_ptr<DataMesh3D> cross_product = std::make_shared<DataMesh3D>((*vector_a).mesh, get_cross_product_helper);

	return cross_product;
}
