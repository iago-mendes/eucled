#include "./DataMesh3D.hpp"
#include "./DataMesh.hpp"
#include "../utils/rms.h"

#include "math.h"
#include <memory>
#include <vector>

DataMesh3D::DataMesh3D(Mesh mesh, double (*function)(int i, int j, char coord)) {
	this->mesh = mesh;

	std::vector<double> base_vector(mesh.N_phi, 0);
	x_points.resize(mesh.N_theta, base_vector);
	y_points.resize(mesh.N_theta, base_vector);
	z_points.resize(mesh.N_theta, base_vector);

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			x_points[i][j] = function(i, j, 'x');
			y_points[i][j] = function(i, j, 'y');
			z_points[i][j] = function(i, j, 'z');
		}
	}
}

DataMesh3D::DataMesh3D(Mesh mesh, double (*function)(double theta, double phi, char coord)) {
	this->mesh = mesh;

	std::vector<double> base_vector(mesh.N_phi, 0);
	x_points.resize(mesh.N_theta, base_vector);
	y_points.resize(mesh.N_theta, base_vector);
	z_points.resize(mesh.N_theta, base_vector);

	for (int i = 0; i < mesh.N_theta; i++) {
		double theta = mesh.theta(i);
		
		for (int j = 0; j < mesh.N_phi; j++) {
			double phi = mesh.phi(j);

			x_points[i][j] = function(theta, phi, 'x');
			y_points[i][j] = function(theta, phi, 'y');
			z_points[i][j] = function(theta, phi, 'z');
		}
	}
}

DataMesh3D::DataMesh3D(Mesh mesh) {
	this->mesh = mesh;

	std::vector<double> base_vector(mesh.N_phi, 0.);
	x_points.resize(mesh.N_theta, base_vector);
	y_points.resize(mesh.N_theta, base_vector);
	z_points.resize(mesh.N_theta, base_vector);
}

std::shared_ptr<DataMesh3D> DataMesh3D::get_copy() {
	auto copy = std::make_shared<DataMesh3D>(mesh);

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			copy->x_points[i][j] = x_points[i][j];
			copy->y_points[i][j] = y_points[i][j];
			copy->z_points[i][j] = z_points[i][j];
		}
	}

	return copy;
}

double DataMesh3D::rms() {
	double x_rms = get_rms(&x_points);
	double y_rms = get_rms(&y_points);
	double z_rms = get_rms(&z_points);

	double rms = (x_rms + y_rms + z_rms) / 3.;
	return rms;
}

std::shared_ptr<DataMesh> DataMesh3D::norm() {
	if (cached_norm != nullptr) {
		return cached_norm;
	}

	auto norm = std::make_shared<DataMesh>(mesh);

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			norm->points[i][j] = sqrt( squared(x_points[i][j]) + squared(y_points[i][j]) + squared(z_points[i][j]) );
		}
	}

	cached_norm = norm;

	return norm;
}

std::shared_ptr<DataMesh3D> DataMesh3D::partial_theta() {
	if (cached_partial_theta != nullptr) {
		return cached_partial_theta;
	}

	auto new_function = get_copy();

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			double df_x, df_y, df_z;

			if (i == 0) { // forward
				df_x = -3./2. * x_points[0][j] +2. * x_points[1][j] -1./2. * x_points[2][j];
				df_y = -3./2. * y_points[0][j] +2. * y_points[1][j] -1./2. * y_points[2][j];
				df_z = -3./2. * z_points[0][j] +2. * z_points[1][j] -1./2. * z_points[2][j];
			}
			else if (i == mesh.N_theta - 1) { // backward
				df_x = +3./2. * x_points[mesh.N_theta-1][j] -2. * x_points[mesh.N_theta-2][j] +1./2. * x_points[mesh.N_theta-3][j];
				df_y = +3./2. * y_points[mesh.N_theta-1][j] -2. * y_points[mesh.N_theta-2][j] +1./2. * y_points[mesh.N_theta-3][j];
				df_z = +3./2. * z_points[mesh.N_theta-1][j] -2. * z_points[mesh.N_theta-2][j] +1./2. * z_points[mesh.N_theta-3][j];
			}
			else { // centered
				df_x = -1./2. * x_points[i-1][j] +1./2. * x_points[i+1][j];
				df_y = -1./2. * y_points[i-1][j] +1./2. * y_points[i+1][j];
				df_z = -1./2. * z_points[i-1][j] +1./2. * z_points[i+1][j];
			}

			new_function->x_points[i][j] = df_x / mesh.delta_theta;
			new_function->y_points[i][j] = df_y / mesh.delta_theta;
			new_function->z_points[i][j] = df_z / mesh.delta_theta;
		}
	}

	cached_partial_theta = new_function;

	return new_function;
}

std::shared_ptr<DataMesh3D> DataMesh3D::partial_phi() {
	if (cached_partial_phi != nullptr) {
		return cached_partial_phi;
	}

	auto new_function = get_copy();

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			if (j == 0) {
				(*new_function).x_points[i][j] = (x_points[i][1] - x_points[i][mesh.N_phi - 1]) / (2 * mesh.delta_phi);
				(*new_function).y_points[i][j] = (y_points[i][1] - y_points[i][mesh.N_phi - 1]) / (2 * mesh.delta_phi);
				(*new_function).z_points[i][j] = (z_points[i][1] - z_points[i][mesh.N_phi - 1]) / (2 * mesh.delta_phi);
			}
			else if (j == mesh.N_phi - 1) {
				(*new_function).x_points[i][j] = (x_points[i][0] - x_points[i][mesh.N_phi - 2]) / (2 * mesh.delta_phi);
				(*new_function).y_points[i][j] = (y_points[i][0] - y_points[i][mesh.N_phi - 2]) / (2 * mesh.delta_phi);
				(*new_function).z_points[i][j] = (z_points[i][0] - z_points[i][mesh.N_phi - 2]) / (2 * mesh.delta_phi);
			}
			else {
				(*new_function).x_points[i][j] = (x_points[i][j + 1] - x_points[i][j - 1]) / (2 * mesh.delta_phi);
				(*new_function).y_points[i][j] = (y_points[i][j + 1] - y_points[i][j - 1]) / (2 * mesh.delta_phi);
				(*new_function).z_points[i][j] = (z_points[i][j + 1] - z_points[i][j - 1]) / (2 * mesh.delta_phi);
			}
		}
	}
 
	cached_partial_phi = new_function;

	return new_function;
}

std::shared_ptr<DataMesh3D> DataMesh3D::second_partial_theta() {
	if (cached_second_partial_theta != nullptr) {
		return cached_second_partial_theta;
	}

	auto result = std::make_shared<DataMesh3D>(mesh);

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			double d2f_x, d2f_y, d2f_z;

			if (i == 0) { // forward finite difference
				// 2nd order
				d2f_x = +2. * x_points[0][j] -5. * x_points[1][j] +4. * x_points[2][j] -1. * x_points[3][j];
				d2f_y = +2. * y_points[0][j] -5. * y_points[1][j] +4. * y_points[2][j] -1. * y_points[3][j];
				d2f_z = +2. * z_points[0][j] -5. * z_points[1][j] +4. * z_points[2][j] -1. * z_points[3][j];
			}
			else if (i == mesh.N_theta - 1) { // backward finite difference
				// 2nd order
				d2f_x = +2. * x_points[i][j] -5. * x_points[i-1][j] +4. * x_points[i-2][j] -1. * x_points[i-3][j];
				d2f_y = +2. * y_points[i][j] -5. * y_points[i-1][j] +4. * y_points[i-2][j] -1. * y_points[i-3][j];
				d2f_z = +2. * z_points[i][j] -5. * z_points[i-1][j] +4. * z_points[i-2][j] -1. * z_points[i-3][j];
			}
			else { // central finite difference
				// 2nd order
				d2f_x = +1. * x_points[i-1][j] -2. * x_points[i][j] +1. * x_points[i+1][j];
				d2f_y = +1. * y_points[i-1][j] -2. * y_points[i][j] +1. * y_points[i+1][j];
				d2f_z = +1. * z_points[i-1][j] -2. * z_points[i][j] +1. * z_points[i+1][j];
			}

			result->x_points[i][j] = d2f_x / squared(mesh.delta_theta);
			result->y_points[i][j] = d2f_y / squared(mesh.delta_theta);
			result->z_points[i][j] = d2f_z / squared(mesh.delta_theta);
		}
	}

	cached_second_partial_theta = result;

	return result;
}

std::shared_ptr<DataMesh3D> DataMesh3D::second_partial_phi() {
	if (cached_second_partial_phi != nullptr) {
		return cached_second_partial_phi;
	}

	auto result = std::make_shared<DataMesh3D>(mesh);

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			double d2f_x, d2f_y, d2f_z;

			// central finite difference
			// 2nd order
			if (j == 0) {
				d2f_x = +1. * x_points[i][mesh.N_phi-1] -2. * x_points[i][j] +1. * x_points[i][j+1];
				d2f_y = +1. * y_points[i][mesh.N_phi-1] -2. * y_points[i][j] +1. * y_points[i][j+1];
				d2f_z = +1. * z_points[i][mesh.N_phi-1] -2. * z_points[i][j] +1. * z_points[i][j+1];
			}
			else if (j == mesh.N_phi - 1) {
				d2f_x = +1. * x_points[i][j-1] -2. * x_points[i][j] +1. * x_points[i][0];
				d2f_y = +1. * y_points[i][j-1] -2. * y_points[i][j] +1. * y_points[i][0];
				d2f_z = +1. * z_points[i][j-1] -2. * z_points[i][j] +1. * z_points[i][0];
			}
			else {
				d2f_x = +1. * x_points[i][j-1] -2. * x_points[i][j] +1. * x_points[i][j+1];
				d2f_y = +1. * y_points[i][j-1] -2. * y_points[i][j] +1. * y_points[i][j+1];
				d2f_z = +1. * z_points[i][j-1] -2. * z_points[i][j] +1. * z_points[i][j+1];
			}

			result->x_points[i][j] = d2f_x / squared(mesh.delta_phi);
			result->y_points[i][j] = d2f_y / squared(mesh.delta_phi);
			result->z_points[i][j] = d2f_z / squared(mesh.delta_phi);
		}
	}
 
	cached_second_partial_phi = result;

	return result;
}

std::shared_ptr<DataMesh3D> DataMesh3D::operator+(double constant) {
	auto new_data = get_copy();

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			(*new_data).x_points[i][j] += constant;
			(*new_data).y_points[i][j] += constant;
			(*new_data).z_points[i][j] += constant;
		}
	}

	return new_data;
}

std::shared_ptr<DataMesh3D> DataMesh3D::operator+(std::shared_ptr<DataMesh3D> other) {
	auto new_data = get_copy();

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			(*new_data).x_points[i][j] += other->x_points[i][j];
			(*new_data).y_points[i][j] += other->y_points[i][j];
			(*new_data).z_points[i][j] += other->z_points[i][j];
		}
	}

	return new_data;
}

std::shared_ptr<DataMesh3D> DataMesh3D::operator+(double (*function)(double theta, double phi)) {
	auto new_data = get_copy();

	for (int i = 0; i < mesh.N_theta; i++) {
		double theta = mesh.theta(i);

		for (int j = 0; j < mesh.N_phi; j++) {
			double phi = mesh.phi(j);

			new_data->x_points[i][j] += function(theta, phi);
			new_data->y_points[i][j] += function(theta, phi);
			new_data->z_points[i][j] += function(theta, phi);
		}
	}

	return new_data;
}

std::shared_ptr<DataMesh3D> DataMesh3D::operator+(double (*function)(double theta, double phi, char coord)) {
	auto new_data = get_copy();

	for (int i = 0; i < mesh.N_theta; i++) {
		double theta = mesh.theta(i);

		for (int j = 0; j < mesh.N_phi; j++) {
			double phi = mesh.phi(j);

			new_data->x_points[i][j] += function(theta, phi, 'x');
			new_data->y_points[i][j] += function(theta, phi, 'y');
			new_data->z_points[i][j] += function(theta, phi, 'z');
		}
	}

	return new_data;
}

std::shared_ptr<DataMesh3D> DataMesh3D::operator*(double constant) {
	auto new_data = get_copy();

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			(*new_data).x_points[i][j] *= constant;
			(*new_data).y_points[i][j] *= constant;
			(*new_data).z_points[i][j] *= constant;
		}
	}

	return new_data;
}

std::shared_ptr<DataMesh3D> DataMesh3D::operator*(std::shared_ptr<DataMesh3D> other) {
	auto new_data = get_copy();

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			(*new_data).x_points[i][j] *= other->x_points[i][j];
			(*new_data).y_points[i][j] *= other->y_points[i][j];
			(*new_data).z_points[i][j] *= other->z_points[i][j];
		}
	}

	return new_data;
}

std::shared_ptr<DataMesh3D> DataMesh3D::operator*(std::shared_ptr<DataMesh> scalar) {
	auto new_data = get_copy();

	for (int i = 0; i < mesh.N_theta; i++) {
		for (int j = 0; j < mesh.N_phi; j++) {
			(*new_data).x_points[i][j] *= scalar->points[i][j];
			(*new_data).y_points[i][j] *= scalar->points[i][j];
			(*new_data).z_points[i][j] *= scalar->points[i][j];
		}
	}

	return new_data;
}

std::shared_ptr<DataMesh3D> DataMesh3D::operator*(double (*function)(double theta, double phi)) {
	auto new_data = get_copy();

	for (int i = 0; i < mesh.N_theta; i++) {
		double theta = mesh.theta(i);

		for (int j = 0; j < mesh.N_phi; j++) {
			double phi = mesh.phi(j);
			
			(*new_data).x_points[i][j] *= function(theta, phi);
			(*new_data).y_points[i][j] *= function(theta, phi);
			(*new_data).z_points[i][j] *= function(theta, phi);
		}
	}

	return new_data;
}

std::shared_ptr<DataMesh3D> DataMesh3D::operator*(double (*function)(double theta, double phi, char coord)) {
	auto new_data = get_copy();

	for (int i = 0; i < mesh.N_theta; i++) {
		double theta = mesh.theta(i);

		for (int j = 0; j < mesh.N_phi; j++) {
			double phi = mesh.phi(j);
			
			(*new_data).x_points[i][j] *= function(theta, phi, 'x');
			(*new_data).y_points[i][j] *= function(theta, phi, 'y');
			(*new_data).z_points[i][j] *= function(theta, phi, 'z');
		}
	}

	return new_data;
}

std::shared_ptr<DataMesh> DataMesh3D::dot(std::shared_ptr<DataMesh3D> other) {
	std::shared_ptr<DataMesh> dot_product = std::make_shared<DataMesh>(this->mesh);	

	for (int i = 0; i < this->mesh.N_theta; i++) {
		for (int j = 0; j < this->mesh.N_phi; j++) {
			dot_product->points[i][j] =
				this->x_points[i][j] * other->x_points[i][j] +
				this->y_points[i][j] * other->y_points[i][j] +
				this->z_points[i][j] * other->z_points[i][j];
		}
	}
	
	return dot_product;
}
