#ifndef METRICS_ROUND_SPHERE_H
#define METRICS_ROUND_SPHERE_H

#include <cmath>

#include "Metric.hpp"
#include "../utils/math.h"

class RoundSphereMetric : public Metric {
	public:
		RoundSphereMetric();

		RoundSphereMetric(double radius);

		void setParameter(std::string parameter, double value);

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);
	
	private:
		// Radius
		double R = 1;
};

#endif
