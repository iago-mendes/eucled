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

double get_rms(const vector<vector<double>> *p_vect) {
	double squared_sum = 0;
	int total_count = 0;

	for (int i = 0; i < (int) (*p_vect).size(); i++) {
		for (int j = 0; j < (int) (*p_vect)[i].size(); j++) {
			squared_sum += squared((*p_vect)[i][j]);
			total_count++;
		}
	}
	
	double result = sqrt(squared_sum / total_count);
	
	return result;
}
