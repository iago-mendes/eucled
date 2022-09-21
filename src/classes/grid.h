#ifndef GRID_H
#define GRID_H

#include <bits/stdc++.h>

class Grid {
	public:
		// Number of grid points
		double N_theta;
		double N_phi;

		// Grid-point divisions
		double delta_theta;
		double delta_phi;

		Grid(double N_theta_, double N_phi_);

		double theta(int i);
		double phi(int j);
};

#endif
