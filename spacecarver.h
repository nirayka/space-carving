#ifndef SPACECARVER_H
#define SPACECARVER_H

#include "camera.h"
#include "structs.h"
#include <vector>

class SpaceCarver
{
public:
    SpaceCarver();

    void multiSweep();
    void planeSweep(sweepDir dir);
    bool isConsistent(Voxel v, std::vector<Camera>);
};

#endif // SPACECARVER_H
