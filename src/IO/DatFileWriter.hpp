// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <variant>

/**
 * Writes data into a .dat file
 */
class DatFileWriter {
  public:
    using DataType = std::variant<int, double>;

    explicit DatFileWriter(const std::string& filename);
    ~DatFileWriter();

    void write_header(const std::vector<std::string>& variable_names);
    void write_data(const std::vector<DataType>& variable_data);

  private:
    std::ofstream file;
};
