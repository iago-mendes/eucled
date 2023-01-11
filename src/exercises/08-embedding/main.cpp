#include "main.h"
using namespace std;

#include "main.h"
using namespace std;

void find_solution(int N_theta, int N_phi) {
	Grid grid(N_theta, N_phi);

	char identifier[50];
	sprintf(identifier, "ellipsoid_%dx%d", N_theta, N_phi);

	shared_ptr<Metric> metric = make_shared<EllipsoidMetric>(1.5, 1, 1);
	shared_ptr<Grid3DFunction> embedding = make_shared<Grid3DFunction>(grid);

	run_embedding(metric, embedding, identifier);

	printf("\n\nDone!\n");
}

int main() {
	// Single run.
	find_solution(10, 4*10);

	// Vary grid space.
	// for (int N = 10; N <= 40; N *= 2) {
	// 	printf("\n%dx%d\n", N, 4*N);

	// 	find_solution(N, 4*N);
	// }
}
