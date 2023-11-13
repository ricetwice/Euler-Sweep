#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"
#include "polyscope/point_cloud.h"
#include "EulerOperation.h"
#include "sweep.h"
#include "utils.h"




// Initialize polyscope
int main()
{
    auto S = createDoubleSquare({0,0,0},  1.5f, 0.5f);
    sweep(S->face(), {0,0,1}, 1.0f);
    S->printFaces();
    auto [V, F] = S->getTriangulation();
    polyscope::init();
//    polyscope::options::groundPlaneMode = polyscope::GroundPlaneMode::None;
    auto mesh = polyscope::registerSurfaceMesh("cube", V, F);
    mesh->setBackFacePolicy(polyscope::BackFacePolicy::Custom);
    polyscope::show();

}