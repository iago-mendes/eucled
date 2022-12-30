#include "dot_product.h"
using namespace std;

double dot_product(const vector<double> *p_a, const vector<double> *p_b) {
	double sum = 0;
	
	int N = min(p_a->size(), p_b->size());
	for (int I = 0; I < N; I++)
		sum += (*p_a)[I] * (*p_b)[I];
	
	return sum;
}
