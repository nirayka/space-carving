#include "scene.h"

Scene::Scene()
{

}

std::vector<Camera> Scene::getCamerasForVoxel(Voxel v, sweepDir dir) {
    std::vector<Camera> returnCams;

    for (Camera cam : cameras) {
        // If camera lies within the pyramidal beam of voxel v based on direction:
        if (true) {
            returnCams.push_back(cam);
        }
    }

}
