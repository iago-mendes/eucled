#include "fidec.hpp"
using namespace std;

int main() {
	cout << "\n#########################################################\n";
	cout << "\tFinite-Difference Embedding Code (FiDEC)\n";
	cout << "#########################################################\n\n";
	
	cout << "Available surface metrics:\n";
	cout << "\t 1) Round Sphere\n";
	cout << "\t 2) Oblate Spheroid\n";
	cout << "\t 3) Dented Sphere\n";
	cout << "\t 4) Ellipsoid\n";
	cout << "\t 5) Z-Peanut\n";
	cout << "\t 6) Non-axisymmetric z-Peanut\n";
	cout << "\t 7) X-Peanut\n";
	cout << "\t 8) Kerr Horizon\n";
	cout << "\t 9) Numerical\n";
	cout << "\n";

	shared_ptr<Metric> pMetric = nullptr;

	bool choseValidMetric = false;
	while (!choseValidMetric) {
		cout << "Choose a surface metric (1, 2, ...): ";

		string input;
		getline(cin, input);

		int metricChoice;
		try {
			metricChoice = stoi(input);
		}
		catch(const std::exception& e) {
			metricChoice = -1;
		}

		// Assume it's valid
		choseValidMetric = true;

		switch (metricChoice) {
			case 1:
				pMetric = make_shared<RoundSphereMetric>();
				break;

			case 2:
				pMetric = make_shared<OblateSpheroidMetric>();
				break;

			case 3:
				pMetric = make_shared<DentedSphereMetric>();
				break;
			
			case 4:
				pMetric = make_shared<EllipsoidMetric>();
				break;

			case 5:
				pMetric = make_shared<ZPeanutMetric>();
				break;

			case 6:
				pMetric = make_shared<NonaxisymmetricZPeanutMetric>();
				break;

			case 7:
				pMetric = make_shared<XPeanutMetric>();
				break;

			case 8:
				pMetric = make_shared<KerrHorizonMetric>();
				break;

			case 9:
				pMetric = make_shared<NumericalMetric>();
				break;

			default:
				cout << "Invalid value!\n\n";
				choseValidMetric = false;
				break;
		}
	}
	cout << "\n";

	for (int i = 0; i < (int) pMetric->parameters.size(); i++) {
		string parameter = pMetric->parameters[i];

		cout << parameter << " = ";

		string input;
		getline(cin, input);

		pMetric->set_parameter(parameter, input);
	}
}
