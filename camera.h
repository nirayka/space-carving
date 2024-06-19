#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "structs.h"
#include <vector>

class Camera
{
public:
    Camera(std::vector<RGBA>* data, glm::vec4 inPos, glm::vec4 inLook, glm::vec4 inUp);

    glm::mat4 calculateViewMatrix() const;
    void getInverseViewMatrix() const;

    bool fallsInBeam(sweepDir direction) const;
    glm::vec2 projectVoxelToImage(Voxel v);
    RGBA getColorFromProjection(glm::vec2 coords);

    glm::mat4 getViewMatrix() const;
    float getHeightAngle() const;
    float calculateAspectRatio() const;

    glm::vec4 pos;
    glm::vec4 look;
    glm::vec4 up;
    glm::mat4 viewMatrix;
    glm::mat4 inverseViewMatrix;
    std::vector<RGBA> *photoData;

    int imageWidth;
    int imageHeight;
    float aspectRatio;
    float heightAngle; // in radians

};

#endif // CAMERA_H
