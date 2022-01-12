#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Camera {
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 worldUp;
    glm::quat orientation;
    int **verticalWalls;
    int **horizontalWalls;
    float xMin;
    float xMax;
    float lastX;
    float lastY;
    float yaw;
    float pitch;
    void updateVectors();
public:
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 worldUp, int **verticalWalls, int **horizontalWalls);
    ~Camera();
    glm::mat4 getViewMatrix();
    void processKeyboard(direction direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);
    bool isCollision(glm::vec3 position);
    glm::vec3 getPosition() const {
        return position;
    }
};