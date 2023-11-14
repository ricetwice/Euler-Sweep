#include "polyscope/polyscope.h"
#include "polyscope/curve_network.h"
#include "polyscope/surface_mesh.h"
#include "polyscope/point_cloud.h"
#include "EulerOperation.h"
#include "sweep.h"
#include "utils.h"

using namespace polyscope;

bool isFrameDisplay = false;

const char * model_names[] = {"cube", "cube_with_a_hole"};
const int num_models = 2;
static int cur_model = 1;
enum model {CUBE, CUBE_WAH};

SurfaceMesh* mesh[num_models];
CurveNetwork* curve[num_models];

void registerCubeWithHole()
{
    auto S = createDoubleSquare({0,0,0},  1.5f, 0.5f);
    sweep(S->face(), {0,0,1}, 1.0f);
    auto [V, F] = S->getTriangulation();
    mesh[CUBE_WAH] = polyscope::registerSurfaceMesh(model_names[CUBE_WAH], V, F);
    auto [VC, E] = S->getCurveFrame();
    curve[CUBE_WAH] = polyscope::registerCurveNetwork(model_names[CUBE_WAH] ,VC, E);
    curve[CUBE_WAH]->setEnabled(false);
}

void registerCube()
{
    auto S = createSquare({0,0,0}, 1.0f);
    sweep(S->face(), {0,0,1}, 1.0f);
    auto [V,F] = S->getTriangulation();
    mesh[CUBE] = polyscope::registerSurfaceMesh(model_names[CUBE], V, F);
    auto [VC, E] = S->getCurveFrame();
    curve[CUBE] = polyscope::registerCurveNetwork(model_names[CUBE] ,VC, E);
    curve[CUBE]->setEnabled(false);
}

void (*registerFunctions[])() = {registerCube, registerCubeWithHole};

void registerModels()
{
    for (auto & registerFunction : registerFunctions)
    {
        registerFunction();
    }
}

void customUI()
{
    ImGui::Checkbox("Frame Display", &isFrameDisplay);
    ImGui::ListBox("Models", &cur_model , model_names, num_models);
    for (int i = 0 ; i < num_models; ++i)
    {
        mesh[i]->setEnabled(false);
        curve[i]->setEnabled(false);
    }

    curve[cur_model]->setEnabled(isFrameDisplay);
    mesh[cur_model]->setEnabled(!isFrameDisplay);

}

// Initialize polyscope
int main()
{
    registerModels();

    polyscope::init();

    polyscope::state::userCallback = customUI;
//    polyscope::options::groundPlaneMode = polyscope::GroundPlaneMode::None;
    polyscope::show();

}