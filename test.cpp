#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <numeric>
#include <random>
#include <vector>

#include "quatcompress.h"

// quaternions, xyzw convention.
using quat = std::array<float, 4>;

quat operator*(quat const &q, quat const &p)
{
	float x =  q[3]*p[0] + q[2]*p[1] - q[1]*p[2] + q[0]*p[3];
	float y = -q[2]*p[0] + q[3]*p[1] + q[0]*p[2] + q[1]*p[3];
	float z =  q[1]*p[0] - q[0]*p[1] + q[3]*p[2] + q[2]*p[3];
	float w = -q[0]*p[0] - q[1]*p[1] - q[2]*p[2] + q[3]*p[3];
	return {x, y, z, w};
}

quat qnorm(quat const &q)
{
	float n = std::sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
	return {q[0] / n, q[1] / n, q[2] / n, q[3] / n};
}

int main()
{
	std::default_random_engine g{0};
	std::normal_distribution<float> d;

	size_t const TRIALS = 10 * 1000 * 1000;
	std::vector<float> angles(TRIALS);

	for (size_t i = 0; i < TRIALS; ++i) {
		// sample uniformly from S^4.
		quat q = qnorm({d(g), d(g), d(g), d(g)});
		quat qroundtrip;
		quatdecompress(quatcompress(q.data()), qroundtrip.data());

		qroundtrip[3] *= -1; // invert
		quat diff = qnorm(q * qroundtrip);
		float angle = 2 * std::acos(diff[3]); // get angle of axis-angle form.

		if (angle > M_PI) {
			angle -= 2 * M_PI;
		}
		angles[i] = (180 / M_PI) * std::abs(angle);
	}

	float mean = std::reduce(angles.begin(), angles.end()) / TRIALS;
	float max = *std::max_element(angles.begin(), angles.end());
	printf("mean: %f\n", mean);
	printf("max: %f\n", max);
}
