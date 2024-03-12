#ifndef METRICS_Z_PEANUT_H
#define METRICS_Z_PEANUT_H

#include <cmath>

#include "Metric.hpp"
#include "../utils/math.h"

class ZPeanutMetric : public Metric {
	public:
		ZPeanutMetric();

		ZPeanutMetric(double chi);

		void set_parameter(std::string parameter, std::string value);

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);
	
	private:
		// Separation of sphere centers.
		double a;

		double rho();
		double sigma(double theta);
};

#endif
