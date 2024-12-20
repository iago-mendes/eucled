// Distributed under the MIT License.
// See LICENSE.txt for details.

#pragma once

#define sqr(n) ((n)*(n))
#define to_the_4th(n) ((n)*(n)*(n)*(n))

#define sin_theta [](double theta, [[maybe_unused]] double phi) {return sin(theta);}
#define sqr_sin_theta [](double theta, [[maybe_unused]] double phi) {return sqr(sin(theta));}
#define sqrt_sin_theta [](double theta, [[maybe_unused]] double phi) {return sqrt(sin(theta));}
#define inv_sin_theta [](double theta, [[maybe_unused]] double phi) {return 1./sin(theta);}
#define inv_sqr_sin_theta [](double theta, [[maybe_unused]] double phi) {return 1./sqr(sin(theta));}
#define inv_sin_theta_4 [](double theta, [[maybe_unused]] double phi) {return 1./to_the_4th(sin(theta));}
#define inv_tan_theta [](double theta, [[maybe_unused]] double phi) {return 1/tan(theta);}
