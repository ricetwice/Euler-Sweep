//
// Created by root on 23-11-1.
//

#include "EulerOperation.h"



std::pair<std::shared_ptr<Solid> , std::shared_ptr<Vertex>> EulerOp::mvsf(const Vec3& p)
{
    auto f = std::make_shared<Face>();
    auto s = std::make_shared<Solid>();
    auto lp = std::make_shared<Loop>();
    auto v = std::make_shared<Vertex>(p);
    v->solid() = s;
    s->addFace(f);
    s->addVertex(v);
    f->solid() = s;
    f->loop() = lp;
    return {s, v};
}

std::shared_ptr<HalfEdge> EulerOp::mev(const Vec3& p, std::shared_ptr<Vertex> v1, std::shared_ptr<Loop> lp){
    auto s = v1->solid();
    auto v2 = std::make_shared<Vertex>(p, s);
    auto he1 = std::make_shared<HalfEdge>(v1),
            he2 = std::make_shared<HalfEdge>(v2);
    auto eg = std::make_shared<Edge>(he1, he2);
    s->addVertex(v2);
    s->addEdge(eg);
    he1->next() = he2;
    he2->prev() = he1;
    he1->loop() = he2->loop() = lp;
    he1->edge() = he2->edge() = eg;
    if (lp->halfEdge() == nullptr)
    {
        he2->next() = he1;
        he1->prev() = he2;
        lp->halfEdge() = he1;
        lp->face() = v1->solid()->face();
    }else
    {
        auto he = lp->halfEdge();
        for(; he->next()->vertex() != v1; he = he->next());
        he2->next() = he->next();
        he->next()->prev() = he2;
        he->next() = he1;
        he1->prev() = he;
    }

    return he1;
}

std::shared_ptr<Face> EulerOp::mef(std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2, std::shared_ptr<Loop> lp1){
    auto he1 = std::make_shared<HalfEdge>(v1),
            he2 = std::make_shared<HalfEdge>(v2);
    auto eg = std::make_shared<Edge>(he1, he2);
    auto lp2 = std::make_shared<Loop>();
    auto f = std::make_shared<Face>();
    auto s = v1->solid();
    s->addFace(f);
    s->addEdge(eg);
    f->loop() = lp2;
    lp2->face() = f;
    f->solid() = s;
    auto he1_next = lp1->halfEdge(), he2_next = lp1->halfEdge();
    for (; he1_next->vertex() != v2 ; he1_next = he1_next->next());
    for (; he2_next->vertex() != v1 ; he2_next = he2_next->next());
    auto he1_prev = he2_next->prev(), he2_prev = he1_next->prev();
    //connect loop containing he1
    he1->next() = he1_next;
    he1_next->prev() = he1;
    he1->prev() = he1_prev;
    he1_prev->next() = he1;
    //connect loop containing he2
    he2->next() = he2_next;
    he1_next->prev() = he2;
    he2->prev() = he2_prev;
    he2_prev->next() = he2;
    lp2->halfEdge() = he2;
    lp1->halfEdge() = he1;

    return f;
}

std::shared_ptr<Loop> EulerOp::kemr(std::shared_ptr<Edge> eg, std::shared_ptr<Loop> lp)
{
    auto he1 = eg->halfEdge(0), he2 = eg->halfEdge(1);
    he1->prev()->next() = he2->next();
    he2->next()->prev() = he1->prev();
    lp->halfEdge() = he2->next();
    auto lp_new = std::make_shared<Loop>();
    if (he1->next() != he2)
    {
        lp_new->halfEdge() = he1->next();
        auto he = lp_new->halfEdge();
        for (; he->next() != he2; he = he->next());
        he->next() = lp_new->halfEdge();
        lp_new->halfEdge()->prev() = he;
        lp->face()->addLoop(lp_new);
    }

    //delete edge
    lp->face()->solid()->deleteEdge(eg);

    return lp_new;
}

void EulerOp::kfmrh(std::shared_ptr<Face> innerFace, std::shared_ptr<Face> outerFace)
{
    outerFace->addLoop(innerFace->loop());
    innerFace->solid()->deleteFace(innerFace);
}




