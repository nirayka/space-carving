#include "camera.h"

Camera::Camera(RGBA *data, glm::vec4 inPos, glm::vec4 inLook, glm::vec4 inUp, int width, int height, float inHeightAng) {
    RGBA *photoData = data;
    pos = inPos;
    look = inLook;
    up = inUp;
    sceneWidth = width;
    sceneHeight = height;
    heightAngle = inHeightAng;
    viewMatrix = calculateViewMatrix();
    inverseViewMatrix = inverse(viewMatrix);
}

glm::mat4 Camera::calculateViewMatrix() const {
    float px = pos[0]; float py = pos[1]; float pz = pos[2];

    glm::vec3 pos3 = pos;
    glm::vec3 look3 = look;
    glm::vec3 up3 = up;

    glm::vec3 w = -glm::normalize(look3);
    glm::vec3 v = glm::normalize(up3 - (glm::dot(up3, w) * w));
    glm::vec3 u = glm::cross(v, w);

    glm::mat4 rotate = glm::mat4(u[0], v[0], w[0], 0.f,
                                 u[1], v[1], w[1], 0.f,
                                 u[2], v[2], w[2], 0.f,
                                 0.f, 0.f, 0.f, 1.f);
    glm::mat4 translate = glm::mat4(1.f, 0.f, 0.f, 0.f,
                                    0.f, 1.f, 0.f, 0.f,
                                    0.f, 0.f, 1.f, 0.f,
                                    -px, -py, -pz, 1.f);

    glm::mat4 result = rotate * translate;

    return glm::mat4(result);
}


float Camera::getAspectRatio() const {
    return (float) sceneWidth / (float) sceneHeight;
}

float Camera::getHeightAngle() const {
    return heightAngle;
}
