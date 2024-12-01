// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "../Mesh/DataMesh.hpp"
#include "../Mesh/Mesh.hpp"

/**
 * Writes a data mesh into a .vol file
 */
class VolFileWriter {
  public:
    explicit VolFileWriter(const std::string& filename);
    ~VolFileWriter();

    void write_mesh(Mesh& mesh);
    void write_data(const std::shared_ptr<DataMesh>& data);

  private:
    std::ofstream file;
    std::string delimiter = ",";
};
