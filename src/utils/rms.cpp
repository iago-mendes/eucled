#include "rms.h"
using namespace std;

double get_rms(const vector<double> *p_vect) {
	double squared_sum = 0;
	int N = (*p_vect).size();

	for (int I = 0; I < N; I++) {
		squared_sum += squared((*p_vect)[I]);
	}
	
	double result = sqrt(squared_sum / N);
	
	return result;
}
