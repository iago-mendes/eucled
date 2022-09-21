#ifndef DYAD_H
#define DYAD_H

#include <bits/stdc++.h>

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

		RoundSphereDyad(double radius);

		double e_theta_x(double theta, double phi);
		double e_theta_y(double theta, double phi);
		double e_theta_z(double theta, double phi);
		double e_phi_x(double theta, double phi);
		double e_phi_y(double theta, double phi);
		double e_phi_z(double theta, double phi);
};

#endif
