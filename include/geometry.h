//
// Created by root on 23-11-2.
//

#ifndef EULER_SWEEP_GEOMETRY_H
#define EULER_SWEEP_GEOMETRY_H

#include <Eigen/Dense>

using Vec3 =  Eigen::Matrix<float, 1, 3>;
using Vec2 =  Eigen::Matrix<float, 1, 2>;

struct Axis{
    Vec3 p, X, Y;
    void print() const;
};

Vec2 project2D(const Vec3& v, const Axis& axis);

Vec3 recover3D(const Vec2& u, const Axis& axis);

#endif //EULER_SWEEP_GEOMETRY_H
