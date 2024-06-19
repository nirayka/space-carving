#pragma once

#include "glm/glm.hpp"
#include <cstdint>


struct Voxel {
    glm::vec3 pos;
    bool isCarved;
    int observations;
    int redSum, redSumSquared;
    int greenSum, greenSumSquared;
    int blueSum, blueSumSquared;

    // Voxel() : sum_color(0.0f), sum_color_squared(0.0f), count(0), is_carved(false) {}
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

