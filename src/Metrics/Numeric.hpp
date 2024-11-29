#pragma once

#include <memory>
#include <string>
#include <vector>

#include "./Metric.hpp"
#include "../Mesh/Mesh.hpp"

namespace Metrics {

class Numeric : public Metric {
  public:
    Mesh mesh;
    
    Numeric(std::string file_path, std::string horizon_key, std::string observation_id);

    double g_theta_theta(double theta, double phi);
    double g_theta_phi(double theta, double phi);
    double g_phi_phi(double theta, double phi);
  private:
    std::shared_ptr<std::vector<std::vector<double>>> data_theta_theta;
    std::shared_ptr<std::vector<std::vector<double>>> data_theta_phi;
    std::shared_ptr<std::vector<std::vector<double>>> data_phi_phi;
};

} // namespace Metrics
