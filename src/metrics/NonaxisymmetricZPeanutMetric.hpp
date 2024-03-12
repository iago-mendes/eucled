#ifndef METRICS_NONAXISYMMETRIC_Z_PEANUT_H
#define METRICS_NONAXISYMMETRIC_Z_PEANUT_H

#include <cmath>

#include "Metric.hpp"
#include "../utils/math.h"

class NonaxisymmetricZPeanutMetric : public Metric {
	public:
		NonaxisymmetricZPeanutMetric();

		NonaxisymmetricZPeanutMetric(double r0, double b, double e);

		void set_parameter(std::string parameter, std::string value);

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);
	
	private:
		// Radius
		double r0;

		// Separation
		double b;

		// Non-axisymmetry
		double e;

		double r(double theta, double phi);
		double partial_theta_r(double theta, double phi);
		double partial_phi_r(double theta, double phi);
};

#endif
