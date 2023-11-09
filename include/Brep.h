//
// Created by root on 23-10-29.
//

#ifndef EULER_SWEEP_BREP_H
#define EULER_SWEEP_BREP_H

#include "geometry.h"

#include <list>
#include <memory>
#include <utility>
#include <vector>

class Vertex;
class HalfEdge;
class Edge;
class Loop;
class Face;
class Solid;

class Vertex{
public:
    Vertex() = default;
    Vertex(Vec3 p) : m_pos(std::move(p)) {}
    Vertex(Vec3 p, std::shared_ptr<Solid> s): m_pos(std::move(p)), m_solid(std::move(s)){}
    std::shared_ptr<Vertex>& next()  {return m_next;}
    std::shared_ptr<Vertex>& prev()  {return m_prev;}
//    std::shared_ptr<HalfEdge>& halfEdge()  {return m_he;}
    std::shared_ptr<Solid>& solid() {return m_solid;}

    [[nodiscard]] Vec3 pos() const {return m_pos;}

private:
    std::shared_ptr<Vertex> m_next, m_prev;
//    std::shared_ptr<HalfEdge> m_he;
    std::shared_ptr<Solid> m_solid;
    // geometry
    Vec3 m_pos;
};

class HalfEdge{
public:
    HalfEdge() = default;
    explicit HalfEdge(const std::shared_ptr<Vertex>& v) : m_vertex(v) {}
    std::shared_ptr<Vertex>& vertex()  {return m_vertex;}
    std::shared_ptr<HalfEdge>& next()  {return m_next;}
    std::shared_ptr<HalfEdge>& prev()  {return m_prev;}
    std::shared_ptr<Edge>& edge()  {return m_edge;}
    std::shared_ptr<Loop>& loop()  {return m_wloop;}

    std::shared_ptr<HalfEdge>& sibling() {return m_sibling;}
    std::shared_ptr<Vertex> tipVertex() const {return m_sibling->vertex();}
private:
    std::shared_ptr<Vertex> m_vertex;
    std::shared_ptr<HalfEdge> m_prev, m_next, m_sibling;
    std::shared_ptr<Edge> m_edge;
    std::shared_ptr<Loop> m_wloop;
};

class Edge{
public:
    Edge() = default;
    Edge(std::shared_ptr<HalfEdge> he1, std::shared_ptr<HalfEdge> he2){
        m_he[0] = std::move(he1);
        m_he[1] = std::move(he2);
        m_he[0]->sibling() = m_he[1];
        m_he[1]->sibling() = m_he[0];
    }
    std::shared_ptr<HalfEdge>& halfEdge(int idx) {return m_he[idx];}
    std::shared_ptr<Edge>& prev() {return m_prev;}
    std::shared_ptr<Edge>& next() {return m_next;}

private:
    std::shared_ptr<HalfEdge> m_he[2];
    std::shared_ptr<Edge> m_prev, m_next;
};

class Loop{
public:
    Loop() = default;
    std::shared_ptr<HalfEdge>& halfEdge() {return m_ledge;}
    std::shared_ptr<Loop>& prev() {return m_prev;}
    std::shared_ptr<Loop>& next() {return m_next;}
    std::shared_ptr<Face>& face() {return m_lface;}

    void printLoop() const;
private:
    std::shared_ptr<HalfEdge> m_ledge;
    std::shared_ptr<Loop> m_prev, m_next;
    std::shared_ptr<Face> m_lface;
};

class Face{
public:
    Face() = default;
    std::shared_ptr<Loop>& loop() {return  m_floop;}
    std::shared_ptr<Face>& next() {return m_next;}
    std::shared_ptr<Face>& prev() {return m_prev;}
    std::shared_ptr<Solid>& solid() {return m_fsolid;}

    void addLoop(std::shared_ptr<Loop> lp);
    void printLoops() const;
private:
    std::shared_ptr<Loop> m_floop;
    std::shared_ptr<Face> m_prev, m_next;
    std::shared_ptr<Solid> m_fsolid;
};

class Solid{
public:
    Solid() = default;
    std::shared_ptr<Vertex>& vertex() {return m_svertex;}
    std::shared_ptr<Edge>& edge() {return m_sedge;}
    std::shared_ptr<Face>& face() {return m_sface;}
    std::shared_ptr<Solid>& next() {return m_next;}
    std::shared_ptr<Solid>& prev() {return m_prev;}

    void printFaces() const;
    [[nodiscard]] std::pair<std::vector<Vec3>, std::vector<std::array<int, 2>>> getCurveFrame() const;

    void addVertex(std::shared_ptr<Vertex> v);
    void addEdge(std::shared_ptr<Edge> eg);
    void addFace(std::shared_ptr<Face> f);

    bool deleteFace(std::shared_ptr<Face> f);
    bool deleteEdge(std::shared_ptr<Edge> eg);
private:
    std::shared_ptr<Vertex> m_svertex;
    std::shared_ptr<Edge> m_sedge;
    std::shared_ptr<Face> m_sface;
    std::shared_ptr<Solid> m_prev, m_next;
};





#endif //EULER_SWEEP_BREP_H
