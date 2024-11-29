#pragma once

#include "./Mesh.hpp"

#include <memory>
#include <vector>

/**
 * @brief Scalar function data on a theta-phi mesh
 */
class DataMesh {
	public:
		Mesh mesh;
		std::vector<std::vector<double>> points;

		DataMesh(Mesh mesh, double (*function)(int i, int j));
		DataMesh(Mesh mesh, std::vector<std::vector<double>> *points);
		DataMesh(Mesh mesh);

		std::shared_ptr<DataMesh> get_copy();

		double rms();

		std::shared_ptr<DataMesh> partial_theta();
		std::shared_ptr<DataMesh> partial_phi();

		std::shared_ptr<DataMesh> operator+(double constant);
		std::shared_ptr<DataMesh> operator+(std::shared_ptr<DataMesh> other);
		std::shared_ptr<DataMesh> operator+(double (*function)(double theta, double phi));

		std::shared_ptr<DataMesh> operator*(double constant);
		std::shared_ptr<DataMesh> operator*(std::shared_ptr<DataMesh> other);
		std::shared_ptr<DataMesh> operator*(double (*function)(double theta, double phi));

	private:
		std::shared_ptr<DataMesh> cached_partial_theta;
		std::shared_ptr<DataMesh> cached_partial_phi;
};

// Operator overloads for shared_ptr<DataMesh>

std::shared_ptr<DataMesh> operator+(std::shared_ptr<DataMesh> ptr, double constant);
std::shared_ptr<DataMesh> operator+(std::shared_ptr<DataMesh> ptr, std::shared_ptr<DataMesh> other);
std::shared_ptr<DataMesh> operator+(std::shared_ptr<DataMesh> ptr, double (*function)(double theta, double phi));

std::shared_ptr<DataMesh> operator*(std::shared_ptr<DataMesh> ptr, double constant);
std::shared_ptr<DataMesh> operator*(std::shared_ptr<DataMesh> ptr, std::shared_ptr<DataMesh> other);
std::shared_ptr<DataMesh> operator*(std::shared_ptr<DataMesh> ptr, double (*function)(double theta, double phi));
