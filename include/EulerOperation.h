//
// Created by root on 23-11-1.
//

#ifndef EULER_SWEEP_EULEROPERATION_H
#define EULER_SWEEP_EULEROPERATION_H

#include "Brep.h"
#include <utility>

namespace EulerOp{
    std::pair<std::shared_ptr<Solid>, std::shared_ptr<Vertex>> mvsf(const Vec3& p);

    std::shared_ptr<HalfEdge> mev(const Vec3& p, std::shared_ptr<Vertex> v1, std::shared_ptr<Loop> lp);

    std::shared_ptr<Face> mef(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2, std::shared_ptr<Loop>);

    std::shared_ptr<Loop> kemr(std::shared_ptr<Edge> eg, std::shared_ptr<Loop> lp);

    void kfmrh(std::shared_ptr<Face> innerFace, std::shared_ptr<Face> outerFace);
};

#endif //EULER_SWEEP_EULEROPERATION_H
