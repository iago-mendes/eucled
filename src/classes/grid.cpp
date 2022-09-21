#include "grid.h"
using namespace std;

class Grid {
	public:
		// Number of grid points
		double N_theta;
		double N_phi;

		// Grid-point divisions
		double delta_theta;
		double delta_phi;

		Grid(double N_theta_, double N_phi_) {
			N_theta = N_theta_;
			N_phi = N_phi_;

			delta_theta = M_PI / N_theta;
			delta_phi = 2 * M_PI / N_phi;
		}

		double theta(int i) {
			return M_PI / N_theta * (i + 1 / 2);
		}

		double phi(int j) {
			return 2 * M_PI / N_theta * j;
		}
};