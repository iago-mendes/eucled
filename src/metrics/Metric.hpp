#ifndef METRICS_METRIC_H
#define METRICS_METRIC_H

#include <string>
#include <vector>

// Metric tensor template
class Metric {
	public:
		// Used when constructing a metric via the CLI
		std::vector<std::string> parameters;
		virtual void set_parameter(std::string parameter, std::string value) = 0;

		virtual double g_theta_theta(double theta, double phi) = 0;
		virtual double g_theta_phi(double theta, double phi) = 0;
		virtual double g_phi_phi(double theta, double phi) = 0;
};

#endif
