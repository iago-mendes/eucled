#ifndef GRIDS_UNIFORM_H
#define GRIDS_UNIFORM_H

#include <cmath>

#include "Grid.hpp"

// Grid with angular coordinates uniformly spaced
class UniformGrid : Grid {
	public:
		// Constructs a uniform grid with N_theta and N_phi 
		UniformGrid(int N_theta, int N_phi);

		// Converts from the index i to the coordinate angle theta
		double theta(int i);

		// Converts from the index j to the coordinate angle phi
		double phi(int j);

		// Converts from the coordinate angle theta to the index i
		int i(double theta);

		// Converts from the coordinate angle phi to the index j
		int j(double phi);

	private:
		// Separation between theta coordinates
		double delta_theta;

		// Separation between phi coordinates
		double delta_phi;
};

#endif
