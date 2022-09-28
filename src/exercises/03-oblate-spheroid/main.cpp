#include "main.h"
using namespace std;

double N_theta;
double N_phi;

// Radius of round sphere
double R = 1;

// Dimensions of oblate spheroid
double a = 1;
double b = 2;

Grid grid;
RoundSphereDyad dyad(R);

void check_dot_products(double theta_scaling_factor, double phi_scaling_factor) {
	printf("\n-------------------------------------------------------------\n");
	printf("Theta scale = %f; Phi scale = %f:\n\n", theta_scaling_factor, phi_scaling_factor);

	bool e_theta_e_theta_is_correct = true;
	bool e_theta_e_phi_is_correct = true;
	bool e_phi_e_phi_is_correct = true;

	for (int i = 0; i < grid.N_theta; i++) {
		double theta = grid.theta(i);

		for (int j = 0; j < grid.N_phi; j++) {
			double phi = grid.theta(j);

			double e_theta_e_theta =
				squared(theta_scaling_factor * dyad.e_theta_x(theta, phi)) +
				squared(theta_scaling_factor * dyad.e_theta_y(theta, phi)) +
				squared(theta_scaling_factor * dyad.e_theta_z(theta, phi));
			double e_theta_e_theta_residual = e_theta_e_theta - squared(a);
			
			double e_phi_e_phi =
				squared(phi_scaling_factor * dyad.e_phi_x(theta, phi)) +
				squared(phi_scaling_factor * dyad.e_phi_y(theta, phi)) +
				squared(phi_scaling_factor * dyad.e_phi_z(theta, phi));
			double e_phi_e_phi_residual = e_phi_e_phi - squared(b) * squared(sin(theta));
			
			double e_theta_e_phi =
				theta_scaling_factor * dyad.e_theta_x(theta, phi) * phi_scaling_factor * dyad.e_phi_x(theta, phi) +
				theta_scaling_factor * dyad.e_theta_y(theta, phi) * phi_scaling_factor * dyad.e_phi_y(theta, phi) +
				theta_scaling_factor * dyad.e_theta_z(theta, phi) * phi_scaling_factor * dyad.e_phi_z(theta, phi);
			double e_theta_e_phi_residual = e_theta_e_phi - 0;

			printf("i = %d, j = %d:\n", i, j);
			printf("\tR(e_theta . e_theta) = %.5e\n", e_theta_e_theta_residual);
			printf("\tR(e_theta . e_phi) = %.5e\n", e_theta_e_phi_residual);
			printf("\tR(e_phi . e_phi) = %.5e\n\n", e_phi_e_phi_residual);
			
			if (abs(e_theta_e_theta_residual) > MAX_RESIDUAL) {
				e_theta_e_theta_is_correct = false;
			}
			if (abs(e_theta_e_phi_residual) > MAX_RESIDUAL) {
				e_theta_e_phi_is_correct = false;
			}
			if (abs(e_phi_e_phi_residual) > MAX_RESIDUAL) {
				e_phi_e_phi_is_correct = false;
			}
		}
	}

	if (e_theta_e_theta_is_correct && e_theta_e_phi_is_correct && e_phi_e_phi_is_correct) {
		printf(GREEN "The dot products of the dyad components are CORRECT!\n" RESET);
	} else {
		printf(RED "The dot products of the dyad components are INCORRECT!\n" RESET);
	}
}

int main() {
	printf("Exercise 3: Dot Products of Dyad Components for a Oblate Spheroid.\n\n");

	printf("N_theta = ");
	cin >> N_theta;

	printf("N_phi = ");
	cin >> N_phi;

	Grid grid_(N_theta, N_phi);
	grid = grid_;

	check_dot_products(1, 1);
	check_dot_products(a / R, b / R);
}
