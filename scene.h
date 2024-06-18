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
    Scene();
    void initializeVoxels();
    Voxel& getVoxel(int x, int y, int z);
    int getWidth();
    int getHeight();
    int getDepth();
    std::vector<Camera*> getCamerasForVoxel(Voxel vox, sweepDir dir);
    std::vector<Camera*> cameras;

private:
    int width, height, depth;
    float voxelSize;
    int xNumVoxels, yNumVoxels, zNumVoxels;
    std::vector<Voxel> grid;
};


#endif // SCENE_H
