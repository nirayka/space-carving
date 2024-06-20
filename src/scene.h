#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
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
    std::vector<Camera*> getCamerasForVoxel(Voxel vox, sweepDir dir);
    std::vector<Camera*> cameras;
    int sceneWidth, sceneHeight, sceneDepth;
    float voxelSize;
    int xNumVoxels, yNumVoxels, zNumVoxels;

private:
    std::vector<Voxel> grid;
};


#endif // SCENE_H
