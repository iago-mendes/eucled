#ifndef METRICS_DENTED_SPHERE_H
#define METRICS_DENTED_SPHERE_H

#include <cmath>

#include "Metric.hpp"
#include "../utils/math.h"

class DentedSphereMetric : public Metric {
	public:
		DentedSphereMetric() = default;

		void set_parameter(std::string parameter, std::string value) {};

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);
};

#endif
