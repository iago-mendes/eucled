#ifndef METRIC_H
#define METRIC_H

#include <bits/stdc++.h>
#include "../utils/math.h"

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

#endif
