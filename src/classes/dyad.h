#ifndef DYAD_H
#define DYAD_H

#include <bits/stdc++.h>
#include "grid.h"

class Dyad {
	public:
		virtual double e_theta_x(double theta, double phi) = 0;
		virtual double e_theta_y(double theta, double phi) = 0;
		virtual double e_theta_z(double theta, double phi) = 0;
		virtual double e_phi_x(double theta, double phi) = 0;
		virtual double e_phi_y(double theta, double phi) = 0;
		virtual double e_phi_z(double theta, double phi) = 0;

		shared_ptr<Grid3DFunction> get_theta_function(Grid grid);
		shared_ptr<Grid3DFunction> get_phi_function(Grid grid);
};

class RoundSphereDyad : public Dyad {
	public:
		double R; // Radius

		RoundSphereDyad(double radius);
		RoundSphereDyad() = default;

		double e_theta_x(double theta, double phi);
		double e_theta_y(double theta, double phi);
		double e_theta_z(double theta, double phi);
		double e_phi_x(double theta, double phi);
		double e_phi_y(double theta, double phi);
		double e_phi_z(double theta, double phi);
};

class OblateSpheroidDyad : public Dyad {
	public:
		// Semi axes
		double a;
		double b;

		OblateSpheroidDyad(double a, double b);
		OblateSpheroidDyad() = default;

		double e_theta_x(double theta, double phi);
		double e_theta_y(double theta, double phi);
		double e_theta_z(double theta, double phi);
		double e_phi_x(double theta, double phi);
		double e_phi_y(double theta, double phi);
		double e_phi_z(double theta, double phi);
};

class RotatedSphereDyad : public Dyad {
	public:
		double (*rotator)(double theta, double phi); // Field rotator

		RoundSphereDyad round_dyad;

		RotatedSphereDyad(double (*rotator)(double theta, double phi));
		RotatedSphereDyad() = default;

		double e_theta_x(double theta, double phi);
		double e_theta_y(double theta, double phi);
		double e_theta_z(double theta, double phi);
		double e_phi_x(double theta, double phi);
		double e_phi_y(double theta, double phi);
		double e_phi_z(double theta, double phi);
};

#endif
