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
		int i(double theta);
		int j(double phi);
};

class GridFunction {
	public:
		Grid grid;
		vector<vector<double>> points;

		GridFunction(Grid grid, double (*function)(int i, int j));
		GridFunction(Grid grid, vector<vector<double>> *points);
		GridFunction(Grid grid);

		shared_ptr<GridFunction> partial_theta();
		shared_ptr<GridFunction> partial_phi();

		// Returns a copy of this function.
		shared_ptr<GridFunction> get_copy();

		// Returns the RMS of this function.
		double rms();

		// Returns a new function added with the given function multiplied by the given multiplier.
		shared_ptr<GridFunction> added_with(shared_ptr<GridFunction> function, double multiplier);
		shared_ptr<GridFunction> added_with(double (*function)(double theta, double phi));

		// Returns a new function multipled by the given multiplier.
		shared_ptr<GridFunction> multiplied_by(double (*multiplier)(double theta, double phi));

    // Prints a string representation of this function.
		void print();

		// Prints a string representation of this function with an identifier string on top.
		void print(string identifier);

		private:
			shared_ptr<GridFunction> cached_partial_theta;
			shared_ptr<GridFunction> cached_partial_phi;
};

class Grid3DFunction {
	public:
		Grid grid;

		vector<vector<double>> x_values;
		vector<vector<double>> y_values;
		vector<vector<double>> z_values;

		Grid3DFunction(Grid grid_, double (*function)(int i, int j, char coordinate));
		Grid3DFunction(Grid grid, double (*function)(double theta, double phi, char coordinate));
		Grid3DFunction(Grid grid);
		Grid3DFunction() = default;

		double rms();

		// Returns a copy of this function.
		shared_ptr<Grid3DFunction> get_copy();

		// Returns the norm of this function.
		shared_ptr<GridFunction> norm();

		// Returns the partial derivative relative to theta of this function.
		shared_ptr<Grid3DFunction> partial_theta();

		// Returns the partial derivative relative to phi of this function.
		shared_ptr<Grid3DFunction> partial_phi();

		// Returns the second partial derivative relative to theta of this function.
		shared_ptr<Grid3DFunction> second_partial_theta();

		// Returns the second partial derivative relative to phi of this function.
		shared_ptr<Grid3DFunction> second_partial_phi();

		// Returns a new function multipled by the given multiplier.
		shared_ptr<Grid3DFunction> multiplied_by(double (*multiplier)(double theta, double phi, char coordinate));
		shared_ptr<Grid3DFunction> multiplied_by(double (*multiplier)(double theta, double phi));
		shared_ptr<Grid3DFunction> multiplied_by(shared_ptr<GridFunction> multiplier);
		shared_ptr<Grid3DFunction> multiplied_by(double multiplier);

		// Returns a new function added with the given function multiplied by the given multiplier.
		shared_ptr<Grid3DFunction> added_with(shared_ptr<Grid3DFunction> function, double (*multiplier)(double theta, double phi, char coordinate));
		shared_ptr<Grid3DFunction> added_with(shared_ptr<Grid3DFunction> function, double (*multiplier)(double theta, double phi));
		shared_ptr<Grid3DFunction> added_with(shared_ptr<Grid3DFunction> function, double multiplier);

    // Returns a new 1D function with the dot product of the x, y, and z values.
		shared_ptr<GridFunction> dot_product_with(shared_ptr<Grid3DFunction> function);

		// Prints a string representation of this function.
		void print();

		// Prints a string representation of this function with an identifier string on top.
		void print(string identifier);

		private:
			shared_ptr<Grid3DFunction> cached_partial_theta = nullptr;
			shared_ptr<Grid3DFunction> cached_partial_phi = nullptr;
			shared_ptr<Grid3DFunction> cached_second_partial_theta = nullptr;
			shared_ptr<Grid3DFunction> cached_second_partial_phi = nullptr;
			shared_ptr<GridFunction> cached_norm = nullptr;
};

#endif
