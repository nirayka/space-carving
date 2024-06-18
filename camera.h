#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "structs.h"

class Camera
{
public:
    Camera(RGBA *data, glm::vec4 inPos, glm::vec4 inLook, glm::vec4 inUp, int width, int height, float inHeightAng);

    glm::mat4 calculateViewMatrix() const;
    void getInverseViewMatrix() const;

    bool fallsInBeam(sweepDir direction) const;
    glm::vec2 projectVoxelToImage(Voxel v);
    RGBA getColorFromProjection(glm::vec2 coords);

    glm::mat4 getViewMatrix() const;
    float getHeightAngle() const;
    float calculateAspectRatio() const;

    glm::vec4 pos;
    glm::mat4 viewMatrix;
    glm::mat4 inverseViewMatrix;

    int imageWidth;
    int imageHeight;
    float aspectRatio;
    float heightAngle; // in radians

private:
    RGBA *photoData;
    glm::vec4 look;
    glm::vec4 up;
};

#endif // CAMERA_H
