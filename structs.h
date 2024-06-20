#pragma once

#include "glm/glm.hpp"
#include <cstdint>


struct Voxel {
    glm::vec3 pos;
    bool isCarved;
    int observations;
    float redSum, redSumSquared;
    float greenSum, greenSumSquared;
    float blueSum, blueSumSquared;
};

enum sweepDir {
    posX,
    negX,
    posY,
    negY,
    posZ,
    negZ
};


struct RGBA {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a = 255;
};

