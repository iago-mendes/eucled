#pragma once

#include "../Mesh/DataMesh3D.hpp"

std::shared_ptr<DataMesh3D> get_cross_product(std::shared_ptr<DataMesh3D> vector_a, std::shared_ptr<DataMesh3D> vector_b);
