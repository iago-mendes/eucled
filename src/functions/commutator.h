#ifndef COMMUTATOR_H
#define COMMUTATOR_H

#include "../classes/grid.h"

Grid3DFunction *get_commutator(Grid3DFunction *e_theta, Grid3DFunction *e_phi);
double get_commutator_rms(Grid3DFunction *e_theta, Grid3DFunction *e_phi);

#endif
