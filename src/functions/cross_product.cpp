#include "cross_product.h"

Grid3DFunction *vector_a__cross_product;
Grid3DFunction *vector_b__cross_product;

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

Grid3DFunction *get_cross_product(Grid3DFunction *vector_a, Grid3DFunction *vector_b) {
	vector_a__cross_product = vector_a;
	vector_b__cross_product = vector_b;

	Grid3DFunction *cross_product;
	cross_product = new Grid3DFunction;
	Grid3DFunction cross_product_((*vector_a).grid, get_cross_product_helper);
	(*cross_product) = cross_product_;

	return cross_product;
}
