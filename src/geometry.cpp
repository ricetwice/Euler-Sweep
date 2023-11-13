//
// Created by root on 23-11-10.
//

#include "geometry.h"
#include <iostream>

Vec2 project2D(const Vec3& v, const Axis& axis)
{
    return Vec2 {axis.X.dot(v-axis.p), axis.Y.dot(v-axis.p)};
}

Vec3 recover3D(const Vec2& u, const Axis& axis)
{
    return u[0] * axis.X + u[1] * axis.Y + axis.p;
}

void Axis::print() const {
    std::cout << "p : ( " << p  <<  " ) , X : ( " << X <<  " ) , Y : ( " <<  Y <<  " )" << std::endl;
}
