#include "relaxation.h"
using namespace std;

shared_ptr<Grid3DFunction> e_theta__relaxation(nullptr);
shared_ptr<Grid3DFunction> e_phi__relaxation(nullptr);

double time_step = 0.0025;

double sin_multiplier(
	double theta,
	[[maybe_unused]] double phi,
	[[maybe_unused]] char coordinate
) {
	return sin(theta);
}

double inverse_sin_multiplier(
	double theta,
	[[maybe_unused]] double phi,
	[[maybe_unused]] char coordinate
) {
	return 1 / sin(theta);
}

double negate_multiplier(
	[[maybe_unused]] double theta,
	[[maybe_unused]] double phi,
	[[maybe_unused]] char coordinate
) {
	return - 1;
}

void update_e_theta() {
	shared_ptr<Grid3DFunction> e_theta = e_theta__relaxation;
	shared_ptr<Grid3DFunction> e_phi = e_phi__relaxation;

	shared_ptr<Grid3DFunction> left_side = get_commutator(e_theta, e_phi);
	shared_ptr<Grid3DFunction> right_side = get_commutator(e_theta, e_phi);

	left_side = (*left_side).multiplied_by(sin_multiplier);
	right_side = (*right_side).multiplied_by(sin_multiplier);

	if (isnan((*left_side).rms()) || isnan((*right_side).rms())) {
		printf("NaN 1\n");
	}

	left_side = (*left_side).partial_theta();
	right_side = (*right_side).partial_phi();

	if (isnan((*left_side).rms()) || isnan((*right_side).rms())) {
		printf("NaN 2\n");
	}

	left_side = get_cross_product(left_side, e_phi);
	right_side = get_cross_product(right_side, e_theta);

	if (isnan((*left_side).rms()) || isnan((*right_side).rms())) {
		printf("NaN 3\n");
	}

	left_side = (*left_side).multiplied_by(inverse_sin_multiplier);
	right_side = (*right_side).multiplied_by(inverse_sin_multiplier);

	if (isnan((*left_side).rms()) || isnan((*right_side).rms())) {
		printf("NaN 4\n");
	}

	shared_ptr<Grid3DFunction> e_theta_derivative = (*left_side).added_with(right_side, -1);

	e_theta_derivative = get_cross_product(e_theta_derivative, e_theta);

	e_theta_derivative = (*e_theta_derivative).multiplied_by(negate_multiplier);

	e_theta__relaxation = (*e_theta__relaxation).added_with(e_theta_derivative, time_step);
}

void update_e_phi() {
	shared_ptr<Grid3DFunction> e_theta = e_theta__relaxation;
	shared_ptr<Grid3DFunction> e_phi = e_phi__relaxation;

	shared_ptr<Grid3DFunction> left_side = get_commutator(e_theta, e_phi);
	shared_ptr<Grid3DFunction> right_side = get_commutator(e_theta, e_phi);

	left_side = (*left_side).multiplied_by(sin_multiplier);
	right_side = (*right_side).multiplied_by(sin_multiplier);

	left_side = (*left_side).partial_theta();
	right_side = (*right_side).partial_phi();

	left_side = get_cross_product(left_side, e_phi);
	right_side = get_cross_product(right_side, e_theta);

	left_side = (*left_side).multiplied_by(inverse_sin_multiplier);
	right_side = (*right_side).multiplied_by(inverse_sin_multiplier);

	shared_ptr<Grid3DFunction> e_phi_derivative = (*left_side).added_with(right_side, -1);

	e_phi_derivative = get_cross_product(e_phi_derivative, e_phi);

	e_phi_derivative = (*e_phi_derivative).multiplied_by(negate_multiplier);

	e_phi__relaxation = (*e_phi__relaxation).added_with(e_phi_derivative, time_step);
}

double run_relaxation(
	shared_ptr<Grid3DFunction> e_theta,
	shared_ptr<Grid3DFunction> e_phi,
	double (*get_residual)(shared_ptr<Grid3DFunction> e_theta, shared_ptr<Grid3DFunction> e_phi)
) {
	e_theta__relaxation = e_theta;
	e_phi__relaxation = e_phi;

	double residual = abs(get_residual(e_theta__relaxation, e_phi__relaxation));
	int iteration_number = 0;
	while (
		residual > RESIDUAL_TOLERANCE &&
		iteration_number < MAX_ITERATIONS
	) {
		if (iteration_number % 10 == 0)
			printf("(%d) %f\n", iteration_number, residual);
		
		update_e_theta();
		update_e_phi();

		residual = get_residual(e_theta__relaxation, e_phi__relaxation);
		iteration_number++;
	}

	(*e_theta) = (*e_theta__relaxation);
	(*e_phi) = (*e_phi__relaxation);

	printf("Relaxation finished with R = %8.2e after %5d iterations.\n", residual, iteration_number);
	return residual;
}
