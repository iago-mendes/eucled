#ifndef COMMUTATOR_H
#define COMMUTATOR_H

#include "../classes/grid.h"

shared_ptr<Grid3DFunction> get_commutator(shared_ptr<Grid3DFunction> e_theta, shared_ptr<Grid3DFunction> e_phi);
double get_commutator_rms(shared_ptr<Grid3DFunction> e_theta, shared_ptr<Grid3DFunction> e_phi);
shared_ptr<GridFunction> get_commutator_norm(shared_ptr<Grid3DFunction> e_theta, shared_ptr<Grid3DFunction> e_phi);

#endif
