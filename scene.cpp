#include "scene.h"

#include <iostream>
#include <vector>
#include "glm/glm.hpp"


Scene::Scene() {
    // currently assumes that all cameras/shapes are located such that 0 <= x, y, z, <= 100
    /* CHANGE IF DESIRED */
    width = 100;
    height = 100;
    depth = 100;
    voxelSize = 0.2;
    //

    xNumVoxels = width * (1 / voxelSize);
    yNumVoxels = height * (1 / voxelSize);
    zNumVoxels = depth * (1 / voxelSize);
    grid.resize(xNumVoxels * yNumVoxels * zNumVoxels);

    initializeVoxels();
}

void Scene::initializeVoxels() {
    for (int x = 0; x < xNumVoxels; ++x) {
        for (int y = 0; y < yNumVoxels; ++y) {
            for (int z = 0; z < zNumVoxels; ++z) {
                // x, y, z is position in grid of voxels
                Voxel& voxel = getVoxel(x, y, z);
                // this is position in worldspace
                voxel.pos = glm::vec3(x * voxelSize, y * voxelSize, z * voxelSize);
            }
        }
    }
}

std::vector<Camera*> Scene::getCamerasForVoxel(Voxel vox, sweepDir dir) {
    std::vector<Camera*> returnCams;

    // these four points represent the base of the pyramidal beam, i.e., they represent a plane
    // which is one face of the scene volume cube, parallel to current sweep plane, on the
    // side that is opposite to the sweep direction
    glm::vec3 p1, p2, p3, p4;

    switch (dir) {
        case posX:
            p1 = {0, 0, 0}; p2 = {0, height, 0}; p3 = {0, height, depth}; p4 = {0, 0, depth};
            break;
        case negX:
            p1 = {width, 0, 0}; p2 = {width, height, 0}; p3 = {width, height, depth}; p4 = {width, 0, depth};
            break;
        case posY:
            p1 = {0, 0, 0}; p2 = {0, 0, depth}; p3 = {width, 0, depth}; p4 = {width, 0, 0};
            break;
        case negY:
            p1 = {0, height, 0}; p2 = {0, height, depth}; p3 = {width, height, depth}; p4 = {width, height, 0};
            break;
        case posZ:
            p1 = {0, 0, 0}; p2 = {0, height, 0}; p3 = {width, height, 0}; p4 = {width, 0, 0};
            break;
        case negZ:
            p1 = {0, 0, depth}; p2 = {0, height, depth}; p3 = {width, height, depth}; p4 = {width, 0, depth};
            break;
    }

    // voxel position
    glm::vec3 v = vox.pos;

    // normal vectors for each face of the pyramid
    glm::vec3 n1 = glm::normalize(glm::cross(p2 - p1, v - p1));
    glm::vec3 n2 = glm::normalize(glm::cross(p3 - p2, v - p2));
    glm::vec3 n3 = glm::normalize(glm::cross(p4 - p3, v - p3));
    glm::vec3 n4 = glm::normalize(glm::cross(p1 - p4, v - p4));

    // check that all normal vectors point towards the interior of the pyramid
    if (glm::dot(n1, p3 - p1) > 0) n1 = -n1;
    if (glm::dot(n2, p4 - p2) > 0) n2 = -n2;
    if (glm::dot(n3, p1 - p3) > 0) n3 = -n3;
    if (glm::dot(n4, p2 - p4) > 0) n4 = -n4;

    for (Camera* cam : cameras) {
        // camera position
        glm::vec3 c = cam->pos;

        // check if camera is inside the pyramid
        if (glm::dot(n1, c - p1) >= 0 && glm::dot(n2, c - p2) >= 0 &&
            glm::dot(n3, c - p3) >= 0 && glm::dot(n4, c - p4) >= 0) {
            returnCams.push_back(cam);
        }
    }

    return returnCams;
}

// tocheck
Voxel& Scene::getVoxel(int x, int y, int z) {
    return grid[x + width * (y + height * z)];
}

int Scene::getWidth() { return width; }

int Scene::getHeight() { return height; }

int Scene::getDepth() { return depth; }
