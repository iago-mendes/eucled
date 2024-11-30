// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include "../Mesh/DataMesh.hpp"

/**
 * Integrate data assuming it's on the surface of a sphere
 */
double integrate(std::shared_ptr<DataMesh> data);

/**
 * Integrate norm of symmetric 2-tensor (with lowered indices) assuming it's on
 * the surface of a sphere
 */
double integrate(std::shared_ptr<DataMesh> data_theta_theta,
                 std::shared_ptr<DataMesh> data_theta_phi,
                 std::shared_ptr<DataMesh> data_phi_phi);
