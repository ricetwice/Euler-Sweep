//
// Created by root on 23-11-5.
//
#include <iostream>
#include "utils.h"
#include "EulerOperation.h"


std::shared_ptr<Solid> createSquare(const Vec3& center, float l) {
    auto [S, V1] = EulerOp::mvsf(center);
    auto& lp = S->face()->loop();
    auto he1 = EulerOp::mev(center + Vec3{l,0,0}, V1, lp);
    auto V2 = he1->sibling()->vertex();
    auto he2 = EulerOp::mev(center + Vec3{l,l,0}, he1->sibling()->vertex(), lp);
    auto he3 = EulerOp::mev(center + Vec3 {0,l,0}, he2->sibling()->vertex(), lp);
    auto f = EulerOp::mef(V1, he3->sibling()->vertex(), lp);
    return S;
}

std::shared_ptr<Solid> createDoubleSquare(const Vec3 & center,float L, float l) {
    auto [S, V1] = EulerOp::mvsf(center);
    auto& lp = S->face()->loop();
    auto he1 = EulerOp::mev(center + Vec3{L,0,0}, V1, lp);
    auto V2 = he1->sibling()->vertex();
    auto he2 = EulerOp::mev(center + Vec3 {L,L,0}, he1->sibling()->vertex(), lp);
    auto he3 = EulerOp::mev(center + Vec3 {0,L,0}, he2->sibling()->vertex(), lp);
    auto f = EulerOp::mef(V1, he3->sibling()->vertex(), lp);
    float a = (L-l)/2, b = (L+l)/2;
    auto he4 = EulerOp::mev(center+Vec3 {a,a,0}, V1, f->loop());
    auto conEdge = S->edge();
    auto he5 = EulerOp::mev(center + Vec3 {b,a,0}, he4->sibling()->vertex(), f->loop());
    auto he6 = EulerOp::mev(center + Vec3 {b,b,0}, he5->sibling()->vertex(), f->loop());
    auto he7 = EulerOp::mev(center + Vec3 {a,b,0}, he6->sibling()->vertex(), f->loop());
    auto f2 = EulerOp::mef(he5->vertex(), he7->sibling()->vertex(), f->loop());
    EulerOp::kemr(conEdge, f->loop());
    EulerOp::kfmrh(f2, lp->face());
    return S;
}
