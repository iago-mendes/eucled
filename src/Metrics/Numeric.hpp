#pragma once

#include "./Metric.hpp"

#include "../classes/grid.h"

namespace Metrics {

class Numeric : public Metric {
  public:
    Grid grid;
    
    Numeric(std::string file_path, std::string horizon_key, std::string observation_id);

    double g_theta_theta(double theta, double phi);
    double g_theta_phi(double theta, double phi);
    double g_phi_phi(double theta, double phi);
  private:
    shared_ptr<std::vector<std::vector<double>>> data_theta_theta;
    shared_ptr<std::vector<std::vector<double>>> data_theta_phi;
    shared_ptr<std::vector<std::vector<double>>> data_phi_phi;
};

} // namespace Metrics
