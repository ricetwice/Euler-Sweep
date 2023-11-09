//
// Created by root on 23-11-2.
//

#include "Brep.h"
#include <iostream>
#include <unordered_map>

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

void Face::printLoops() const {
    int lp_cnt = 0;
    for (auto lp = m_floop; lp != nullptr; lp = lp->next(), lp_cnt++)
    {
        std::cout << "lp" << lp_cnt << ": ";
        lp->printLoop();
    }
    std::cout << std::endl;
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

void Solid::printFaces() const {
    int f_cnt = 0;
    for (auto f = m_sface; f != nullptr; f = f->next(), f_cnt++)
    {
        std::cout << "F" << f_cnt << ":" << std::endl;
        f->printLoops();
    }
    std::cout << std::endl;
}

std::pair<std::vector<Vec3>, std::vector<std::array<int, 2>>> Solid::getCurveFrame() const {
    std::vector<Vec3> nodes;
    std::vector<std::array<int, 2>> edges;
    std::unordered_map<std::shared_ptr<Vertex>, int> vertexRecorder;
    int v_cnt = 0;
    for (auto v = m_svertex; v != nullptr; v = v->next(), v_cnt++)
    {
        nodes.push_back(v->pos());
        vertexRecorder[v] = v_cnt;
    }
    for (auto e = m_sedge; e != nullptr; e = e->next())
    {
        edges.push_back({vertexRecorder[e->halfEdge(0)->vertex()], vertexRecorder[e->halfEdge(1)->vertex()]});
    }
    return {nodes, edges};
}

void Loop::printLoop() const {
    if (m_ledge)
    {
        std::cout << "( " << m_ledge->vertex()->pos() << " ), ";
        auto he_fast = m_ledge->next()->next();
        for (auto he = m_ledge->next(); he != m_ledge; he = he->next() , he_fast = he_fast->next()->next())
        {
            if (he == he_fast)
            {
                std::cerr << "The loop is not closed!" << std::endl;
                exit(-1);
            }
            std::cout <<  "( " << he->vertex()->pos() << " ), ";
        }
        std::cout << std::endl;
    }
}
