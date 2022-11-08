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

		// Returns the partial derivative relative to theta of this function.
		Grid3DFunction *partial_theta();

		// Returns the partial derivative relative to phi of this function.
		Grid3DFunction *partial_phi();

		// Returns a new function multipled by the given multiplier.
		Grid3DFunction *multiplied_by(double (*multiplier)(double theta, double phi, char coordinate));

		// Returns a new function multipled by the given multiplier.
		Grid3DFunction *multiplied_by(double multiplier);

		// Returns a new function added with the given function multiplied by the given multiplier.
		Grid3DFunction *added_with(
			Grid3DFunction *function,
			double (*multiplier)(double theta, double phi, char coordinate)
		);

		// Returns a new function added with the given function multiplied by the given multiplier.
		Grid3DFunction *added_with(
			Grid3DFunction *function,
			double multiplier
		);
};

#endif
