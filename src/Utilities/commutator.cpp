// Distributed under the MIT License.
// See LICENSE.txt for details.

#include "./commutator.hpp"

#include <math.h>

#include "./math.hpp"

std::shared_ptr<DataMesh3D> get_commutator(std::shared_ptr<DataMesh3D> e_theta, std::shared_ptr<DataMesh3D> e_phi) {
	auto commutator = e_phi->partial_theta() - e_theta->partial_phi();

	return commutator;
}

double get_commutator_rms(std::shared_ptr<DataMesh3D> e_theta, std::shared_ptr<DataMesh3D> e_phi) {
	auto commutator = get_commutator(e_theta, e_phi);

	return (commutator * sqrt_sin_theta)->norm()->rms();
}
