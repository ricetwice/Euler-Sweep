#include "polyscope/polyscope.h"
#include "polyscope/surface_mesh.h"
#include "polyscope/point_cloud.h"
#include "EulerOperation.h"

// Initialize polyscope
int main()
{
    polyscope::init();

    std::vector<glm::vec3> points({glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1)});

// Register a point cloud
// `points` is a Nx3 array-like container of points
    polyscope::registerPointCloud("my points", points);

// View the point cloud and mesh we just registered in the 3D UI
    polyscope::show();

}