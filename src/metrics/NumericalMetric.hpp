#ifndef METRICS_NUMERICAL_H
#define METRICS_NUMERICAL_H

#include <cmath>

#include "Metric.hpp"
#include "../utils/math.h"
#include "../classes/grid.h"

class NumericalMetric : public Metric {
	public:
		NumericalMetric();

		void set_parameter(std::string parameter, std::string value);

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);

	private:
		std::vector<std::vector<double>> data[3];
		std::shared_ptr<Grid> pGrid;

		void read_data(std::string file_name);
};

#endif
