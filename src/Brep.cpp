//
// Created by root on 23-11-2.
//

#include "Brep.h"
#include <igl/triangle/triangulate.h>
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

Axis Face::getFaceAxis() const {
    auto he = m_floop->halfEdge();
    if (he == nullptr)
    {
        std::cerr << "The face is empty!" << std::endl;
        exit(-1);
    }
    Vec3 X = he->getVec().normalized();
    he = he->next();
    Vec3 X_next = he->getVec().normalized();
    Vec3 N = X.cross(X_next);
    Vec3 Y = N.cross(X);
    return Axis{he->vertex()->pos(), X, Y};
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
        if (curF->prev())
            curF->prev()->next() = curF->next();
        else
        {
            m_sface = curF->next();
        }

        if (curF->next())
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

std::pair<Eigen::MatrixXf, Eigen::MatrixXi> Solid::getTriangulation() const {
    std::vector<Eigen::MatrixXf> Vs;
    std::vector<Eigen::MatrixXi> Fs;
    int v_cnt = 0, f_cnt = 0;
    for (auto f = m_sface; f != nullptr; f = f->next())
    {
        //construct 2D V & E
        std::vector<Vec2> fV_2D, points_in_holes;
        std::vector<std::array<int ,2>> edge_indices;
        std::unordered_map<std::shared_ptr<Vertex>, int> fV_recorder;
        Axis axis = f->getFaceAxis();
        int fv_cnt = 0;
        for (auto l = f->loop(); l != nullptr; l = l->next())
        {
            Vec2 POH = Vec2::Zero();
            int lp_v_cnt = 0;
            auto he = l->halfEdge();
            fV_2D.push_back(project2D(he->vertex()->pos(), axis));
            if (l != f->loop())
            {
                POH += fV_2D.back();
                lp_v_cnt++;
            }
            fV_recorder[he->vertex()] = fv_cnt++;
            fV_recorder[he->tipVertex()] = fv_cnt++;
            edge_indices.push_back({fV_recorder[he->vertex()], fV_recorder[he->tipVertex()]});
            for(he = he->next(); he != l->halfEdge(); he = he->next())
            {
                fV_2D.push_back(project2D(he->vertex()->pos(), axis));
                if (!fV_recorder.count(he->tipVertex()))
                    fV_recorder[he->tipVertex()] = fv_cnt++;
                edge_indices.push_back({fV_recorder[he->vertex()], fV_recorder[he->tipVertex()] });
                if (l != f->loop())
                {
                    POH += fV_2D.back();
                    lp_v_cnt++;
                }
            }
            if (l != f->loop())
                points_in_holes.emplace_back(POH / lp_v_cnt);
        }
        Eigen::MatrixXf V((long)fV_2D.size(), 2);
        Eigen::MatrixXi E((long)edge_indices.size(), 2);
        Eigen::MatrixXf H((long)points_in_holes.size(), 2);
        for (int i = 0; i < fV_2D.size(); ++i)
            V.row(i) = fV_2D[i];
        for (int i = 0 ; i < edge_indices.size(); ++i)
            E.row(i) << edge_indices[i][0],edge_indices[i][1];
        for (int i = 0; i < points_in_holes.size(); ++i)
            H.row(i) << points_in_holes[i];
//        std::cout << "V : " << V << std::endl;
//        std::cout << "H : " << H << std::endl;
        // add point in the hole
        // Triangulated interior
        Eigen::MatrixXf V2;
        Eigen::MatrixXi F2;
        //triangulation
        igl::triangle::triangulate(V, E, H,"", V2, F2);
        //store the triangulation : map F with vertices
        Eigen::MatrixXf V2_3D( V2.rows(), 3);
        for (int i = 0; i < V2.rows(); ++i)
        {
            V2_3D.row(i) << recover3D(V2.row(i), axis);
        }
//        axis.print();
//        std::cout << "V2 : " << V2 << std::endl;
//        std::cout << "V2_3D : " << V2_3D << std::endl;
        F2 += Eigen::MatrixXi::Ones( F2.rows(), 3) * v_cnt;
        v_cnt += fv_cnt;
        f_cnt += F2.rows();
        Vs.push_back(std::move(V2_3D));
        Fs.push_back(std::move(F2));
    }

    //assemble V and F
    Eigen::MatrixXf V(v_cnt, 3);
    Eigen::MatrixXi F(f_cnt, 3);
    size_t cur_vcnt = 0, cur_fcnt = 0;
    for (auto & v : Vs)
    {
        V.block(cur_vcnt, 0,  v.rows() , 3)  << v;
        cur_vcnt += v.rows();
    }
    for (auto & f : Fs)
    {
        F.block(cur_fcnt, 0, f.rows(), 3) << f;
        cur_fcnt += f.rows();
    }

    return {V, F};
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

Vec3 HalfEdge::getVec() const {
    return this->tipVertex()->pos() - m_vertex->pos();
}
