#include "fidec.hpp"
using namespace std;

int main() {
	cout << "\n#########################################################\n";
	cout << "\tFinite-Difference Embedding Code (FiDEC)\n";
	cout << "#########################################################\n\n";
	
	cout << "Available surface metrics:\n";
	cout << "\t 1) RoundSphere\n";
	cout << "\t 2) OblateSpheroid\n";
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

		double value;
		cin >> value;

		pMetric->setParameter(parameter, value);
	}
}
