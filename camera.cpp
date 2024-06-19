#include "camera.h"
#include <cmath>
#include <iostream>
#include <ostream>

Camera::Camera(std::vector<RGBA> *data, glm::vec4 inPos, glm::vec4 inLook, glm::vec4 inUp, int inWidth, int inHeight) {
    photoData = data;
    pos = inPos;
    look = inLook;
    up = inUp;
    imageWidth = inWidth;
    imageHeight = inHeight;

    /* CHANGE IF DESIRED */
    heightAngle = M_PI / 4;
    viewPlaneDepth = 1;

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

/* projects the voxel onto the camera's view plane */
glm::vec2 Camera::projectVoxelToImage(Voxel vox) {
    // convert coordinates to camera space
    glm::vec4 voxWorldSpacePos(vox.pos, 1.0f);
    glm::vec4 voxCameraSpace = viewMatrix * voxWorldSpacePos;

    // perform basic perspective division
    float u = voxCameraSpace.x / voxCameraSpace.z;
    float v = voxCameraSpace.y / voxCameraSpace.z;

    // scale to normalized device coordinates
    float viewPlaneHeight = 2 * viewPlaneDepth * tan(heightAngle / 2);
    float viewPlaneWidth = viewPlaneHeight * aspectRatio;
    float x = u / viewPlaneWidth;
    float y = v / viewPlaneHeight;

    return glm::vec2{x, y};
}

/* returns true and the color of the projection if within image bounds,
 * otherwise returns false and black */
std::pair<bool, RGBA> Camera::getColorFromProjection(glm::vec2 coords) {
    // convert normalized device coordinates to pixel coordinates
    float floatPixelX = (coords.x + 1.f) * 0.5f * imageWidth;
    float floatPixelY = (1.f - coords.y) * 0.5f * imageHeight;

    int pixelX = round(floatPixelX);
    int pixelY = round(floatPixelY);

    // check if projection is within image bounds
    if (pixelX < 0 || pixelX >= imageWidth || pixelY < 0 || pixelY >= imageHeight) {
        return std::make_pair(false, RGBA{0, 0, 0, 0});
    }

    // get color from image data
    int pixelIndex = pixelY * imageWidth + pixelX;
    RGBA color = photoData->at(pixelIndex);

    return std::make_pair(true, color);
}
