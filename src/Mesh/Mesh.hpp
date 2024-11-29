#pragma once

/**
 * @brief Theta-phi uniform mesh
 */
class Mesh {
	public:
    // Number of grid points
		int N_theta;
		int N_phi;

    // Grid-point divisions
		double delta_theta;
		double delta_phi;

		Mesh(int N_theta_, int N_phi_);
		Mesh() = default;

		double theta(int i);
		double phi(int j);
		int i(double theta);
		int j(double phi);
};
