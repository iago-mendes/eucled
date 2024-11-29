#pragma once

#include "./Metric.hpp"

namespace Metrics {

/**
 * @brief 2-Metric of an ellipsoid
 */
class Ellipsoid : public Metric {
  public:
		Ellipsoid(double a, double b, double c);
		Ellipsoid() = default;

    double g_theta_theta(double theta, double phi);
    double g_theta_phi(double theta, double phi);
    double g_phi_phi(double theta, double phi);
  private:
    // Semi-axis in x
		double a;
    // Semi-axis in y
		double b;
    // Semi-axis in c
		double c;
};

} // namespace Metrics
