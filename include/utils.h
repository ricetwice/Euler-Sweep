//
// Created by root on 23-11-5.
//

#ifndef EULER_SWEEP_UTILS_H
#define EULER_SWEEP_UTILS_H

#include <utility>
#include <vector>
#include "Brep.h"

std::shared_ptr<Solid> createSquare(const Vec3& center,float l);
std::shared_ptr<Solid> createDoubleSquare(const Vec3& center,float L, float l);

#endif //EULER_SWEEP_UTILS_H
