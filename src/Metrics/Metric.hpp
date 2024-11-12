#pragma once

/**
 * @brief Interface for the metric of 2-surfaces
 */
class Metric {
  public:
    virtual double g_theta_theta(double theta, double phi) = 0;
    virtual double g_theta_phi(double theta, double phi) = 0;
    virtual double g_phi_phi(double theta, double phi) = 0;
};
