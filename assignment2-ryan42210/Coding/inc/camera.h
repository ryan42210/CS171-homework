#ifndef CAMERA_H
#define CAMERA_H

#include "../3rdLibs/glm/glm/glm.hpp"
#include "../3rdLibs/glm/glm/gtc/type_ptr.hpp"
#include "../3rdLibs/glm/glm/gtc/matrix_transform.hpp"

class Camera {
public:
    Camera(glm::vec3 pos, glm::vec3 gaze = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) {
        camPos = pos;
        camGaze = gaze;
        camUp = up;
        pitch = 0.0, yaw = -90.0;
        view = glm::lookAt(camPos, camPos+camGaze, camUp);
    }
    glm:: mat4 getViewMat() const {
        return view;
    }

    void moveUp(float dist) {
        camPos += camUp * dist;
        updateView();
    }
    void moveDown(float dist) {
        camPos -= camUp * dist;
        updateView();
    }
    void moveLeft(float dist) {
        camPos -= glm::normalize(glm::cross(camGaze, camUp)) * dist;
        updateView();
    }
    void moveRight(float dist){
        camPos += glm::normalize(glm::cross(camGaze, camUp)) * dist;
        updateView();
    }
    void moveFront(float dist){
        camPos += camGaze * dist;
        updateView();
    }
    void moveBack(float dist){
        camPos -= camGaze * dist;
        updateView();
    }
    void rotate(double delt_x, double delt_y) {
        pitch += delt_y;
        if (pitch > 89.0) pitch = 89.0;
        if (pitch < -89.0) pitch = -89.0;
        yaw += delt_x;
        glm::vec3 gaze;
        gaze.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        gaze.y = sin(glm::radians(pitch));
        gaze.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        camGaze = glm::normalize(gaze);
        updateView();
    }
    // void yawRight(float angle) {
    //     glm::mat4 rotate_mat;
    //     rotate_mat = glm::rotate(rotate_mat, glm::radians(-angle*10), camUp);
    //     camGaze = glm::mat3(rotate_mat) * camGaze;
    //     updateView();
    // }

private:
    void updateView() {
        view = glm::lookAt(camPos, camPos+camGaze, camUp);
        return;
    }

    glm::vec3 camPos;
    glm::vec3 camGaze;
    glm::vec3 camUp;
    glm::mat4 view;

    double pitch, yaw;
};

#endif