// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include "./Metric.hpp"

namespace Metrics {

/**
 * @brief 2-Metric of a Kerr horizon
 */
class KerrHorizon : public Metric {
	public:
		KerrHorizon(double chi);
		KerrHorizon() = default;

		double g_theta_theta(double theta, double phi);
		double g_theta_phi(double theta, double phi);
		double g_phi_phi(double theta, double phi);

  private:
    // Dimensionless spin
    double chi;

    // Useful parameters
		double rho();
		double sigma(double theta);
};

} // namespace Metrics
