#ifndef GRID_H
#define GRID_H

#include <bits/stdc++.h>
using namespace std;

#include "../utils/rms.h"

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

class Grid3DFunction {
	public:
		Grid grid;

		vector<vector<double>> x_values;
		vector<vector<double>> y_values;
		vector<vector<double>> z_values;

		Grid3DFunction(Grid grid_, double (*function)(int i, int j, char coordinate));
		Grid3DFunction() = default;

		double rms();
};

#endif
