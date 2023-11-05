//
// Created by root on 23-11-3.
//

#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "polyscope/polyscope.h"
#include "polyscope/curve_network.h"
#include "EulerOperation.h"
#include "utils.h"

TEST_CASE("construct a basic square", "[EulerOp]"){
    auto [S, V1] = EulerOp::mvsf({0,0,0});
    auto& lp = S->face()->loop();
    auto he1 = EulerOp::mev({1,0,0}, V1, lp);
    std::cout << "After making mev (0,0,0)-(1,0,0) : " << std::endl;
    S->printFaces();
    auto V2 = he1->sibling()->vertex();
    auto he2 = EulerOp::mev({1,0,1}, he1->sibling()->vertex(), lp);
    std::cout << "After making mev (1,0,0)-(1,0,1) : " << std::endl;
    S->printFaces();
    auto he3 = EulerOp::mev({0,0,1}, he2->sibling()->vertex(), lp);
    std::cout << "After making mev (1,0,1)-(0,0,1) : " << std::endl;
    S->printFaces();
    auto f = EulerOp::mef(V1, he3->sibling()->vertex(), lp);
    std::cout << "After making mef (0,0,1)-(0,0,0) : " << std::endl;
    S->printFaces();
}

TEST_CASE("construct a square with a inner square", "[EulerOp]"){
    auto [S, V1] = EulerOp::mvsf({0,0,0});
    auto& lp = S->face()->loop();
    auto he1 = EulerOp::mev({3,0,0}, V1, lp);
    auto V2 = he1->sibling()->vertex();
    auto he2 = EulerOp::mev({3,0,3}, he1->sibling()->vertex(), lp);
    auto he3 = EulerOp::mev({0,0,3}, he2->sibling()->vertex(), lp);
    auto f = EulerOp::mef(V1, he3->sibling()->vertex(), lp);
    auto he4 = EulerOp::mev({1,0,1}, V1, f->loop());
    std::cout << "After mev (0,0,0)-(1,0,1): " << std::endl;
    f->printLoops();
    auto conEdge = S->edge();
    auto he5 = EulerOp::mev({2,0,1}, he4->sibling()->vertex(), f->loop());
    std::cout << "After mev (1,0,1)-(2,0,1): " << std::endl;
    f->printLoops();
    auto he6 = EulerOp::mev({2,0,2}, he5->sibling()->vertex(), f->loop());
    std::cout << "After mev (2,0,1)-(2,0,2): " << std::endl;
    f->printLoops();
    auto he7 = EulerOp::mev({1,0,2}, he6->sibling()->vertex(), f->loop());
    std::cout << "After mev (2,0,2)-(1,0,2): " << std::endl;
    f->printLoops();
    auto f2 = EulerOp::mef(he5->vertex(), he7->sibling()->vertex(), f->loop());
    std::cout << "After mef (1,0,2)-(1,0,1): " << std::endl;
    S->printFaces();
    EulerOp::kemr(conEdge, f->loop());
    std::cout << "After kemr (0,0,0)-(1,0,1)" << std::endl;
    S->printFaces();
}

//TEST_CASE("display a square", "[FrameDisplay]")
//{
//    auto S = createSquare({0,0.5,0},1.0f);
//    auto [nodes, edges] = S->getCurveFrame();
//    polyscope::init();
//    polyscope::options::groundPlaneMode = polyscope::GroundPlaneMode::None;
//    polyscope::registerCurveNetwork("square", nodes, edges);
//    polyscope::show();
//}

TEST_CASE("display a double square", "[FrameDisplay]")
{
    auto S = createDoubleSquare({0,0,0},1.5f, 0.5f);
    auto [nodes, edges] = S->getCurveFrame();
    polyscope::init();
    polyscope::options::groundPlaneMode = polyscope::GroundPlaneMode::None;
    polyscope::registerCurveNetwork("square", nodes, edges);
    polyscope::show();
}
