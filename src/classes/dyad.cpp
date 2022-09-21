#include "dyad.h"
using namespace std;

class Dyad {
	public:
		virtual double e_theta_x(double theta, double phi) = 0;
		virtual double e_theta_y(double theta, double phi) = 0;
		virtual double e_theta_z(double theta, double phi) = 0;
		virtual double e_phi_x(double theta, double phi) = 0;
		virtual double e_phi_y(double theta, double phi) = 0;
		virtual double e_phi_z(double theta, double phi) = 0;
};

class RoundSphereDyad : public Dyad {
	public:
		double R; // Radius

		RoundSphereDyad(double radius) {
			R = radius;
		}

		double e_theta_x(double theta, double phi) {
			return R * cos(theta) * cos(phi);
		}

		double e_theta_y(double theta, double phi) {
			return R * cos(theta) * sin(phi);
		}

		double e_theta_z(double theta, double phi) {
			return - R * sin(theta);
		}

		double e_phi_x(double theta, double phi) {
			return - R * sin(theta) * sin(phi);
		}

		double e_phi_y(double theta, double phi) {
			return R * sin(theta) * cos(phi);
		}

		double e_phi_z(double theta, double phi) {
			return 0;
		}
};
