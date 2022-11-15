#include "time_stepper.h"
using namespace std;

TimeStepper::TimeStepper(double step, double cache_number) {
	this->step = step;
	this->cache_number = cache_number;
}

double TimeStepper::get_step() {
	return step;
}

shared_ptr<Iteration> TimeStepper::update_step(
	shared_ptr<Grid3DFunction> solution1,
	shared_ptr<Grid3DFunction> solution2,
	double residual
) {
	shared_ptr<Iteration> new_iteration = make_shared<Iteration>();
	new_iteration->solution1 = solution1;
	new_iteration->solution2 = solution2;
	new_iteration->residual = residual;

	iterations.push(new_iteration);

	// If not enough iterations
	if (iterations.size() <= cache_number) {
		return new_iteration; // Keep new iteration
	}

	shared_ptr<Iteration> old_iteration = iterations.front();
	iterations.pop();

	// If new is better than old
	if (new_iteration->residual < old_iteration->residual) {
		step *= 1.1; // Increase step by 10%
		return new_iteration; // Keep new iteration
	}

	// If old is better than new
	step *= 0.5; // Decrease step by 50%
	iterations = {}; // Clear iterations queue
	iterations.push(old_iteration); // Add returned iteration
	return old_iteration; // Return to old iteration
}

shared_ptr<Iteration> TimeStepper::update_step(
	shared_ptr<Grid3DFunction> solution,
	double residual
) {
	return update_step(solution, nullptr, residual);
}
