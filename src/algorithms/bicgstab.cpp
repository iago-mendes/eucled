#include "bicgstab.h"
using namespace std;

vector<double> linear_solver_rms_residuals;

bool is_residual_small_enough(vector<double> *p_residuals) {
	double rms_residual = get_rms(p_residuals);
	linear_solver_rms_residuals.push_back(rms_residual);

	return rms_residual <= BICGSTAB_MIN_ACCURACY;
}

// solves the equation 'A * solution = b'
// OBS.: the solution vector must have its initial data
void run_bicgstab(
	double (*A_operator)(const vector<double> *p_vect, int I),
	vector<double> *p_solution,
	const vector<double> *p_b
) {
	int N = p_solution->size();

	vector<double> b = *p_b;
	vector<double> base_vector(N, 0);
	linear_solver_rms_residuals.clear();

	int l = 1;
	int k = -l;

	// choose x0, r0~
	vector<double> x_0; x_0 = *p_solution;
	vector<double> r_tilde_0; // = r_0

	// compute r0 = b - Ax0
	vector<vector<double>> r(1, base_vector);
	for (int I = 0; I < N; I++)
		r[0][I] = b[I] - A_operator(&x_0, I);
	r_tilde_0 = r[0];
	
	// take u−1 = 0, x0 = x0, ρ0 = 1, α = 0, ω = 1.
	vector<double> u_negative_1(N, 0);
	vector<vector<double>> u(1, base_vector);
	vector<vector<double>> x(1, base_vector); x[0] = x_0;
	double rho_0 = 1;
	double alpha = 0;
	double omega = 1;

	int number_of_iterations = 0;
	// repeat until ||r[k+l]|| is small enough
	while (!is_residual_small_enough(&r[k+l])) {
		number_of_iterations++;
		// printf("(%d) R = %8.2e\n", number_of_iterations, get_rms(&r[k+l]));

		// k = k + l,
		k = k + l;

		// Put u^0 = uk−1, r^0 = rk and x^0 = xk.
		vector<vector<double>> u_hat(l+1, base_vector);
		if (k-1 < 0)
			u_hat[0] = u_negative_1;
		else
			u_hat[0] = u[k-1];
		vector<vector<double>> r_hat(l+1, base_vector); r_hat[0] = r[k];
		vector<double> x_hat_0; x_hat_0 = x[k];

		// ρ0 = −ωρ0
		rho_0 = - omega * rho_0;

		// For j = 0,...,l − 1
		for (int j = 0; j <= l-1; j++) {
			// ρ1 = (rbj , r˜0), β = βk+j = α ρ1/ρ0 , ρ0 = ρ1
			double rho_1 = dot_product(&r_hat[j], &r_tilde_0);
			double beta = alpha * rho_1 / rho_0;
			rho_0 = rho_1;

			// For i = 0,...,j
			for (int i = 0; i <= j; i++) {
				// r^i = r^i − αu^i+1
				for (int I = 0; I < N; I++)
					u_hat[i][I] = r_hat[i][I] - beta * u_hat[i][I];
			}
			
			// u^j+1 = Au^j
			for (int I = 0; I < N; I++)
				u_hat[j+1][I] = A_operator(&u_hat[j], I);
			
			// γ = (u^j+1, r˜0), α = αk+j = ρ0/γ
			double gamma = dot_product(&u_hat[j+1], &r_tilde_0);
			alpha = rho_0 / gamma;

			// For i = 0,...,j
			for (int i = 0; i <= j; i++) {
				// r^i = r^i − αu^i+1
				for (int I = 0; I < N; I++)
					r_hat[i][I] = r_hat[i][I] - alpha * u_hat[i+1][I];
			}

			// r^j+1 = Ar^j, x^0 = x^0 + αu^0
			for (int I = 0; I < N; I++) {
				r_hat[j+1][I] = A_operator(&r_hat[j], I);
				x_hat_0[I] = x_hat_0[I] + alpha * u_hat[0][I];
			}
		}
		
		vector<vector<double>> tau(l+1, base_vector);
		for (int i = 0; i < l+1; i++)
			tau[i].resize(l+1, 0);
		
		vector<double> sigma(l+1, 1);
		vector<double> gamma_prime(l+1, 1);

		// For j = 1,...,l
		for (int j = 1; j <= l; j++) {
			// For i = 1,...,j − 1
			for (int i = 1; i <= j-1; i++) {
				// τij = 1 / σi (r^j , r^i)
				// NOT SURE WHAT σi SHOULD BE
				tau[i][j] = 1 / sigma[i] * dot_product(&r_hat[j], &r_hat[i]);

				// r^j = r^j − τij r^i
				for (int I = 0; I < N; I++)
					r_hat[j][I] = r_hat[j][I] - tau[i][j] * r_hat[i][I];
			}
			
			// σj = (r^j , r^j), γ'j = 1/σj (r^0, r^j)
			sigma[j] = dot_product(&r_hat[j], &r_hat[j]);
			gamma_prime[j] = 1 / sigma[j] * dot_product(&r_hat[0], &r_hat[j]);
		}
		
		// γl = γ'l, ω = γl
		vector<double> gamma(l+1, 1);
		gamma[l] = gamma_prime[l];
		omega = gamma[l];

		// For j = l − 1,..., 1
		for (int j = l-1; j >= 1; j--) {
			// γj = γ'j − SUM_{i=j+1}^{l} τji γi
			double sum = 0;
			for (int i = j+1; i <= l; i++)
				sum += tau[j][i] * gamma[i+1];
			gamma[j] = gamma_prime[j] - sum;
		}
		
		vector<double> gamma_prime_prime(l+1, 1);

		// For j = 1,...,l − 1
		for (int j = 1; j <= l-1; j++) {
			// γ''j = γj+1 − SUM_{i=j+1}^{l-1} τji γi+1
			double sum = 0;
			for (int i = j+1; i <= l-1; i++)
				sum += tau[j][i] * gamma[i+1];
			gamma_prime_prime[j] = gamma[j+1] - sum;
		}
		
		// x^0 = x^0 + γ1 r^0, r^0 = r^0 − γ'l r^l, u^0 = u^0 − γl u^l
		for (int I = 0; I < N; I++) {
			x_hat_0[I] = x_hat_0[I] + gamma[1] * r_hat[0][I];
			r_hat[0][I] = r_hat[0][I] - gamma_prime[l] * r_hat[l][I];
			u_hat[0][I] = u_hat[0][I] - gamma[l] * u_hat[l][I];
		}

		// For j = 1,...,l − 1
		for (int j = 1; j <= l-1; j++) {
			for (int I = 0; I < N; I++) {
				u_hat[0][I] = u_hat[0][I] - gamma[j] * u_hat[j][I];
				x_hat_0[I] = x_hat_0[I] + gamma_prime_prime[j] * r_hat[j][I];
				r_hat[0][I] = r_hat[0][I] - gamma_prime[j] * r_hat[j][I];
			}
		}

		// Check for NaN's.
		bool has_nan = false;
		for (int I = 0; I < N; I++) {
			if (isnan(x_hat_0[I])) {
				has_nan = true;
        break;
			}
		}
		if (has_nan) {
			printf("(NaN detected at i = %d) ", number_of_iterations);
			break;
		}
		
		// Put uk+l−1 = u^0, rk+l = r^0 and xk+l = x^0.
		u.resize(k+l+1, base_vector); u[k+l-1] = u_hat[0];
		r.resize(k+l+1, base_vector); r[k+l] = r_hat[0];
		x.resize(k+l+1, base_vector); x[k+l] = x_hat_0;

		if (number_of_iterations >= BICGSTAB_MAX_ITERATIONS) {
			// printf("[(BiCGSTAB) Reached maximum number of iterations] ");
			break;
		}
	}

	double final_rms_residual = linear_solver_rms_residuals[linear_solver_rms_residuals.size() - 1];
	printf("BiCGSTAB ran %d times and finished with R = %8.2e\n", number_of_iterations, final_rms_residual);
	
	*p_solution = x[x.size() - 1];
}
