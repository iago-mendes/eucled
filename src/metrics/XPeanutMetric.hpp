#ifndef METRICS_X_PEANUT_H
#define METRICS_X_PEANUT_H

#include <cmath>

#include "Metric.hpp"
#include "../utils/math.h"

class XPeanutMetric : public Metric {
	public:
		XPeanutMetric();

		XPeanutMetric(double s0);

		void set_parameter(std::string parameter, std::string value);

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);
	
	private:
		// Radius
		double s0;
};

#endif
