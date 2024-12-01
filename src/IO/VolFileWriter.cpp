// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "./VolFileWriter.hpp"

#include <iomanip>
#include <stdexcept>

VolFileWriter::VolFileWriter(const std::string& filename) : file(filename, std::ios::out) {
  if (!file.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + filename);
  }
}

VolFileWriter::~VolFileWriter() {
  if (file.is_open()) {
    file.close();
  }
}

void VolFileWriter::write_mesh(Mesh& mesh) {
  if (file.is_open()) {
    // Write the mesh theta values
    for (int i = 0; i < mesh.N_theta; i++) {
      for (int j = 0; j < mesh.N_phi; j++) {
        file << mesh.theta(i);
        if (i == mesh.N_theta - 1 && j == mesh.N_phi - 1) {
          file << std::endl;
        } else {
          file << delimiter;
        }
      }
    }

    // Write the mesh phi values
    for (int i = 0; i < mesh.N_theta; i++) {
      for (int j = 0; j < mesh.N_phi; j++) {
        file << mesh.phi(j);
        if (i == mesh.N_theta - 1 && j == mesh.N_phi - 1) {
          file << std::endl;
        } else {
          file << delimiter;
        }
      }
    }
  }
}

void VolFileWriter::write_data(const std::shared_ptr<DataMesh>& data) {
  if (file.is_open()) {
    for (int i = 0; i < data->mesh.N_theta; i++) {
      for (int j = 0; j < data->mesh.N_phi; j++) {
        file << data->points[i][j];
        if (i == data->mesh.N_theta - 1 && j == data->mesh.N_phi - 1) {
          file << std::endl;
        } else {
          file << delimiter;
        }
      }
    }
  }
}
