#ifndef GRIDS_UNIFORM_H
#define GRIDS_UNIFORM_H

#include <cmath>

// Grid with angular coordinates uniformly spaced
class UniformGrid {
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

		// Returns the number of theta grid points
		int get_N_theta() { return this->N_theta; }

		// Returns the number of phi grid points
		int get_N_phi() { return this->N_phi; }

		// Returns the separation between theta coordinates
		int get_delta_theta() { return this->delta_theta; }

		// Returns the separation between phi coordinates
		int get_delta_phi() { return this->delta_phi; }
	
	private:
		// Number of theta grid points
		int N_theta;

		// Number of phi grid points
		int N_phi;

		// Separation between theta coordinates
		double delta_theta;

		// Separation between phi coordinates
		double delta_phi;
};

#endif
