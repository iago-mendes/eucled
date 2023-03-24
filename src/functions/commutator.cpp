#include "commutator.h"

shared_ptr<Grid3DFunction> get_commutator(shared_ptr<Grid3DFunction> e_theta, shared_ptr<Grid3DFunction> e_phi) {
	auto commutator = e_phi->partial_theta()->added_with(e_theta->partial_phi(), -1);

	return commutator;
}

double get_commutator_rms(shared_ptr<Grid3DFunction> e_theta, shared_ptr<Grid3DFunction> e_phi) {
	auto sqrt_sin_theta = [](double theta, [[maybe_unused]] double phi) {return sqrt(sin(theta));};

	auto commutator = get_commutator(e_theta, e_phi);

	return commutator->multiplied_by(sqrt_sin_theta)->norm()->rms();
}
