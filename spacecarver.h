#ifndef SPACECARVER_H
#define SPACECARVER_H

#include "scene.h"
#include "structs.h"
#include <vector>

class SpaceCarver
{
public:
    SpaceCarver();
    bool isConsistent(Voxel v);
    void helpPlaneSweep(Voxel voxel, sweepDir dir);
    void planeSweep(sweepDir dir);
    void multiSweep();


private:
    Scene scene;
    int threshold;
};

#endif // SPACECARVER_H
