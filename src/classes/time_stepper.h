#ifndef TIME_STEPPER_H
#define TIME_STEPPER_H

#include <bits/stdc++.h>
#include "grid.h"

struct Iteration {
	shared_ptr<Grid3DFunction> solution1;
	shared_ptr<Grid3DFunction> solution2;
	double residual;
};

class TimeStepper {
	public:
		TimeStepper(double step, double cache_number);

		double get_step();

		/*
		 * Uses the iteration values to determine how the step should be updated.
		 * Returns the iteration values to be used.
		 */
		shared_ptr<Iteration> update_step(
			shared_ptr<Grid3DFunction> solution1,
			shared_ptr<Grid3DFunction> solution2,
			double residual
		);

		shared_ptr<Iteration> update_step(
			shared_ptr<Grid3DFunction> solution,
			double residual
		);

	private:
		double step;
		double cache_number;

		// Keeps last iterations.
		queue<shared_ptr<Iteration>> iterations;
};

#endif
