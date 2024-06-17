#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "structs.h"

class Camera
{
public:
    Camera(RGBA *data, glm::vec4 inPos, glm::vec4 inLook, glm::vec4 inUp, int width, int height, float inHeightAng);

    glm::mat4 calculateViewMatrix() const;
    void getViewMatrix() const;
    void getInverseViewMatrix() const;

    glm::vec2 projectVoxelToImage(Voxel v);
    glm::vec3 getColorFromProjection(glm::vec2 coords);

    float getHeightAngle() const;
    float getAspectRatio() const;

    glm::vec4 pos;

private:
    RGBA *photoData;
    glm::vec4 look;
    glm::vec4 up;

    glm::mat4 viewMatrix;
    glm::mat4 inverseViewMatrix;

    int sceneWidth;
    int sceneHeight;
    float heightAngle; // in radians
};

#endif // CAMERA_H
