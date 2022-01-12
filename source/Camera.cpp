#include <Camera.h>
#include <iostream>
#include <GLUT/glut.h>

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 worldUp, int **verticalWalls, int **horizontalWalls) {
    this->position = position;
    this->front = front;
    this->worldUp = worldUp;
    this->verticalWalls = verticalWalls;
    this->horizontalWalls = horizontalWalls;
    this->lastX = 400.0f;
    this->lastY = 300.0f;
    this->yaw = -50.0f;
    this->pitch = 0.0f;
    this->orientation = glm::quat(0.0f, 0.0f, 0.0f, -1.0f);
    this->updateVectors();
}

Camera::~Camera() {

}

glm::mat4 Camera::getViewMatrix() {
    updateVectors();
    glm::quat conjugate = glm::conjugate(orientation);
    glm::mat4 rotate = glm::mat4_cast(conjugate);
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), -this->position);

    // std::cout<<"lastX: "<<lastX;
    // std::cout<<" front: "<<front.x<<" "<<front.y<<" "<<front.z;
    // std::cout<<" yaw: "<<this->yaw<<" pitch: "<<this->pitch;

    return rotate * translate;
}

bool Camera::isCollision(glm::vec3 position) {
    int x = (int) position.x / 200;
    int z = (int) position.z / 200;

    int modX = abs((int)position.x % 200);
    int modZ = abs((int)position.z % 200);
    if(x < 0 || x >= 7 || z < 0 || z >= 7) {
        return false;
    }
    // std::cout<<"x: "<<x<<" z: "<<z<<" vertical: "<<verticalWalls[x][z]<<" horizontal: "<<horizontalWalls[x][z]<<std::endl;
    // std::cout<<"modX: "<<modX<<" modZ: "<<modZ<<std::endl;
    bool insideVertical = (verticalWalls[x][z] == 1) && (modZ <= 30);
    // std::cout<<"cond 1: "<<verticalWalls[x][z]<<" cond 2: "<< (modZ <= 30) <<std::endl;
    bool insideHorizontal = (horizontalWalls[x][z] == 1) && (modX <= 30);
    // std::cout<<"insideVertical: "<<insideVertical<<" insideHorizontal: "<<insideHorizontal<<std::endl;
    if(insideHorizontal || insideVertical) {
        std::cout<<"collision"<<std::endl;
        // return true;
    }
    return false;
}

void Camera::processKeyboard(direction direction, float deltaTime) {
    glm::quat qt = this->orientation * glm::quat(0.0f, 0.0f, 0.0f, -1.0f) * glm::conjugate(this->orientation);
    this->front = glm::vec3(qt.x, qt.y, qt.z);
    glm::vec3 right = glm::normalize(glm::cross(this->front, this->worldUp));
    right.y = 0.0f;
    front.y = 0.0f;
    // std::cout<<" front: "<<front.x<<" "<<front.y<<" "<<front.z;
    // std::cout<<" right: "<<right.x<<" "<<right.y<<" "<<right.z<<std::endl;
    // std::cout<<"x:"<<glm::sin(qt.x)<<" y:"<<glm::cos(qt.y)<<" yaw:"<<yaw<<std::endl;

    glm::vec3 positionTemp = this->position;

    switch (direction) {
        case UP:
            positionTemp += front * deltaTime;
            break;
        case DOWN:
            positionTemp -= front * deltaTime;
            break;
        case LEFT:
            positionTemp += right * deltaTime;
            break;
        case RIGHT:
            positionTemp -= right * deltaTime;
            break;
    }

    if(!this->isCollision(positionTemp)) {
        this->position = positionTemp;
    }

    // std::cout<<"position: "<<this->position.x<<" "<<this->position.y<<" "<<this->position.z<<std::endl;
    
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    std :: cout << "xoffset: " << xoffset << " yoffset: " << yoffset << std::endl;
    if(xoffset <= 30.0)
        this->yaw += 1.0f;

    if(xoffset >= glutGet(GLUT_WINDOW_WIDTH) - 30.0)
        this->yaw -= 1.0f;

    float deltaX = xoffset - this->lastX;
    float deltaY = yoffset - this->lastY;
    this->lastX = xoffset;
    this->lastY = yoffset;

    this->yaw -= deltaX / 20.0f;
    this->pitch += deltaY / 20.0f;
    if(this->pitch > 15.0f)
        this->pitch = 15.0f;
    if(this->pitch < -15.0f)
        this->pitch = -15.0f;

    this->updateVectors();
}

void Camera::updateVectors(){
    glm::quat xQuat = glm::angleAxis(glm::radians(this->yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat yQuat = glm::angleAxis(glm::radians(this->pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    this->orientation = glm::normalize(yQuat * xQuat);
}
