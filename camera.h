#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "structs.h"

class Camera
{
public:
    Camera();
    bool fallsInBeam(sweepDir direction);

private:
    RGBA *photoData;
    glm::vec4 pos;
    glm::vec4 look;
    glm::vec4 up;

    float heightAngle; // The height angle of the camera in RADIANS

};

#endif // CAMERA_H
