#pragma once

#include "./Metric.hpp"

namespace Metrics {

/**
 * @brief 2-Metric of a sphere
 */
class Sphere : public Metric {
  public:
		Sphere(double R);
		Sphere() = default;

    double g_theta_theta(double theta, double phi);
    double g_theta_phi(double theta, double phi);
    double g_phi_phi(double theta, double phi);
  private:
    // Radius
		double R;
};

} // namespace Metrics
