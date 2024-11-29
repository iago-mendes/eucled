// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#include "./DataMesh.hpp"
#include "./Mesh.hpp"

#include <memory>
#include <vector>

/**
 * @brief 3D vector function data on a theta-phi mesh
 */
class DataMesh3D {
	public:
		Mesh mesh;
		std::vector<std::vector<double>> x_points;
		std::vector<std::vector<double>> y_points;
		std::vector<std::vector<double>> z_points;

		DataMesh3D(Mesh mesh, double (*function)(int i, int j, char coord));
		DataMesh3D(Mesh mesh, double (*function)(double theta, double phi, char coord));
		DataMesh3D(Mesh mesh);
		DataMesh3D() = default;

		std::shared_ptr<DataMesh3D> get_copy();

		double rms();
		std::shared_ptr<DataMesh> norm();

		std::shared_ptr<DataMesh3D> partial_theta();
		std::shared_ptr<DataMesh3D> partial_phi();
		std::shared_ptr<DataMesh3D> second_partial_theta();
		std::shared_ptr<DataMesh3D> second_partial_phi();

		std::shared_ptr<DataMesh3D> operator+(double constant);
		std::shared_ptr<DataMesh3D> operator+(std::shared_ptr<DataMesh3D> other);
		std::shared_ptr<DataMesh3D> operator+(double (*function)(double theta, double phi));
		std::shared_ptr<DataMesh3D> operator+(double (*function)(double theta, double phi, char coord));

		std::shared_ptr<DataMesh3D> operator-(std::shared_ptr<DataMesh3D> other);

		std::shared_ptr<DataMesh3D> operator*(double constant);
		std::shared_ptr<DataMesh3D> operator*(std::shared_ptr<DataMesh3D> other);
		std::shared_ptr<DataMesh3D> operator*(std::shared_ptr<DataMesh> scalar);
		std::shared_ptr<DataMesh3D> operator*(double (*function)(double theta, double phi));
		std::shared_ptr<DataMesh3D> operator*(double (*function)(double theta, double phi, char coord));

		std::shared_ptr<DataMesh> dot(std::shared_ptr<DataMesh3D> other);

	private:
		std::shared_ptr<DataMesh3D> cached_partial_theta = nullptr;
		std::shared_ptr<DataMesh3D> cached_partial_phi = nullptr;
		std::shared_ptr<DataMesh3D> cached_second_partial_theta = nullptr;
		std::shared_ptr<DataMesh3D> cached_second_partial_phi = nullptr;
		std::shared_ptr<DataMesh> cached_norm = nullptr;
};

// Operator overloads for shared_ptr<DataMesh3D>

std::shared_ptr<DataMesh3D> operator+(std::shared_ptr<DataMesh3D> ptr, double constant);
std::shared_ptr<DataMesh3D> operator+(std::shared_ptr<DataMesh3D> ptr, std::shared_ptr<DataMesh3D> other);
std::shared_ptr<DataMesh3D> operator+(std::shared_ptr<DataMesh3D> ptr, double (*function)(double theta, double phi));
std::shared_ptr<DataMesh3D> operator+(std::shared_ptr<DataMesh3D> ptr, double (*function)(double theta, double phi, char coord));

std::shared_ptr<DataMesh3D> operator-(std::shared_ptr<DataMesh3D> ptr, std::shared_ptr<DataMesh3D> other);

std::shared_ptr<DataMesh3D> operator*(std::shared_ptr<DataMesh3D> ptr, double constant);
std::shared_ptr<DataMesh3D> operator*(std::shared_ptr<DataMesh3D> ptr, std::shared_ptr<DataMesh3D> other);
std::shared_ptr<DataMesh3D> operator*(std::shared_ptr<DataMesh3D> ptr, std::shared_ptr<DataMesh> scalar);
std::shared_ptr<DataMesh3D> operator*(std::shared_ptr<DataMesh3D> ptr, double (*function)(double theta, double phi));
std::shared_ptr<DataMesh3D> operator*(std::shared_ptr<DataMesh3D> ptr, double (*function)(double theta, double phi, char coord));
