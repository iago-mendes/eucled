#include "XPeanutMetric.hpp"

XPeanutMetric::XPeanutMetric() {
	parameters.push_back("Radius");
}

XPeanutMetric::XPeanutMetric(double s0) {
	this->s0 = s0;
}

void XPeanutMetric::set_parameter(std::string parameter, std::string value) {
	if (parameter == "Radius") {
		this->s0 = stod(value);
	}
}

double XPeanutMetric::g_theta_theta(double theta, double phi) {
	return ((pow(s0,4)-4*pow(cos(phi),2)*pow(sin(theta),2)+4*pow(cos(phi),4)*pow(sin(theta),2))*(-1+2*pow(cos(phi),2)*pow(sin(theta),2)+sqrt(pow(s0,4)-4*pow(cos(phi),2)*pow(sin(theta),2)+4*pow(cos(phi),4)*pow(sin(theta),4))))/(pow(s0,4)-4*pow(cos(phi),2)*pow(sin(theta),2)+4*pow(cos(phi),4)*pow(sin(theta),4));
}

double XPeanutMetric::g_theta_phi(double theta, double phi) {
	return (-2*pow(cos(phi),3)*sin(phi)*pow(sin(theta),2)*(-1+2*pow(cos(phi),2)*pow(sin(theta),2)+sqrt(pow(s0,4)-4*pow(cos(phi),2)*pow(sin(theta),2)+4*pow(cos(phi),4)*pow(sin(theta),4)))*sin(2*theta))/(pow(s0,4)-4*pow(cos(phi),2)*pow(sin(theta),2)+4*pow(cos(phi),4)*pow(sin(theta),4));
}

double XPeanutMetric::g_phi_phi(double theta, double phi) {
	return (pow(sin(theta),2)*(pow(s0,4)-4*pow(cos(phi),2)*pow(sin(theta),2)+pow(sin(2*phi),2)*pow(sin(theta),2)+4*pow(cos(phi),4)*pow(sin(theta),4))*(-1+2*pow(cos(phi),2)*pow(sin(theta),2)+sqrt(pow(s0,4)-4*pow(cos(phi),2)*pow(sin(theta),2)+4*pow(cos(phi),4)*pow(sin(theta),4))))/(pow(s0,4)-4*pow(cos(phi),2)*pow(sin(theta),2)+4*pow(cos(phi),4)*pow(sin(theta),4));
}
