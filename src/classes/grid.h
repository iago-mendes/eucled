#ifndef GRID_H
#define GRID_H

#include <bits/stdc++.h>
using namespace std;

class Grid {
	public:
		// Number of grid points
		int N_theta;
		int N_phi;

		// Grid-point divisions
		double delta_theta;
		double delta_phi;

		Grid(int N_theta_, int N_phi_);
		Grid() = default;

		double theta(int i);
		double phi(int j);
};

class GridFunction {
	public:
		Grid grid;
		vector<vector<double>> points;

		GridFunction(Grid grid_, double (*function)(int i, int j));

		double partial_theta(int i, int j);
		double partial_phi(int i, int j);
};

#endif
