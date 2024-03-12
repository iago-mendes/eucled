#ifndef METRICS_OBLATE_SPHEROID_H
#define METRICS_OBLATE_SPHEROID_H

#include <cmath>

#include "Metric.hpp"
#include "../utils/math.h"

class OblateSpheroidMetric : public Metric {
	public:
		OblateSpheroidMetric();

		OblateSpheroidMetric(double a, double b);

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);

		void setParameter(std::string parameter, double value);
	
	private:
		// 1st Radius
		double a;

		// 2nd Radius
		double b;
};

#endif
