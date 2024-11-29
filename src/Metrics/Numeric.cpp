// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "./Numeric.hpp"

#include "H5Cpp.h"
// #include <string.h>

namespace Metrics {

Numeric::Numeric(std::string file_path, std::string horizon_key, std::string observation_id) {
  try{
    // Open given file and groups
    H5::H5File file(file_path, H5F_ACC_RDONLY);
    H5::Group horizon_group = file.openGroup(horizon_key);
    H5::Group observation_group = horizon_group.openGroup(observation_id);

    // Open data sets
    H5::DataSet dataset_theta_theta = observation_group.openDataSet("g_theta_theta");
    H5::DataSet dataset_phi_phi = observation_group.openDataSet("g_phi_phi");
    H5::DataSet dataset_theta_phi = observation_group.openDataSet("g_theta_phi");

    // Get mesh extents
    hsize_t extents[2];
    dataset_theta_theta.getSpace().getSimpleExtentDims(extents, nullptr);
    int N_theta = extents[0];
    int N_phi = extents[1];
    Mesh mesh(N_theta, N_phi);
    this->mesh = mesh;

    // Construct data matrices
    this->data_theta_theta = std::make_shared<std::vector<std::vector<double>>>(
          N_theta, std::vector<double>(N_phi));
    this->data_phi_phi = std::make_shared<std::vector<std::vector<double>>>(
          N_theta, std::vector<double>(N_phi));
    this->data_theta_phi = std::make_shared<std::vector<std::vector<double>>>(
          N_theta, std::vector<double>(N_phi));

    for (int i = 0; i < N_theta; i++) {
      for (int j = 0; j < N_phi; j++) {
        // Start at element (i, j)
        hsize_t offset[2] = {static_cast<hsize_t>(i), static_cast<hsize_t>(j)};
        // Read a single element
        hsize_t count[2] = {1, 1};

        // Select hyperslab for one element
        H5::DataSpace dataspace = dataset_theta_theta.getSpace();
        dataspace.selectHyperslab(H5S_SELECT_SET, count, offset);

        // Memory dataspace for one element
        H5::DataSpace memspace(2, count);

        // Temporary variable to hold the single value
        double value;

        // Read values
        dataset_theta_theta.read(&value, H5::PredType::NATIVE_DOUBLE, memspace, dataspace);
        (*this->data_theta_theta)[i][j] = value;
        dataset_phi_phi.read(&value, H5::PredType::NATIVE_DOUBLE, memspace, dataspace);
        (*this->data_phi_phi)[i][j] = value;
        dataset_theta_phi.read(&value, H5::PredType::NATIVE_DOUBLE, memspace, dataspace);
        (*this->data_theta_phi)[i][j] = value;
      }
    }

  } catch (H5::Exception &error) {
    error.printErrorStack();
    exit(1);
  }
}

double Numeric::g_theta_theta(double theta, double phi) {
  int i = this->mesh.i(theta);
  int j = this->mesh.j(phi);

  return (*data_theta_theta)[i][j];
}

double Numeric::g_theta_phi(double theta, double phi) {
  int i = this->mesh.i(theta);
  int j = this->mesh.j(phi);
  
  return (*data_theta_phi)[i][j];
}

double Numeric::g_phi_phi(double theta, double phi) {
  int i = this->mesh.i(theta);
  int j = this->mesh.j(phi);
  
  return (*data_phi_phi)[i][j];
}

} // namespace Metrics
