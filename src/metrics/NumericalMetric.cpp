#include "NumericalMetric.hpp"

NumericalMetric::NumericalMetric() {
	parameters.push_back("Data File");
}

void NumericalMetric::set_parameter(std::string parameter, std::string value) {
	if (parameter == "Data File") {
		this->read_data(value);
	}
}

void NumericalMetric::read_data([[maybe_unused]] std::string file_name) {
	// TODO: make this method read file data

	int N_theta, N_phi;
	cin >> N_theta >> N_phi;

	this->pGrid = std::make_shared<Grid>(N_theta, N_phi);

	for (int i = 0; i < N_theta; i++) {
		vector<double> phi_values[3];

		for (int j = 0; j < N_phi; j++) {
			double point[3];
			cin >> point[0] >> point[1] >> point[2];

			phi_values[0].push_back(point[0]);
			phi_values[1].push_back(point[1]);
			phi_values[2].push_back(point[2]);
		}

		data[0].push_back(phi_values[0]);
		data[1].push_back(phi_values[1]);
		data[2].push_back(phi_values[2]);
	}
}

double NumericalMetric::g_theta_theta([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	int i = this->pGrid->i(theta);
	int j = this->pGrid->j(phi);

	return this->data[0][i][j];
}

double NumericalMetric::g_theta_phi([[maybe_unused]] double theta, [[maybe_unused]] double phi) {
	int i = this->pGrid->i(theta);
	int j = this->pGrid->j(phi);
	
	return this->data[2][i][j];
}

double NumericalMetric::g_phi_phi(double theta, [[maybe_unused]] double phi) {
	int i = this->pGrid->i(theta);
	int j = this->pGrid->j(phi);
	
	return this->data[1][i][j];
}
