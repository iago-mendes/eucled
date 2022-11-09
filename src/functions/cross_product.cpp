#include "cross_product.h"

shared_ptr<Grid3DFunction> vector_a__cross_product(nullptr);
shared_ptr<Grid3DFunction> vector_b__cross_product(nullptr);

double get_cross_product_helper(int i, int j, char coordinate) {
	switch (coordinate) {
		case 'x':
			return
				(*vector_a__cross_product).y_values[i][j] * (*vector_b__cross_product).z_values[i][j] -
				(*vector_a__cross_product).z_values[i][j] * (*vector_b__cross_product).y_values[i][j];
			break;
		
		case 'y':
			return
				(*vector_a__cross_product).z_values[i][j] * (*vector_b__cross_product).x_values[i][j] -
				(*vector_a__cross_product).x_values[i][j] * (*vector_b__cross_product).z_values[i][j];
			break;
		
		case 'z':
			return
				(*vector_a__cross_product).x_values[i][j] * (*vector_b__cross_product).y_values[i][j] -
				(*vector_a__cross_product).y_values[i][j] * (*vector_b__cross_product).x_values[i][j];
			break;
	
		default:
			return -1;
			break;
	}
}

shared_ptr<Grid3DFunction> get_cross_product(shared_ptr<Grid3DFunction> vector_a, shared_ptr<Grid3DFunction> vector_b) {
	vector_a__cross_product = vector_a;
	vector_b__cross_product = vector_b;

	shared_ptr<Grid3DFunction> cross_product = make_shared<Grid3DFunction>((*vector_a).grid, get_cross_product_helper);

	return cross_product;
}
