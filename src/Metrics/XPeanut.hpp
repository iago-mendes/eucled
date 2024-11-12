#pragma once

#include "./Metric.hpp"

namespace Metrics {

/**
 * @brief 2-Metric of a peanut aligned with the x-axis
 */
class XPeanut : public Metric {
  public:
		XPeanut(double s0);
		XPeanut() = default;

    double g_theta_theta(double theta, double phi);
    double g_theta_phi(double theta, double phi);
    double g_phi_phi(double theta, double phi);
  private:
    // Thickness parameter
    double s0;
};

} // namespace Metrics
