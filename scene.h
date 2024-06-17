#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "glm/glm.hpp"
#include "structs.h"
#include <stdio.h>
#include <vector>

// represents the scene to be space carved

class Scene
{
public:
    Scene(int inWidth, int inHeight, int inDepth, float inVoxSize);
    void initializeVoxels();
    Voxel& getVoxel(int x, int y, int z);
    int getWidth();
    int getHeight();
    int getDepth();
    std::vector<Camera> getCamerasForVoxel(Voxel vox, sweepDir dir);

private:
    int width, height, depth;
    float voxelSize;
    std::vector<Voxel> grid;
    std::vector<Camera> cameras;
};


#endif // SCENE_H
