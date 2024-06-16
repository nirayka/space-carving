#include "scene.h"

#include <iostream>
#include <vector>
#include "glm/glm.hpp"


Scene::Scene(int inWidth, int inHeight, int inDepth, float inVoxSize) {
    width = inWidth;
    height = inHeight;
    depth = inDepth;
    voxelSize = inVoxSize;
    grid.resize(width * height * depth);
    initializeVoxels();
}

void Scene::initializeVoxels() {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            for (int z = 0; z < depth; ++z) {
                Voxel& voxel = getVoxel(x, y, z);
                voxel.pos = glm::vec3(x * voxelSize, y * voxelSize, z * voxelSize);
            }
        }
    }
}

Voxel& Scene::getVoxel(int x, int y, int z) {
    return grid[x + width * (y + height * z)];
}

// TODO
std::vector<Camera> Scene::getCamerasForVoxel(Voxel v, sweepDir dir) {
    std::vector<Camera> returnCams;

    for (Camera cam : cameras) {
        // If camera lies within the pyramidal beam of voxel v based on direction:
        if (true) {
            returnCams.push_back(cam);
        }
    }

    return returnCams;
}

int Scene::getWidth() { return width; }

int Scene::getHeight() { return height; }

int Scene::getDepth() { return depth; }
