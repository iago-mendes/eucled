#ifndef METRICS_KERR_HORIZON_H
#define METRICS_KERR_HORIZON_H

#include <cmath>

#include "Metric.hpp"
#include "../utils/math.h"

class KerrHorizonMetric : public Metric {
	public:
		KerrHorizonMetric();

		KerrHorizonMetric(double chi);

		void set_parameter(std::string parameter, std::string value);

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);
	
	private:
		// Rotation parameter (between 0 and 1)
		double chi;

		double rho();
		double sigma(double theta);
};

#endif
