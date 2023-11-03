//
// Created by root on 23-11-2.
//

#include "Brep.h"

void Face::addLoop(std::shared_ptr<Loop> lp) {
    if (m_floop == nullptr)
    {
        m_floop = lp;
        return;
    }
    auto curlp = m_floop;
    for (; curlp->next() != nullptr; curlp = curlp->next());
    curlp->next() = lp;
    lp->prev() = curlp;
}

void Solid::addVertex(std::shared_ptr<Vertex> v) {
    if (m_svertex)
    {
        m_svertex->prev() = v;
        v->next() = m_svertex;
    }
    m_svertex = v;
}

void Solid::addEdge(std::shared_ptr<Edge> eg) {
    if (m_sedge)
    {
        m_sedge->prev() = eg;
        eg->next() = m_sedge;
    }
    m_sedge = eg;

}

void Solid::addFace(std::shared_ptr<Face> f) {
    if (m_sface) {
        m_sface->prev() = f;
        f->next() = m_sface;
    }
    m_sface = f;
}

bool Solid::deleteEdge(std::shared_ptr<Edge> eg) {
    auto curE = m_sedge;
    for (; curE && curE != eg; curE = curE->next());
    if (curE)
    {
        curE->prev()->next() = curE->next();
        curE->next()->prev() = curE->next();
        return true;
    }
    return false;
}

bool Solid::deleteFace(std::shared_ptr<Face> f) {
    auto curF = m_sface;
    for (; curF && curF != f; curF = curF->next());
    if (curF)
    {
        curF->prev()->next() = curF->next();
        curF->next()->prev() = curF->prev();
        return true;
    }
    return false;
}