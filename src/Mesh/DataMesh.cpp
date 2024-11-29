#include "./DataMesh.hpp"
#include "../utils/rms.h"

#include "math.h"
#include <memory>
#include <vector>

DataMesh::DataMesh(Mesh mesh, double (*function)(int i, int j)) {
	this->mesh = mesh;

	std::vector<double> base_vector(mesh.N_phi, 0.);
	this->points.resize(mesh.N_theta, base_vector);

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			this->points[i][j] = function(i, j);
		}
	}

	cached_partial_theta = nullptr;
	cached_partial_phi = nullptr;
}

DataMesh::DataMesh(Mesh mesh, std::vector<std::vector<double>> *points) {
	this->mesh = mesh;

	std::vector<double> base_vector(mesh.N_phi, 0);
	this->points.resize(mesh.N_theta, base_vector);

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			this->points[i][j] = (*points)[i][j];
		}
	}

	this->cached_partial_theta = nullptr;
	this->cached_partial_phi = nullptr;
}

DataMesh::DataMesh(Mesh mesh) {
	this->mesh = mesh;

	std::vector<double> base_vector(mesh.N_phi, 0.);
	this->points.resize(mesh.N_theta, base_vector);

	this->cached_partial_theta = nullptr;
	this->cached_partial_phi = nullptr;
}

std::shared_ptr<DataMesh> DataMesh::get_copy() {
	auto copy = std::make_shared<DataMesh>(mesh);

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			copy->points[i][j] = this->points[i][j];
		}
	}

	return copy;
}

std::shared_ptr<DataMesh> DataMesh::partial_theta() {
	if (cached_partial_theta != nullptr) {
		return cached_partial_theta;
	}

	auto new_function = get_copy();

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			if (i == 0) { // forward
				// 1st order
				new_function->points[i][j] = -1. * points[0][j] +1. * points[1][j];
			}
			else if (i == mesh.N_theta - 1) { // backward
				// 1st order
				new_function->points[i][j] = -1. * points[mesh.N_theta-2][j] +1. * points[mesh.N_theta-1][j];
			}
			else { // centered
				new_function->points[i][j] = -1./2. * points[i-1][j] +1./2. * points[i+1][j];
			}

			new_function->points[i][j] /= mesh.delta_theta;
		}
	}

	cached_partial_theta = new_function;

	return new_function;
}

std::shared_ptr<DataMesh> DataMesh::partial_phi() {
	if (cached_partial_phi != nullptr) {
		return cached_partial_phi;
	}

	auto new_function = get_copy();

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			double df;

			if (j == 0) {
				df = points[i][1] - points[i][mesh.N_phi - 1];
			}
			else if (j == mesh.N_phi - 1) {
				df = points[i][0] - points[i][mesh.N_phi - 2];
			}
			else {
				df = points[i][j + 1] - points[i][j - 1];
			}
			
			new_function->points[i][j] = df / (2 * mesh.delta_phi);
		}
	}

	cached_partial_phi = new_function;

	return new_function;
}

double DataMesh::rms() {
	return get_rms(&this->points);
}

std::shared_ptr<DataMesh> DataMesh::operator+(double constant) {
	std::shared_ptr<DataMesh> new_data = this->get_copy();

	for (int i = 0; i < this->mesh.N_theta; i++) {
		for (int j = 0; j < this->mesh.N_phi; j++) {
			new_data->points[i][j] += constant;
		}
	}

	return new_data;
}

std::shared_ptr<DataMesh> DataMesh::operator+(std::shared_ptr<DataMesh> other) {
	std::shared_ptr<DataMesh> new_data = this->get_copy();

	for (int i = 0; i < this->mesh.N_theta; i++) {
		for (int j = 0; j < this->mesh.N_phi; j++) {
			new_data->points[i][j] += other->points[i][j];
		}
	}

	return new_data;
}

std::shared_ptr<DataMesh> DataMesh::operator+(double (*function)(double theta, double phi)) {
	std::shared_ptr<DataMesh> new_data = this->get_copy();

	for (int i = 0; i < this->mesh.N_theta; i++) {
		double theta = this->mesh.theta(i);

		for (int j = 0; j < this->mesh.N_phi; j++) {
			double phi = this->mesh.phi(j);

			new_data->points[i][j] += function(theta, phi);
		}
	}

	return new_data;
}

std::shared_ptr<DataMesh> DataMesh::operator*(double constant) {
	std::shared_ptr<DataMesh> new_data = get_copy();

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			new_data->points[i][j] *= constant;
		}
	}

	return new_data;
}

std::shared_ptr<DataMesh> DataMesh::operator*(std::shared_ptr<DataMesh> other) {
	std::shared_ptr<DataMesh> new_data = this->get_copy();

	for (int i = 0; i < this->mesh.N_theta; i++) {
		for (int j = 0; j < this->mesh.N_phi; j++) {
			new_data->points[i][j] *= other->points[i][j];
		}
	}

	return new_data;
}

std::shared_ptr<DataMesh> DataMesh::operator*(double (*function)(double theta, double phi)) {
	std::shared_ptr<DataMesh> new_data = get_copy();

	for (int i = 0; i < mesh.N_theta; i++) {
		double theta = mesh.theta(i);

		for (int j = 0; j < mesh.N_phi; j++) {
			double phi = mesh.phi(j);
			
			new_data->points[i][j] *= function(theta, phi);
		}
	}

	return new_data;
}

// Operator overloads for shared_ptr<DataMesh>

std::shared_ptr<DataMesh> operator+(std::shared_ptr<DataMesh> ptr, double constant) {
	return (*ptr) + constant;
}
std::shared_ptr<DataMesh> operator+(std::shared_ptr<DataMesh> ptr, std::shared_ptr<DataMesh> other) {
	return (*ptr) + other;
}
std::shared_ptr<DataMesh> operator+(std::shared_ptr<DataMesh> ptr, double (*function)(double theta, double phi)) {
	return (*ptr) + function;
}

std::shared_ptr<DataMesh> operator*(std::shared_ptr<DataMesh> ptr, double constant) {
	return (*ptr) * constant;
}
std::shared_ptr<DataMesh> operator*(std::shared_ptr<DataMesh> ptr, std::shared_ptr<DataMesh> other) {
	return (*ptr) * other;
}
std::shared_ptr<DataMesh> operator*(std::shared_ptr<DataMesh> ptr, double (*function)(double theta, double phi)) {
	return (*ptr) * function;
}
