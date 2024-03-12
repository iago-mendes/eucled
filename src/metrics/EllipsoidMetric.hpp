#ifndef METRICS_ELLIPSOID_H
#define METRICS_ELLIPSOID_H

#include <cmath>

#include "Metric.hpp"
#include "../utils/math.h"

class EllipsoidMetric : public Metric {
	public:
		EllipsoidMetric();

		EllipsoidMetric(double a, double b, double c);

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);

		void set_parameter(std::string parameter, std::string value);
	
	private:
		// X Semi-axis
		double a;

		// Y Semi-axis
		double b;

		// Z Semi-axis
		double c;
};

#endif
