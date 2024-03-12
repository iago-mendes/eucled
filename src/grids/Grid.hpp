#ifndef GRIDS_GRID_H
#define GRIDS_GRID_H

#include <cmath>

// Grid for the angular coordinates theta (polar) and phi (azimuthal)
class Grid {
	public:
		// Converts from the index i to the coordinate angle theta
		virtual double theta(int i) = 0;

		// Converts from the index j to the coordinate angle phi
		virtual double phi(int j) = 0;

		// Converts from the coordinate angle theta to the index i
		virtual int i(double theta) = 0;

		// Converts from the coordinate angle phi to the index j
		virtual int j(double phi) = 0;

		// Returns the number of theta grid points
		int get_N_theta() { return this->N_theta; }

		// Returns the number of phi grid points
		int get_N_phi() { return this->N_phi; }

	protected:
		// Number of theta grid points
		int N_theta;

		// Number of phi grid points
		int N_phi;
};

#endif
