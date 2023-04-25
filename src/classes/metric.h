#ifndef METRIC_H
#define METRIC_H

#include <bits/stdc++.h>
#include "../utils/math.h"
#include "./grid.h"

// Metric Tensor
class Metric {
	public:
		virtual double g_theta_theta(double theta, double phi) = 0;
		virtual double g_theta_phi(double theta, double phi) = 0;
		virtual double g_phi_phi(double theta, double phi) = 0;
};

class RoundSphereMetric : public Metric {
	public:
		double R; // Radius

		RoundSphereMetric(double radius);
		RoundSphereMetric() = default;

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);
};

class OblateSpheroidMetric : public Metric {
	public:
		// Semi-axes
		double a;
		double b;

		OblateSpheroidMetric(double a, double b);
		OblateSpheroidMetric() = default;

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);
};

// Metric of the Horizon of a Spinning Black Hole.
class HorizonMetric : public Metric {
	private:
		double rho();
		double sigma(double theta);
	
	public:
		double chi; // Rotation parameter (0 - 1).

		HorizonMetric(double chi);
		HorizonMetric() = default;

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);
};

class EllipsoidMetric : public Metric {
	public:
	  // Semi-axes
		double a;
		double b;
		double c;

		EllipsoidMetric(double a, double b, double c);
		EllipsoidMetric() = default;

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);
};

class PeanutMetric : public Metric {
	public:
		double a; // Distance of sphere centers.

		PeanutMetric(double a);
		PeanutMetric() = default;

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);
};

class DentedSphereMetric : public Metric {
	public:
		DentedSphereMetric() = default;

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);
};

class NumericalMetric : public Metric {
	public:
		vector<vector<double>> data[3];
		Grid grid;

		NumericalMetric();

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);
};

class NonaxisymmetricZPeanutMetric : public Metric {
	public:
		double r0; // radius
		double b; // separation
		double e; // non-axisymmetry

		NonaxisymmetricZPeanutMetric(double r0, double b, double e);
		NonaxisymmetricZPeanutMetric() = default;

		double r(double theta, double phi);
		double partial_theta_r(double theta, double phi);
		double partial_phi_r(double theta, double phi);

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);
};

class XPeanutMetric : public Metric {
	public:
		double s0; // radius

		XPeanutMetric(double s0);
		XPeanutMetric() = default;

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);
};

#endif
