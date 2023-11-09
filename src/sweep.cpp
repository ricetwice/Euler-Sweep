//
// Created by root on 23-11-7.
//

#include "sweep.h"

void sweep(std::shared_ptr<Face> F, const Vec3 &V, float d) {
    for (auto L = F->loop(); L != nullptr; L = L->next())
    {
        std::vector<std::shared_ptr<Vertex>> loopVertices;
        for (auto he = L->halfEdge()->next(); he != L->halfEdge(); he = he->next())
        {
            loopVertices.push_back(he->vertex());
        }
        auto firstV = L->halfEdge()->vertex();
        auto firstUp = EulerOp::mev(firstV->pos()+d*V, firstV, L)->tipVertex();
        L->printLoop();
        auto prevUp = firstUp;
        for (auto nextV : loopVertices)
        {
            auto up = EulerOp::mev(nextV->pos() + d*V,nextV, L)->tipVertex();
//            L->printLoop();
            EulerOp::mef(prevUp, up, L);
//            L->printLoop();
            prevUp = up;
        }
        EulerOp::mef(prevUp,firstUp , L);
    }
}
