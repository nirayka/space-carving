#include "spacecarver.h"
#include "structs.h"

SpaceCarver::SpaceCarver()
{
    // TODO
    // intiialize the scene so that its a cube in the positive x y z octant and that
    // it contains all cameras
}

bool SpaceCarver::isConsistent(Voxel v) {
    float redStdDev = (1/(v.seenCount)) *
                      (v.redSumSquared - ((1/v.seenCount) * v.redSumSquared * v.redSumSquared));

    float greenStdDev = (1/(v.seenCount)) *
                      (v.greenSumSquared - ((1/v.seenCount) * v.greenSumSquared * v.greenSumSquared));

    float blueStdDev = (1/(v.seenCount)) *
                      (v.blueSumSquared - ((1/v.seenCount) * v.blueSumSquared * v.blueSumSquared));

    if (redStdDev > threshold || greenStdDev > threshold || blueStdDev > threshold) {
        return false;
    }
    return true;
}


void SpaceCarver::helpPlaneSweep(Voxel voxel, sweepDir dir) {
    std::vector<Camera> voxCams = scene.getCamerasForVoxel(voxel, dir);

    for (Camera& camera : voxCams) {
        glm::vec2 projection = camera.projectVoxelToImage(voxel);
        RGBA color = camera.getColorFromProjection(projection);
        voxel.redSum += color.r; voxel.greenSum += color.g; voxel.blueSum += color.b;
        voxel.redSumSquared += color.r * color.r;
        voxel.greenSumSquared += color.g * color.g;
        voxel.blueSumSquared += color.b * color.b;
        voxel.seenCount += 1;

        if (!isConsistent(voxel)) {
            voxel.isCarved = true;
        }
    }
}


void SpaceCarver::planeSweep(sweepDir dir) {
    if (dir == posX) {
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
    } else if (dir == negX) {
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
    } else if (dir == posY) {
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
    } else if (dir == negY) {
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
    } else if (dir == posZ) {
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
    } else if (dir == negZ) {
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
    }
}


void SpaceCarver::multiSweep() {
    std::vector<sweepDir> directions = {posX, negX, posY, negY, posZ, negZ};

    bool voxelsRemoved;
    do {
        voxelsRemoved = false;

        for (sweepDir direction : directions) {
            planeSweep(direction);
        }

        for (int x = 0; x < scene.getWidth(); ++x) {
            for (int y = 0; y < scene.getHeight(); ++y) {
                for (int z = 0; z < scene.getDepth(); ++z) {
                    Voxel& voxel = scene.getVoxel(x, y, z);
                    if (voxel.isCarved) continue;

                    if (isConsistent(voxel)) {
                        voxel.isCarved = true;
                        voxelsRemoved = true;
                    }
                }
            }
        }
    } while (voxelsRemoved);

    produceOutput();
}

void SpaceCarver::produceOutput() {
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
