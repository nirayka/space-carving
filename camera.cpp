#include "camera.h"
#include <cmath>

Camera::Camera(std::vector<RGBA> *data, glm::vec4 inPos, glm::vec4 inLook, glm::vec4 inUp) {
    photoData = data;
    pos = inPos;
    look = inLook;
    up = inUp;


    /* CHANGE IF DESIRED */
    imageWidth = 100;
    imageHeight = 100;
    heightAngle = M_PI / 4;
    //

    aspectRatio = calculateAspectRatio();
    viewMatrix = calculateViewMatrix();
    inverseViewMatrix = inverse(viewMatrix);
}

float Camera::calculateAspectRatio() const {
    return (float) imageWidth / (float) imageHeight;
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

glm::vec2 Camera::projectVoxelToImage(Voxel v) {
    // convert coordinates to camera space
    glm::vec4 voxWorldSpacePos(v.pos, 1.0f);
    glm::vec4 voxCameraSpace = viewMatrix * voxWorldSpacePos;

    // perform basic perspective division
    float x = voxCameraSpace.x / voxCameraSpace.z;
    float y = voxCameraSpace.y / voxCameraSpace.z;
    // TOCHECK: does this need to be like raytracing or something??

    // intrinsic parameters ignored here

    return glm::vec2{x, y};
}


RGBA Camera::getColorFromProjection(glm::vec2 coords) {
    // Given normalized image space coordinates
    float floatI = (imageWidth * (coords.x + 0.5)) - 0.5;
    float floatJ = (imageHeight * (coords.y + 0.5)) - 0.5;

    int i = floor(floatI);
    int j = floor(floatJ);

    int oneDimCoord = j * imageWidth + i;

    return photoData->at(oneDimCoord);
}
