#pragma once

#include "../Mesh/DataMesh3D.hpp"

std::shared_ptr<DataMesh3D> get_commutator(std::shared_ptr<DataMesh3D> e_theta, std::shared_ptr<DataMesh3D> e_phi);
double get_commutator_rms(std::shared_ptr<DataMesh3D> e_theta, std::shared_ptr<DataMesh3D> e_phi);
