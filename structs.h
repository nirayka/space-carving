#pragma once

#include "glm/glm.hpp"
#include <cstdint>

struct RGBA {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a = 255;
};

struct Voxel {
    glm::vec3 pos;
    bool isCarved;
    int sweepCount;
    int redSum, redSumSquared;
    int greenSum, greenSumSquared;
    int blueSum, blueSumSquared;
};

enum sweepDir {
    posX,
    negX,
    posY,
    negY,
    posZ,
    negZ
};
