#ifndef SCENE_H
#define SCENE_H

#include"camera.h"
#include "glm/glm.hpp"
#include "structs.h"
#include <stdio.h>
#include <vector>

// represents the scene to be space carved

class Scene
{
public:
    Scene();
    // getters here


private:
    // voxel volume
    std::vector<Camera> cameras;
};


#endif // SCENE_H
