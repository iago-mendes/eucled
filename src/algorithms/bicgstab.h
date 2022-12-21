#ifndef BICGSTAB_H
#define BICGSTAB_H

#include <bits/stdc++.h>
using namespace std;

#include "../utils/dot_product.h"
#include "../utils/rms.h"

#define BICGSTAB_MIN_ACCURACY 1e-15
#define BICGSTAB_MIN_SOLUTION_DIFFERENCE 1e-16
#define BICGSTAB_MAX_ITERATIONS 1e4

void run_bicgstab(
	double (*p_A_operator)(const vector<double> *p_vect, int I),
	vector<double> *p_solution,
	const vector<double> *p_b,
	int N
);

#endif