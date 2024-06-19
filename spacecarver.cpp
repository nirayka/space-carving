#include "spacecarver.h"
#include "structs.h"
#include <iostream>
#include <ostream>

SpaceCarver::SpaceCarver()
{
    scene = Scene();
    threshold = .05; // CHANGE IF DESIRED
}

bool SpaceCarver::isConsistent(Voxel v) {
    float redStdDev = (1/(v.observations)) *
                      (v.redSumSquared - ((1/v.observations) * v.redSumSquared * v.redSumSquared));

    float greenStdDev = (1/(v.observations)) *
                      (v.greenSumSquared - ((1/v.observations) * v.greenSumSquared * v.greenSumSquared));

    float blueStdDev = (1/(v.observations)) *
                      (v.blueSumSquared - ((1/v.observations) * v.blueSumSquared * v.blueSumSquared));

    if (redStdDev > threshold || greenStdDev > threshold || blueStdDev > threshold) {
        return false;
    }
    return true;
}


void SpaceCarver::helpPlaneSweep(Voxel voxel, sweepDir dir) {
    std::vector<Camera*> voxCams = scene.getCamerasForVoxel(voxel, dir);

    for (Camera* camera : voxCams) {
        glm::vec2 projection = camera->projectVoxelToImage(voxel);
        std::pair<bool, RGBA> inBoundsAndColor = camera->getColorFromProjection(projection);

        // if point p is not visible from c, it is trivially photo-consistent with c
        bool voxelInBounds = inBoundsAndColor.first;
        if (!voxelInBounds) {
            return;
        }

        // if point p is a background color, carve
        RGBA color = inBoundsAndColor.second;
        if (color.r == 0 && color.g == 0 && color.b == 0) {
            voxel.isCarved = true;
            voxelsRemoved = true;
            return;
        }

        // otherwise, keep track of per-voxel statistics
        voxel.redSum += color.r; voxel.greenSum += color.g; voxel.blueSum += color.b;
        voxel.redSumSquared += color.r * color.r;
        voxel.greenSumSquared += color.g * color.g;
        voxel.blueSumSquared += color.b * color.b;
        voxel.observations += 1;

        // and if photo-inconsistent, carve
        if (!isConsistent(voxel)) {
            voxel.isCarved = true;
            voxelsRemoved = true;
        }
    }
}


void SpaceCarver::planeSweep(sweepDir dir) {
    if (dir == posX) {
        std::cout << "Start +X plane-sweeping" << std::endl;
        for (int x = 0; x < scene.getWidth(); ++x) {
            for (int y = 0; y < scene.getHeight(); ++y) {
                for (int z = 0; z < scene.getDepth(); ++z) {
                    Voxel& voxel = scene.getVoxel(x, y, z);
                    if (!voxel.isCarved) {
                        helpPlaneSweep(voxel, dir);
                    }
                }
            }
        }
        std::cout << "Finished +X plane-sweeping" << std::endl;
    } else if (dir == negX) {
        std::cout << "Start -X plane-sweeping" << std::endl;
        for (int x = scene.getWidth() - 1; x >= 0; --x) {
            for (int y = 0; y < scene.getHeight(); ++y) {
                for (int z = 0; z < scene.getDepth(); ++z) {
                    Voxel& voxel = scene.getVoxel(x, y, z);
                    if (!voxel.isCarved) {
                        helpPlaneSweep(voxel, dir);
                    }
                }
            }
        }
        std::cout << "Finished -X plane-sweeping" << std::endl;
    } else if (dir == posY) {
        std::cout << "Start +Y plane-sweeping" << std::endl;
        for (int y = 0; y < scene.getHeight(); ++y) {
            for (int x = 0; x < scene.getWidth(); ++x) {
                for (int z = 0; z < scene.getDepth(); ++z) {
                    Voxel& voxel = scene.getVoxel(x, y, z);
                    if (!voxel.isCarved) {
                        helpPlaneSweep(voxel, dir);
                    }
                }
            }
        }
        std::cout << "Finished +Y plane-sweeping" << std::endl;
    } else if (dir == negY) {
        std::cout << "Start -Y plane-sweeping" << std::endl;
        for (int y = scene.getHeight() - 1; y >= 0; --y) {
            for (int x = 0; x < scene.getWidth(); ++x) {
                for (int z = 0; z < scene.getDepth(); ++z) {
                    Voxel& voxel = scene.getVoxel(x, y, z);
                    if (!voxel.isCarved) {
                        helpPlaneSweep(voxel, dir);
                    }
                }
            }
        }
        std::cout << "Finished -Y plane-sweeping" << std::endl;
    } else if (dir == posZ) {
        std::cout << "Start +Z plane-sweeping" << std::endl;
        for (int z = 0; z < scene.getDepth(); ++z) {
            for (int x = 0; x < scene.getWidth(); ++x) {
                for (int y = 0; y < scene.getHeight(); ++y) {
                    Voxel& voxel = scene.getVoxel(x, y, z);
                    if (!voxel.isCarved) {
                        helpPlaneSweep(voxel, dir);
                    }
                }
            }
        }
        std::cout << "Finished +Z plane-sweeping" << std::endl;
    } else if (dir == negZ) {
        std::cout << "Start -Z plane-sweeping" << std::endl;
        for (int z = scene.getDepth() - 1; z >= 0; --z) {
            for (int x = 0; x < scene.getWidth(); ++x) {
                for (int y = 0; y < scene.getHeight(); ++y) {
                    Voxel& voxel = scene.getVoxel(x, y, z);
                    if (!voxel.isCarved) {
                        helpPlaneSweep(voxel, dir);
                    }
                }
            }
        }
        std::cout << "Finished -Z plane-sweeping" << std::endl;
    }
}


void SpaceCarver::multiSweep() {
    std::vector<sweepDir> directions = {posX, negX, posY, negY, posZ, negZ};

    std::cout << "Start multi-sweeping" << std::endl;

    do {
        voxelsRemoved = false;

        for (sweepDir direction : directions) {
            planeSweep(direction);
        }
    } while (voxelsRemoved);

    std::cout << "Multi-sweeping finished" << std::endl;

    produceOutput();
}

void SpaceCarver::produceOutput() {
    // will implement later, do not touch
    /*
     * create string
     * add beginning of json file
     * iterate through all voxels
         * for each voxel:
         * if not carved, add to string
     * add end of json file
     * put string into json file
     * save json file
     *
    */
}
