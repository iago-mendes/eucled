#pragma once

#include "./Metric.hpp"

namespace Metrics {

/**
 * @brief 2-Metric of a peanut aligned with the z-axis
 */
class ZPeanut : public Metric {
  public:
		ZPeanut(double r0, double b, double e);
		ZPeanut() = default;

    double g_theta_theta(double theta, double phi);
    double g_theta_phi(double theta, double phi);
    double g_phi_phi(double theta, double phi);
  private:
    // Radius
    double r0; 
    // Separation
		double b;
    // Non-axisymmetry
		double e;

    double r(double theta, double phi);
		double partial_theta_r(double theta, double phi);
		double partial_phi_r(double theta, double phi);
};

} // namespace Metrics
