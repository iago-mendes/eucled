#include "main.h"
using namespace std;

int main() {
	printf("Exercise 1: Dot Products of dyad components.\n\n");

	double N_theta;
	printf("N_theta = ");
	cin >> N_theta;

	double N_phi;
	printf("N_phi = ");
	cin >> N_phi;

	Grid grid(N_theta, N_phi);

	// Radius of round sphere
	double R = 1;

	RoundSphereDyad dyad(R);

	printf("\n-------------------------------------------------------------\n");
	printf("Dot products residuals for each grid point:\n\n");

	bool e_theta_e_theta_is_correct = true;
	bool e_theta_e_phi_is_correct = true;
	bool e_phi_e_phi_is_correct = true;

	for (int i = 0; i < grid.N_theta; i++) {
		double theta = grid.theta(i);

		for (int j = 0; j < grid.N_phi; j++) {
			double phi = grid.theta(j);

			double e_theta_e_theta =
				squared(dyad.e_theta_x(theta, phi)) +
				squared(dyad.e_theta_y(theta, phi)) +
				squared(dyad.e_theta_z(theta, phi));
			double e_theta_e_theta_residual = e_theta_e_theta - squared(R);
			
			double e_phi_e_phi =
				squared(dyad.e_phi_x(theta, phi)) +
				squared(dyad.e_phi_y(theta, phi)) +
				squared(dyad.e_phi_z(theta, phi));
			double e_phi_e_phi_residual = e_phi_e_phi - squared(R) * squared(sin(theta));
			
			double e_theta_e_phi =
				dyad.e_theta_x(theta, phi) * dyad.e_phi_x(theta, phi) +
				dyad.e_theta_y(theta, phi) * dyad.e_phi_y(theta, phi) +
				dyad.e_theta_z(theta, phi) * dyad.e_phi_z(theta, phi);
			double e_theta_e_phi_residual = e_theta_e_phi - 0;

			printf("i = %d, j = %d:\n", i, j);
			printf("\tR(e_theta . e_theta) = %.5e\n", e_theta_e_theta_residual);
			printf("\tR(e_theta . e_phi) = %.5e\n", e_theta_e_phi_residual);
			printf("\tR(e_phi . e_phi) = %.5e\n\n", e_phi_e_phi_residual);
			
			if (e_theta_e_theta_residual > MAX_RESIDUAL) {
				e_theta_e_theta_is_correct = false;
			}
			if (e_theta_e_phi_residual > MAX_RESIDUAL) {
				e_theta_e_phi_is_correct = false;
			}
			if (e_phi_e_phi_residual > MAX_RESIDUAL) {
				e_phi_e_phi_is_correct = false;
			}
		}
	}

	printf("-------------------------------------------------------------\n\n");

	if (e_theta_e_theta_is_correct && e_theta_e_phi_is_correct && e_phi_e_phi_is_correct) {
		printf(GREEN "The dot products of the dyad components are CORRECT!\n" RESET);
	} else {
		printf(RED "The dot products of the dyad components are INCORRECT!\n" RESET);
	}
}
