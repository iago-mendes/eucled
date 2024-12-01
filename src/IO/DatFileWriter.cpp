// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "./DatFileWriter.hpp"

#include <stdexcept>

DatFileWriter::DatFileWriter(const std::string& filename) : file(filename, std::ios::out) {
  if (!file.is_open()) {
    throw std::ios_base::failure("Failed to open file: " + filename);
  }
}

DatFileWriter::~DatFileWriter() {
  if (file.is_open()) {
    file.close();
  }
}

void DatFileWriter::write_header(const std::vector<std::string>& variable_names) {
  if (file.is_open()) {
    for (size_t i = 0; i < variable_names.size(); ++i) {
      file << "# [" << i + 1 << "] = " << variable_names[i] << std::endl;
    }
  }
}

void DatFileWriter::write_data(const std::vector<DataType>& variable_data) {
  if (file.is_open()) {
    for (const auto& value : variable_data) {
      std::visit([&](auto&& v) {
        file << std::scientific << std::setprecision(16)
             << std::setw(25) << v;
      }, value);
    }
    file << std::endl;
  }
}
